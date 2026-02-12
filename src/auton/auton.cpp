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
      dist_from_wall = front_distance ;
      forwards = true;

    } else if (back_valid && !from_front) {
      dist_from_wall = back_distance + 5.25;
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
  pros::delay(1100);
  intakeIn();
  chassis.moveToPoint(24, -24, 2000, {.minSpeed=90, .earlyExitRange=6});
  chassis.waitUntilDone();
  chassis.turnToHeading(135,250);
  chassis.waitUntilDone();
  chassis.moveToPoint(48, -48, 2000, {.minSpeed=90, .earlyExitRange=13});
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 750);
  chassis.waitUntilDone();
  engageLoaderMech();
  int distance = (dEast.get() / 25.4) + 5;
  int distanceFromLoader = -48 - (24 - distance);
  chassis.setPose(chassis.getPose().x, distanceFromLoader,90);
  // std::cout << "Distance from east wall: " << distanceFromLoader << " inches"
  //           << std::endl;
  chassis.moveToPoint(66, -48, 625,
                      {.maxSpeed = 60}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(62, -48, 250,
                      {.forwards = false}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(66, -48, 250); // go to match loader
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, -48, chassis.getPose().theta);
  chassis.waitUntilDone();
   std::cout << "Current Pose: x=" << chassis.getPose().x
             << ", y=" << chassis.getPose().y
             << ", theta=" << chassis.getPose().theta << std::endl;
  chassis.moveToPoint(
      30, -48, 2000,
      {.forwards = false, .maxSpeed=80}); // go to long goal
  chassis.waitUntilDone();
  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(2000); // score
  std::cout << "Current Pose: x=" << chassis.getPose().x
            << ", y=" << chassis.getPose().y
            << ", theta=" << chassis.getPose().theta << std::endl;

  // WING
  // chassis.moveToPoint(45, -48, 2000, {.minSpeed = 70, .earlyExitRange = 2});
  // chassis.waitUntilDone();

  // chassis.turnToHeading(310, 1000);
  // chassis.waitUntilDone();

  // chassis.moveToPoint(30, -39, 2000);
  // chassis.waitUntilDone();

  // disengageLeftWing();
  // chassis.turnToHeading(270, 1000);
  // chassis.waitUntilDone();

  // chassis.moveToPoint(14, -39, 2000);
  // chassis.waitUntilDone();

  // chassis.swingToHeading(300, DriveSide::RIGHT, 1000);
  // chassis.waitUntilDone();
  
  // chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
 }
