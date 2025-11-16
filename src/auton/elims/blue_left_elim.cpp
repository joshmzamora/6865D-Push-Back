#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "util/odomLift.h"


void blueLeftElimAuton() {
  chassis.setPose(50, -17, 255);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, -22, 2000, {.maxSpeed = 80});
  chassis.waitUntil(21);
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(245, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, -24, 245); // reset
  chassis.waitUntilDone();

  // // Back to elim code
  intakeIn();
  chassis.moveToPoint(20, -52, 2000, {.maxSpeed = 80});
  chassis.waitUntil(21);
  pros::delay(250);

  chassis.moveToPoint(30, -35, 3000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.moveToPoint(48, -44, 3000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();
  // chassis.moveToPose(48, -48, 225, 3000, {.forwards = false, .maxSpeed=80});
  // // Note: This line was commented out in the original. The values have been
  // transformed. chassis.waitUntilDone();

  chassis.turnToHeading(85, 1000);
  chassis.waitUntilDone();
  chassis.setPose(48, -48, 90); // reset
  chassis.waitUntilDone();

  // // // Approach the long goal to score
  engageOdomLift();

  intakeIn();
  chassis.moveToPoint(28, -49, 2000, {.forwards = false, .maxSpeed = 70});
  chassis.waitUntilDone();
  disengageOdomLift();
  // //  // Score the blocks
  engageLoaderMech();
  intakeTopGoal();
  pros::delay(2500); // Run intakes to score
  // // // Move to the match loader area
  chassis.moveToPoint(67, -46, 3000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  intakeIn();
  pros::delay(1500);
  chassis.setPose(60, -46, 90);
  chassis.waitUntilDone();
  chassis.moveToPoint(30, -48, 3000, {.forwards = false});
  chassis.waitUntilDone();

  intakeTopGoal();
  pros::delay(3000);
}