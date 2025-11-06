
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void redLeftAuton() {
  // Set robot's starting position and state
  chassis.setPose(-47, 8, 90);
  intakeIn();
  chassis.moveToPoint(-13, 17, 2000, {.maxSpeed = 60});
  chassis.waitUntilDone();
  // Move to grab the next 2 blocks
  chassis.moveToPose(-12, 40, 3000, -180, {.maxSpeed = 60});
  chassis.waitUntilDone();

  intakeIn(); 
  chassis.moveToPoint(-11, 42, 3000, {.maxSpeed = 60});
  chassis.waitUntilDone();
  pros::delay(500); // Allow time to secure blocks
  //engageLoaderMech();
  // Move back
  chassis.moveToPoint(-24, 30, 3000, {.forwards = false, .maxSpeed = 73});
  chassis.waitUntilDone();
  //disengageLoaderMech();
  //  //  Turn to face the long goal
  chassis.moveToPoint(-53, 50, 3000, {.forwards = false, .maxSpeed = 73});
  chassis.waitUntilDone();

  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();

  //  //  chassis.setPose(48,-48,90);
  //  //  // Approach the long goal to score
  chassis.moveToPoint(-30, 50, 3000, {.forwards = false});
  chassis.waitUntilDone();

  //  // Score the blocks
  engageLoaderMech();
  intakeTopGoal();
  pros::delay(4000); // Run intakes to score
  intakeIn();
  // // Move to the match loader area
  chassis.moveToPoint(-55, 54, 3000);
  chassis.waitUntilDone();

  pros::delay(2000);
  chassis.moveToPoint(-30, 57, 3000, {.forwards = false, .maxSpeed = 50});
  chassis.waitUntilDone();

  intakeTopGoal();
  // Autonomous routine is complete
}