#include "lemlib/api.hpp"
#include "main.h"
#include "globals.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

pros::Distance dNorth('A');
pros::Distance dSouth('B');
pros::Distance dEast('C');
pros::Distance dWest('D');

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
      dist_from_wall = front_distance + 4.67;
      forwards = true;

    } else if (back_valid && !from_front) {
      dist_from_wall = back_distance + 5.67;
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
 
void left() {
  drive_distance_from_wall(25, 5000, 80);
  chassis.turnToHeading(90, 1000);
  double y_pos = chassis.getPose().y;
  engageLoaderMech();
  intake.move_velocity(600);
  chassis.waitUntilDone();
  drive_distance(999, 1100, 80);
  chassis.moveToPoint(-30, y_pos + 3, 1500, {false, 90});
  chassis.waitUntilDone();
  intakeTopGoal();
  chassis.turnToHeading(90, 1000);
  pros::delay(100);
  chassis.moveToPoint(chassis.getPose().x + 5, y_pos + 4, 2000, {false, 90});
  disengageLoaderMech();
  intake.move_velocity(600);
  chassis.turnToHeading(200, 1000);
  chassis.waitUntilDone();
  drive_distance(25, 2000, 65);
  pros::delay(700);
  chassis.turnToHeading(220, 1000);
  chassis.waitUntilDone();
  pros::delay(20);
  drive_distance(15, 500, 67);
  chassis.waitUntilDone();
  intake.move_velocity(-300);
  pros::delay(1500);
  chassis.turnToHeading(210, 1000);
  chassis.waitUntilDone();
  pros::delay(20);
  drive_distance(-33, 2000, 80);
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  drive_distance(25, 2000, 127);
  chassis.turnToHeading(-110, 5000);
}
void right() {
  drive_distance_from_wall(26, 5000, 80);
  chassis.turnToHeading(-90, 1000);
  double y_pos = chassis.getPose().y;
  engageLoaderMech();
  intake.move_velocity(600);
  chassis.waitUntilDone();
  drive_distance(999, 1100, 65);
  chassis.moveToPoint(30, y_pos - 3, 1500, {false, 90});
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(200);
  drive_distance(5, 1412, 67);
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  drive_distance_from_wall(35, 1412, 67);
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  drive_distance(-28, 15000, 67);
  chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

void sawp() {
  chassis.moveToPoint(0, 10, 200, {true, 80, 60});
  intake.move_velocity(600);
  chassis.moveToPoint(0, -1000, 600, {false, 127, 80});
  drive_distance_from_wall(24, 5000, 80, false);
  double y_pos = chassis.getPose().y;
  engageLoaderMech();
  intake.move_velocity(600);
  chassis.turnToHeading(-90, 500);
  chassis.waitUntilDone();
  drive_distance(999, 1100, 80);
  move_relative(30, -4, 1500, false, 127);
  chassis.waitUntil(22);
  intakeIn();
  disengageLoaderMech();
  intakeTopGoal();
  intake.move_velocity(600);
  chassis.turnToHeading(15, 700);
  chassis.turnToHeading(15, 500);

  chassis.waitUntilDone();
  chassis.resetLocalPosition();
  chassis.moveToPoint(0, 50, 1500, {true, 80, 100});
  chassis.waitUntilDone();
  engageLoaderMech();
  chassis.turnToHeading(20, 800);
  chassis.waitUntilDone();
  drive_distance_from_wall(50, 5000, 80);
  chassis.turnToHeading(-40, 1000);
  move_relative(10, -13, 1000, false, 80);
  pros::delay(300);
  
  pros::delay(200);
  intakeTopGoal();
  chassis.resetLocalPosition();
  move_relative(-32, 37, 2000, 127);

  
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  intake.move_velocity(600);
  drive_distance(999, 1100, 75);
  move_relative(30, -3, 1500, false);
  chassis.waitUntil(22);
  intakeIn();
  disengageLoaderMech();
  intakeTopGoal();
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