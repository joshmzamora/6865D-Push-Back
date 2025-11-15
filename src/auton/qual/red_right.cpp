#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "util/odomLift.h"

void redRightAuton() {
  chassis.setPose(-50, -17, 105);
  intakeIn();
  // // Move to grab the next 2 blocks

  chassis.moveToPoint(-24, -22, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  // chassis.turnToHeading(160, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-24, -22, 160); // reset
  // chassis.waitUntilDone();
  //engageLoaderMech();
  // pros::delay(500); // Allow time to secure blocks
  // disengageLoaderMech();

  // //middle goal
  // chassis.turnToHeading(225, 1000, {.maxSpeed = 60});
  // chassis.waitUntilDone();
  // chassis.setPose(-24, -22, 225); // reset
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-12, -10, 1000, {.forwards = false, .maxSpeed = 60});
  // chassis.waitUntilDone();
  // intakeMiddle();
  // pros::delay(1000); // score low
  // chassis.moveToPoint(-24, -20, 2000, {.maxSpeed = 80});
  // chassis.waitUntilDone();
  // chassis.turnToHeading(160, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-24, -22, 160); // reset
  // chassis.waitUntilDone();
  
  // // Back to elim code
  // intakeIn();
  chassis.moveToPoint(-8, -48, 2000, {.maxSpeed = 80});
  chassis.waitUntilDone();
  pros::delay(500);

  // // // Move back
  // chassis.moveToPoint(-24, -35, 3000, {.forwards = false, .maxSpeed = 80});
  // chassis.waitUntilDone();
  // //  //  Turn to face the long goal
  // chassis.moveToPoint(-48, -48, 3000, {.forwards = false, .maxSpeed = 80});
  // chassis.waitUntilDone();

  // chassis.turnToHeading(-90, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-48,-48,-90); //reset
  // chassis.waitUntilDone();


  // // // Approach the long goal to score
  // engageOdomLift();
  // chassis.moveToPoint(-30, -48, 3000, {.forwards = false, .maxSpeed=80});
  // chassis.waitUntilDone();
 
  // // //  // Score the blocks
  // // engageLoaderMech();
  // intakeTopGoal();
  // pros::delay(3000); // Run intakes to score
  // // intakeIn();
  // // // // Move to the match loader area
  // chassis.moveToPoint(-60, -48, 3000, {.maxSpeed= 60});
  // chassis.waitUntilDone();

  // pros::delay(2000);
  // chassis.moveToPoint(-30, -48, 3000, {.forwards = false});
  // chassis.waitUntilDone();

  // intakeTopGoal();
}