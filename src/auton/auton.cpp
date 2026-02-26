#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "globals.h"
#include "selector.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "util/colorsort.h"
#include <iostream>

// Forward declaration if getColor is not in colorsort.h
Alliance getColor(double hue);

pros::Distance dNorth(PORT_FRONT_DISTANCE);
pros::Distance dSouth(PORT_BACK_DISTANCE);
pros::Distance dEast(PORT_RIGHT_DISTANCE);
pros::Distance dWest(PORT_LEFT_DISTANCE);
pros::Optical colorSensor(PORT_OPTICAL);

void move_relative(double x, double y, double timeout, bool forwards,
                   float speed = 127) {
  chassis.moveToPoint(chassis.getPose().x + x, chassis.getPose().y + y, timeout,
                      {forwards, speed});
}
void drive_distance(double distance, double timeout, float speed = 127) {
  chassis.moveToPoint(
      chassis.getPose().x +
          distance * cos((-chassis.getPose().theta + 90) * M_PI / 180.0),
      chassis.getPose().y +
          distance * sin((-chassis.getPose().theta + 90) * M_PI / 180.0),
      timeout, {(distance >= 0), speed});
}
void drive_distance_from_wall(double distance, double timeout,
                              float speed = 127, bool from_front = true) {
  double exit_range = 2.0;
  double field_length = 144.0;
  double dist_from_wall = 0;
  double time = pros::millis();
  bool forwards = true;
  while (std::abs(dist_from_wall - distance) > exit_range &&
         (pros::millis() - time) < timeout) {
    double front_distance = dNorth.get() / 25.4;
    double back_distance = dSouth.get() / 25.4;
    bool front_valid = false;
    bool back_valid = false;
    if (front_distance < 2000 / 25.4 && front_distance > 10 / 25.4)
      front_valid = true;
    if (back_distance < 2000 / 25.4 && back_distance > 10 / 25.4)
      back_valid = true;
    if (from_front && front_valid) {
      dist_from_wall = front_distance - 2;
      forwards = true;

    } else if (back_valid && !from_front) {
      dist_from_wall = back_distance + 5.5;
      forwards = false;
    }
    drive_distance((forwards ? 1 : -1) * (dist_from_wall - distance), 5000,
                   speed);
    chassis.waitUntilDone();
    pros::delay(20);
  }
}
// TODO
void drive_until_detect_distance(double distance, double timeout, float speed,
                                 bool forward) {
  double time = pros::millis();
  double distance_from_wall =
      forward ? dNorth.get() / 25.4 + 4.67 : dSouth.get() / 25.4 + 5.67;
  while ((pros::millis() - time) < timeout) {
  }
}
void test360() {
  chassis.setPose(0, 0, 0);
  chassis.moveToPoint(0, -12, 1000, {.forwards = false});
  chassis.turnToHeading(180, 5000,
                        {.direction = lemlib::AngularDirection::CW_CLOCKWISE,
                         .maxSpeed = 70,
                         .earlyExitRange = .00000000001});
  chassis.turnToHeading(0, 5000,
                        {.direction = lemlib::AngularDirection::CW_CLOCKWISE,
                         .maxSpeed = 100,
                         .earlyExitRange = .00000000001},
                        false);
  chassis.turnToHeading(0, 5000,
                        {.direction = lemlib::AngularDirection::CW_CLOCKWISE,
                         .maxSpeed = 100,
                         .earlyExitRange = .00000000001},
                        false);

  {
    {                                                  // Get sensor readings
      double distNorth = dNorth.get_distance() / 25.4; // Convert to inches
      double distNorthW = dSouth.get_distance() / 25.4;

      // Expected sensor reading at (0,0,0) - when aligned with wall
      double expectedDistance = 9.1;

      // Calculate angle error (in degrees) -  sensors are facing Y axis now
      // If dSouth > dNorth, robot is rotated clockwise, theta error is
      // negative
      double theta_error_rad =
          atan2((distNorthW - distNorth),
                13.75); // Still use horizontal separation for angle
      double theta_error_deg = theta_error_rad * (180 / M_PI);

      // Calculate Y position error (in inches) - sensors facing Y axis
      double averageDistance = (distNorth + distNorthW) / 2.0;
      double y_error = averageDistance - expectedDistance;

      // Get current pose
      lemlib::Pose current_pose = chassis.getPose();

      // Calculate corrected pose - now correcting Y and Theta
      double corrected_theta = current_pose.theta - theta_error_deg;
      double corrected_y = current_pose.y - y_error;
      double corrected_x = current_pose.x; // Keep X unchanged

      // Set corrected pose
      chassis.setPose(corrected_x, corrected_y, corrected_theta);

      std::cout << "dNorth: " << distNorth << " inches, dSouth: " << distNorthW
                << " inches" << std::endl;
      std::cout << "Theta Error: " << theta_error_deg
                << " degrees, Y Error: " << y_error << " inches" << std::endl;
      std::cout << "Corrected Pose: x=" << corrected_x << ", y=" << corrected_y
                << ", theta=" << corrected_theta << std::endl;
    }
  }
}

