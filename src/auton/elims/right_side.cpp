#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

void rightAuton() {
  chassis.setPose(49, 14, 285);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, 24, 2000, {.maxSpeed = 80});
  chassis.waitUntil(21);
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(45, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, 24, chassis.getPose().theta); // reset
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(48, 48, 3000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(92, 1000);
  chassis.waitUntilDone();
  chassis.setPose(48, 48, 90); // reset
  chassis.waitUntilDone();
  // //  // Score the blocks
  engageLoaderMech();
  intakeIn();
  // // // Move to the match loader area
  chassis.moveToPoint(61, 47, 2000, {.maxSpeed = 65});
  chassis.waitUntilDone();
  pros::delay(350);
  chassis.moveToPoint(59, 47, 2000, {.forwards = false, .maxSpeed = 65});
  chassis.waitUntilDone();
  pros::delay(350);
  chassis.moveToPoint(61, 47, 2000, {.maxSpeed = 65});
  chassis.waitUntilDone();
  pros::delay(350);
  chassis.moveToPoint(26, chassis.getPose().y + 3, 2000, {.forwards = false});
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(1500); // score
  jamIntake();
  intakeTopGoal();
  pros::delay(1500); // score
  disengageLoaderMech();
  intakeIn();
  chassis.moveToPoint(48, 48, 1000);
  chassis.waitUntilDone();
  chassis.turnToHeading(45, 1000);
  chassis.waitUntilDone();
  chassis.setPose(48, 48, 45);
  engageLeftWing();
  chassis.moveToPoint(24, 35, 2000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(92.5, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, 35, 90);
  disengageLeftWing();
  chassis.moveToPoint(12, 38, 1000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(70, 1000);
  chassis.waitUntilDone();
}