void right() {
  disengageLeftWing();
  
  chassis.setPose(47, 10, 270);

  intakeIn();
  chassis.moveToPoint(20, 22, 1000);
  chassis.waitUntil(20);

  engageLoaderMech();
  chassis.moveToPoint(24, 24, 2000, {.forwards = false});
  chassis.waitUntilDone();

  chassis.turnToHeading(45, 1000);
  chassis.waitUntilDone();
  
  chassis.moveToPoint(48, 48, 2000, {.minSpeed = 90, .earlyExitRange = 13});
  chassis.waitUntilDone();
  
  chassis.turnToHeading(90, 750);
  chassis.waitUntilDone();
  
  engageLoaderMech();
  int distance = (dWest.get() / 25.4) + 5;
  int distanceFromLoader = 48 + (24 - distance);
  
  chassis.setPose(chassis.getPose().x, distanceFromLoader,
  90);
  
  chassis.moveToPoint(66, 48, 1000, {.maxSpeed = 60});
  chassis.waitUntilDone();

  chassis.setPose(chassis.getPose().x, 48, chassis.getPose().theta);
  chassis.waitUntilDone();
  chassis.moveToPoint(30, 48, 1500, {.forwards = false, .maxSpeed=80});
  chassis.waitUntilDone();

  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(3000);
  // wing
  intakeIn();
  chassis.moveToPoint(48, 48, 2000, {.minSpeed = 70, .earlyExitRange = 2});
  chassis.waitUntilDone();

  chassis.turnToHeading(40, 500, {.maxSpeed=80});
  chassis.waitUntilDone();

  chassis.moveToPoint(30, 40, 3000, {.forwards=false,.maxSpeed = 60, .earlyExitRange = 2});
  chassis.waitUntilDone();

  disengageLeftWing();
  chassis.turnToHeading(90, 1000, {.minSpeed = 50, .earlyExitRange = .01});
  chassis.waitUntilDone();

  chassis.moveToPoint(12, 41, 3000, {.forwards=false,.maxSpeed =40 , .earlyExitRange = 1}); //minSpeed = 90
  chassis.waitUntilDone();
  
  chassis.swingToHeading(50, DriveSide::LEFT, 1000);
  chassis.waitUntilDone();
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

void sawp() {
  chassis.moveToPoint(0, 10, 200, {true, 80, 60});
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(0, -1000, 400, {false, 127, 80});
  chassis.waitUntilDone();
  drive_distance_from_wall(24, 5000, 80, false);
  double y_pos = chassis.getPose().y;
  engageLoaderMech();
  chassis.turnToHeading(-90, 500);
  chassis.waitUntilDone();
  drive_distance(999, 1250, 80);
  move_relative(30, -2, 1500, false, 127);
  chassis.waitUntil(22);
  intakeOut();
  pros::delay(250);
  disengageLoaderMech();
  intakeTopGoal();
  pros::delay(1750);
  intakeIn();
  chassis.turnToHeading(15, 700);
  chassis.turnToHeading(15, 450);
  chassis.waitUntilDone();
  chassis.resetLocalPosition();
  intakeIn();
  chassis.moveToPoint(0, 54, 1500, {true, 100, 80});
  chassis.waitUntilDone();
  intakeIn();
  engageLoaderMech();
  chassis.turnToHeading(20, 800);
  chassis.waitUntilDone();
  intakeIn();
  chassis.turnToHeading(315, 500);
  chassis.waitUntilDone();
  move_relative(16.25, -16.25, 1000, false, 80);
  chassis.waitUntilDone();
  intakeOut();
  pros::delay(100);
  intakeMiddle();
  pros::delay(1000);
  chassis.resetLocalPosition();
  intakeIn();
  move_relative(-33.5, 33.5, 2000, 127);
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  intakeIn();
  // drive_distance(999, 1100, 75);
  move_relative(30, 5, 1500, false);
  chassis.waitUntilDone();
  intakeOut();
  pros::delay(250);
  intakeTopGoal();
  pros::delay(2000);
  disengageLoaderMech();
}
void skills() {
  intake.move_velocity(600);
  disengageLeftWing();
  chassis.moveToPoint(0, 999, 1800, {true, 90, 80});
  chassis.waitUntilDone();
  drive_distance_from_wall(33, 5000, 80);
  chassis.turnToHeading(90, 1000);
  chassis.turnToHeading(90, 500);
  chassis.waitUntilDone();
  drive_distance_from_wall(34, 5000, 80, false);
  chassis.turnToHeading(135, 1000);
  chassis.turnToHeading(135, 500);
  chassis.waitUntilDone();
  move_relative(10, -10, 1500, true, 80);
  chassis.turnToHeading(-45, 500);
  chassis.turnToHeading(-45, 500);
  chassis.waitUntilDone();
  drive_distance(-16.5, 2000, 80);
  pros::delay(1000);
  
  pros::delay(200);
 intakeTopGoal();
  move_relative(-32, 36, 1500, 127);
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(24, 5000, 60);
  engageLoaderMech();
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  intake.move_velocity(600);
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
  drive_distance(-15, 1000, 80);
  intake.move_velocity(0);
  chassis.turnToHeading(-90, 1000);
  move_relative(70, 0, 5000, false);
  chassis.waitUntilDone();
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(24, 5000, 60);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance(-20, 1000, 80);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(400);
  engageLoaderMech();

  chassis.resetLocalPosition();
  chassis.moveToPoint(30, 1, 1500, {true, 65});
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  move_relative(-30, -1, 1500, false);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(400);
  disengageLoaderMech();

  move_relative(20, -20, 1000, 80);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(10, 5000, 80);
  chassis.turnToHeading(170, 1000);
  chassis.waitUntilDone();
  drive_distance(999, 2000, 90);
  chassis.waitUntilDone();
  move_relative(-15, -20, 3000, true, 80);
  chassis.turnToHeading(180, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(25, 5000, 80, true);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  drive_distance(-20, 1000, 80);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(400);
  chassis.resetLocalPosition();
  engageLoaderMech();

  chassis.moveToPoint(30, -1, 1500, {true, 65});
  chassis.waitUntilDone();
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  drive_distance(-999, 100, 65);
  drive_distance(999, 1000, 65);
  chassis.waitUntilDone();
  move_relative(-30, 1, 1500, false);
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(400);
  disengageLoaderMech();

}

void runMacros() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    // wing left
    chassis.setPose(30,48,90);
    intakeIn();
    chassis.moveToPoint(48, 48, 2000, {.minSpeed = 70, .earlyExitRange = 2});
    chassis.waitUntilDone();

    chassis.turnToHeading(40, 500, {.maxSpeed = 80});
    chassis.waitUntilDone();

    chassis.moveToPoint(
        30, 40, 3000, {.forwards = false, .maxSpeed = 60, .earlyExitRange = 2});
    chassis.waitUntilDone();

    disengageLeftWing();
    chassis.turnToHeading(90, 1000, {.minSpeed = 50, .earlyExitRange = .01});
    chassis.waitUntilDone();

    chassis.moveToPoint(12, 41, 3000,
                        {.forwards = false,
                         .maxSpeed = 40,
                         .earlyExitRange = 1}); // minSpeed = 90
    chassis.waitUntilDone();

    chassis.swingToHeading(50, DriveSide::LEFT, 1000);
    chassis.waitUntilDone();
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    // } else if
    // (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    //   //wing right
    //   chassis.setPose(0, 0, 270);
    //   chassis.moveToPoint(-12, -8, 1000);
    //   chassis.turnToPoint(0, -8, 1000);
    //   chassis.waitUntilDone();
    //   engageLeftWing();
    //   chassis.moveToPoint(0, 0, 1000);
    //   chassis.waitUntilDone();
    //   disengageLeftWing();
    }
}