void waitForColorOrTimeout(Alliance target, int timeout_ms) {
  int start = pros::millis();

  while (pros::millis() - start < timeout_ms) {
    double hue = colorSensor.get_hue();

    // if (getColor(hue) == target) {
    //   break; // color seen → immediately continue
    // }

    pros::delay(10);
  }
}

void left() {
  disengageLeftWing();
  chassis.setPose(49, -10, 240);
  intakeIn();
  chassis.moveToPoint(20, -22, 1000);
  chassis.waitUntil(20);
  engageLoaderMech();
  intakeIn();
  chassis.moveToPoint(22.5, -24, 2000, {.forwards = false});
  chassis.waitUntilDone();
  intakeIn();
  chassis.turnToHeading(135, 1000);
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(12, -12, 1000, {.forwards=false});
  chassis.waitUntilDone();
  intakeMiddle();
  pros::delay(1500);
  intakeOut();
  pros::delay(250);
  intakeIn();
  chassis.moveToPoint(24, -24, 1000, {.minSpeed=90, .earlyExitRange=6});
  chassis.waitUntilDone();
  chassis.turnToHeading(135,250, {.earlyExitRange=.01});
  chassis.waitUntilDone();
  chassis.moveToPoint(48, -48, 2000, {.minSpeed=90, .earlyExitRange=13});
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 750);
  chassis.waitUntilDone();
  engageLoaderMech();
  int distance = (dEast.get() / 25.4) + 5;
  int distanceFromLoader = -48 - (24 - distance);
  chassis.setPose(chassis.getPose().x, distanceFromLoader,
                  chassis.getPose().theta);
  // std::cout << "Distance from east wall: " << distanceFromLoader << " inches"
  //           << std::endl;
  chassis.moveToPoint(70, -48, 700,
                      {.maxSpeed = 60}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(68, -48, 200,
                      {.forwards = false}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(70, -48, 200); // go to match loader
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, -48, 90 );
  chassis.waitUntilDone();
   std::cout << "Current Pose: x=" << chassis.getPose().x
             << ", y=" << chassis.getPose().y
             << ", theta=" << chassis.getPose().theta << std::endl;
  chassis.moveToPoint(
      30, -48, 2000,
      {.forwards = false, .maxSpeed=80}); // go to long goal
  chassis.waitUntil(18.5);
  if (chassis.getPose().theta > 110 || chassis.getPose().theta < 70) {
    chassis.turnToHeading(90, 500);
  }
  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(1600); // score
  std::cout << "Current Pose: x=" << chassis.getPose().x
            << ", y=" << chassis.getPose().y
            << ", theta=" << chassis.getPose().theta << std::endl;
//wing
  chassis.setPose(30, 48, 90);
  intakeIn();
  chassis.moveToPoint(37, 48, 2000, {.minSpeed = 90, .earlyExitRange = 2});
  chassis.waitUntilDone();

  chassis.turnToHeading(55, 300, {.maxSpeed = 80, .earlyExitRange = .01});

  chassis.moveToPoint(
      34, 45, 3000,
      {.forwards = false, .minSpeed = 100, .earlyExitRange = .25});
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000, {.minSpeed = 90, .earlyExitRange = .01});

  chassis.waitUntilDone();

  disengageLeftWing();
  chassis.moveToPoint(0, chassis.getPose().y, 600,
                      {.forwards = false, .minSpeed = 90, .earlyExitRange = 3});
  chassis.waitUntilDone();
  chassis.swingToHeading(60, DriveSide::LEFT, 1000);
  chassis.waitUntilDone();
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
 }
