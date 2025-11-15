#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "util/odomLift.h"


void redLeftElimAuton() {
  chassis.setPose(-50, 17, 75);
  intakeIn();
  // // Move to grab the next 2 blocks

  chassis.moveToPoint(-24, 22, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  engageLoaderMech();
  engageLeftWing();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();

  chassis.turnToHeading(35, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-24, 22, 35); // reset
  chassis.waitUntilDone();

  // // Back to elim code
  intakeIn();
  chassis.moveToPose(-8, 48, -170, 2500, {.maxSpeed = 80});
  chassis.waitUntilDone();
  // // pros::delay(500);

  // // // Move back
  chassis.moveToPoint(-24, 35, 3000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.moveToPoint(-48, 48, 3000, {.forwards = false, .maxSpeed = 80});
  chassis.waitUntilDone();

  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-48, 48, 270); // reset
  chassis.waitUntilDone();

  // // // Approach the long goal to score
  engageOdomLift();
  chassis.moveToPoint(-30, 48, 3000, {.forwards = false, .maxSpeed=80});
  chassis.waitUntilDone();

  // //  // Score the blocks
  // engageLoaderMech();
  intakeTopGoal();
  pros::delay(3000); // Run intakes to score
  // intakeIn();
  // // // Move to the match loader area
  chassis.moveToPoint(-60, 48, 3000, {.maxSpeed= 60});
  chassis.waitUntilDone();

  pros::delay(2000);
  chassis.moveToPoint(-30, 48, 3000, {.forwards = false});
  chassis.waitUntilDone();

  intakeTopGoal();
}