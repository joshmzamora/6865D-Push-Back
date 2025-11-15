
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
void redLeftAuton() {
  chassis.setPose(-50, 17, 75);
  intakeIn();
  // // Move to grab the next 2 blocks

  chassis.moveToPoint(-24, 22, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  engageLoaderMech();
  engageLeftWing();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();

  chassis.turnToHeading(310, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-24, 22, 310); // reset
  chassis.waitUntilDone();


  // middle goal
  chassis.waitUntilDone();
  chassis.moveToPoint(-15, 15, 1000, {.forwards = false, .maxSpeed = 60});
  chassis.waitUntilDone();
  intakeMiddle();
  pros::delay(1000); // score mid
  chassis.moveToPoint(-24, 22, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(25, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-24, 22, 25); // reset
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
  chassis.setPose(-48, 48, 270); //reset
  chassis.waitUntilDone();

  // // // Approach the long goal to score
  // // engageOdomLift();
  // // chassis.moveToPoint(-30, 48, 3000, {.forwards = false, .maxSpeed=80});
  // // chassis.waitUntilDone();

  // // // //  // Score the blocks
  // // // engageLoaderMech();
  // // intakeTopGoal();
  // // pros::delay(3000); // Run intakes to score
  // // // intakeIn();
  // // // // // Move to the match loader area
  // // chassis.moveToPoint(-60, 48, 3000, {.maxSpeed= 60});
  // // chassis.waitUntilDone();

  // // pros::delay(2000);
  // // chassis.moveToPoint(-30, 48, 3000, {.forwards = false});
  // // chassis.waitUntilDone();

  // // intakeTopGoal();
}