void right() {
  engageLeftWing();
  
  chassis.setPose(47, 10, 270);

  intakeIn();
  chassis.moveToPoint(20, 22, 1000);
  chassis.waitUntil(20);

  engageLoaderMech();
  chassis.cancelAllMotions();

  chassis.turnToHeading(50, 600);
  chassis.waitUntilDone();
  
  chassis.moveToPoint(48, 48, 2000, {.minSpeed = 90, .earlyExitRange = 14.5});
  chassis.waitUntilDone();
  
  chassis.turnToHeading(90, 500);
  chassis.waitUntilDone();
  
  engageLoaderMech();
  int distance = (dWest.get() / 25.4) + 5;
  int distanceFromLoader = 48 + (24 - distance);
  
  chassis.setPose(chassis.getPose().x - 5, distanceFromLoader,
  chassis.getPose().theta);
  
  chassis.moveToPoint(60, 48, 1000, {.minSpeed = 80, .earlyExitRange = 3});
  chassis.waitUntilDone();
  chassis.moveToPoint(66, 48, 1000, {.maxSpeed = 40});
  chassis.waitUntilDone();

  chassis.setPose(chassis.getPose().x, 48, 90);
  chassis.waitUntilDone();
  chassis.moveToPoint(30, 48, 1500, {.forwards = false, .maxSpeed=80});
  chassis.waitUntil(18.5);

  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(3000);
  if (chassis.getPose().theta > 110 || chassis.getPose().theta < 70) {
    chassis.turnToHeading(90, 500);
  }
  // wing
  chassis.setPose(30, 48, 90);
  intakeIn();
  chassis.moveToPoint(37, 48, 2000, {.minSpeed = 90, .earlyExitRange = 2});
  chassis.waitUntilDone();

  chassis.turnToHeading(55, 300, {.maxSpeed = 80, .earlyExitRange = .01});

  chassis.moveToPoint(
      34, 45, 3000,
      {.forwards = false, .minSpeed = 100, .earlyExitRange = .25});
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000, {.minSpeed = 90, .earlyExitRange = .01});

  chassis.waitUntilDone();

  disengageLeftWing();
  chassis.moveToPoint(14, chassis.getPose().y, 3000,
                      {.forwards = false, .minSpeed = 90, .earlyExitRange = 1});
  chassis.waitUntilDone();
  chassis.swingToHeading(60, DriveSide::LEFT, 1000);
  chassis.waitUntilDone();
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

