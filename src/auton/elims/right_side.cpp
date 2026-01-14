#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

void rightAuton() {
  chassis.setPose(47, 5, 270);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, 24, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  engageLoaderMech();
  pros::delay(500);
  disengageLoaderMech();
  chassis.turnToHeading(45, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(
      48, 48, 2000,
      {.maxSpeed = 80, .earlyExitRange = 8}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  // chassis.setPose(50, -50, chassis.getPose().theta);
  pros::delay(250);
  engageLoaderMech();
  chassis.moveToPoint(65, 48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(63, 48, 2000, {.forwards = false}); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(65, 48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(30, 48, 1000,
                      {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  disengageLoaderMech();
  intakeTopGoal();
  pros::delay(3000); // score
}