void sawp() {

  intakeIn();
  chassis.moveToPoint(0, -27, 525, {false, 127, 80});
  chassis.waitUntilDone();
  engageLoaderMech();
  chassis.turnToHeading(270, 500); //270
  chassis.waitUntilDone();
  drive_distance(999, 1050, 80);
  chassis.resetLocalPosition();
  move_relative(30, 0, 1500, false, 127);
  chassis.waitUntil(16);
  disengageLoaderMech();
  intakeTopGoal();
  pros::delay(1000);
  intakeIn();
  chassis.setPose(0,0,270);
  //move_relative(-8, 0, 700, true, 115);
  chassis.moveToPoint(-10,0,700, {.forwards=true});
  chassis.waitUntilDone();
  chassis.turnToHeading(30, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(3,24,650, {.forwards=true, .earlyExitRange=1});
  //move_relative(15, 15, 700, true, 115);
  chassis.waitUntilDone();
  chassis.swingToHeading(0, DriveSide::LEFT, 700);
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(chassis.getPose().x, 67, 1200, { .earlyExitRange = 1});
  chassis.waitUntil(22.5);
  engageLoaderMech();
  chassis.turnToHeading(315, 500);
  chassis.waitUntilDone();

  move_relative(15, -15, 500, false, 80);
  chassis.waitUntilDone();
  intakeOut();
  pros::delay(100);
  intakeMiddle();
  pros::delay(500);
  chassis.resetLocalPosition();
  intakeIn();
  
  chassis.moveToPoint(chassis.getPose().x + -36, chassis.getPose().y + 36, 2000,
                      {.forwards=true, .maxSpeed=115, .earlyExitRange = 3});

  chassis.turnToHeading(-90, 500);
  chassis.waitUntilDone();

  drive_distance(999, 1050, 80);
  move_relative(30, 0, 1500, false, 127);
  chassis.waitUntil(19);
  intakeTopGoal();
  pros::delay(3000);
  // chassis.turnToHeading(270, 350, {.earlyExitRange= .01});
  // chassis.waitUntilDone();
  // int distance = (dEast.get() / 25.4) + 5;
  // int distanceFromLoader = -48 - (24 - distance);
  // chassis.setPose(chassis.getPose().x, distanceFromLoader, 270);
  // // std::cout << "Distance from east wall: " << distanceFromLoader << " inches"
  // //           << std::endl;
  // engageLoaderMech();
  // intakeIn();
  // chassis.moveToPoint(chassis.getPose().x - 20, -48, 2000, {.forwards = false});
  // chassis.waitUntilDone();
  // chassis.moveToPoint(chassis.getPose().x + 30, -48, 3000,
  //                     {.forwards = false});
  // chassis.waitUntil(20);
  // intakeTopGoal();
  // pros::delay(1000);
  // drive_distance(999, 1100, 75);
  // intakeIn();
}

void skills() {
  intakeIn();
  engageLeftWing();
  chassis.setPose(0, 0, 180);
  drive_distance_from_wall(24, 5000, 60, false);
  engageLoaderMech();
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  drive_distance(999, 1200, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  drive_distance(-12, 1000, 80);
  disengageLoaderMech();
  chassis.turnToHeading(-135, 1000);
  chassis.waitUntilDone();
  drive_distance(-18, 1000, 80);
  intake.move_velocity(0);
  chassis.turnToHeading(-90, 1000);
  move_relative(72, 0, 5000, false);
  chassis.waitUntilDone();
  chassis.turnToHeading(180, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(22 , 5000, 60, false);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance(-20, 1000, 80);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(2000);
  engageLoaderMech();

  chassis.resetLocalPosition();
  intakeIn();
  chassis.moveToPoint(30, 1, 1500, {true, 65});
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  move_relative(-30, 0, 1500, false);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(2000);
  disengageLoaderMech();
  //REMOVE below
  chassis.setPose(0, 0, 90);

  //remove above
  move_relative(12, 0, 1000, 80);
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  move_relative(0, -80, 6000, false);
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, chassis.getPose().y, 180);
  drive_distance_from_wall(22, 5000, 60, false);
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  engageLoaderMech();

  intakeIn();
  move_relative(-30, 0, 2000, true);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  move_relative(30, 0, 1500, false);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(400);
  disengageLoaderMech();

//3rd goal
  drive_distance(12, 1000, 80);
  disengageLoaderMech();
  chassis.turnToHeading(315, 1000);
  chassis.waitUntilDone();
  drive_distance(-18, 1000, 80);
  chassis.turnToHeading(0, 1000);
  move_relative(0, -72, 5000, false);
  chassis.waitUntilDone();
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, chassis.getPose().y, 180);
  drive_distance_from_wall(22, 5000, 60, false);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance(-20, 1000, 80);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(2000);
  engageLoaderMech();

  chassis.resetLocalPosition();
  
  intakeIn();
  chassis.moveToPoint(30, 1, 1500, {true, 65});
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  move_relative(-30, 0, 1500, false);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(2000);
  disengageLoaderMech();
  chassis.resetLocalPosition();
  // go park
  drive_distance(12, 1000, 80);
  disengageLoaderMech();
  chassis.turnToHeading(55, 1000);
  chassis.waitUntilDone();
  drive_distance(18, 1000, 80);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance(28, 1000, 80);
  chassis.waitUntilDone();
  
}

bool hasAnyInput(pros::Controller master) {
  // Check Analog Sticks (with a deadzone of 10 to ignore drift)
  if (abs(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)) > 10 ||
  abs(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)) > 10 ||
  abs(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) > 10 ||
  abs(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)) > 10) {
    return true;
  }
  
  // No input detected
  return false;
}

pros::Task *macroTask = nullptr;

void wingMacroFunctions() {
      chassis.setPose(30, 48, 90);
      intakeIn();
      chassis.moveToPoint(37, 48, 2000, {.minSpeed = 90, .earlyExitRange = 2});
      chassis.waitUntilDone();

      chassis.turnToHeading(55, 300, {.maxSpeed = 80, .earlyExitRange = .01});

      chassis.moveToPoint(
          34, 45, 3000,
          {.forwards = false, .minSpeed = 100, .earlyExitRange = .25});
      chassis.waitUntilDone();
      chassis.turnToHeading(90,  1000, {.minSpeed=90, .earlyExitRange=.01});

      chassis.waitUntilDone();
   
      disengageLeftWing();
      chassis.moveToPoint(
        -5, chassis.getPose().y, 3000,
        {.forwards = false, .minSpeed = 90, .earlyExitRange = 1});
      pros::delay(500);
      engageLeftWing();
      pros::delay(250);
      disengageLeftWing();
      pros::delay(800);

      chassis.waitUntilDone();
      macroTask = nullptr;
}

void runMacros() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    if (macroTask == nullptr) { // Only start if not already running
      macroTask = new pros::Task(wingMacroFunctions);
    }
  }

  // 2. DETECT INTERRUPT
  if (macroTask != nullptr) { // If macro is currently running
    if (hasAnyInput(controller)) {
      macroTask->remove(); // Kill the task immediately
      delete macroTask;    // Clean up memory
      macroTask = nullptr;

      // IMPORTANT: Stop the motors!
      // Killing the task doesn't stop the last command sent to motors.
      chassis.cancelAllMotions();
      controller.rumble("."); // Feedback that macro was cancelled
    }
  }
}