#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

void redRightAuton() {
  
  chassis.setPose(-46, -8, 90);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPose(-8, -48, 180, 3000, {.lead=.45});
  chassis.waitUntilDone();

  intakeIn();
  pros::delay(500); // Allow time to secure blocks

  // Move back
  chassis.moveToPoint(-24, -30, 3000, {.forwards = false, .maxSpeed = 70});
  chassis.waitUntilDone();
  //  //  Turn to face the long goal
  chassis.moveToPoint(-53, -48, 3000, {.forwards = false, .maxSpeed = 70});
  chassis.waitUntilDone();

  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();

  //  //  chassis.setPose(48,-48,90);
  //  //  // Approach the long goal to score
  chassis.moveToPoint(-30, -50, 3000, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(-90, 1000);
  chassis.waitUntilDone();
  //  // Score the blocks
  engageLoaderMech();
  intakeTopGoal();
  pros::delay(3000); // Run intakes to score
  intakeIn();
  // // Move to the match loader area
  chassis.moveToPoint(-67, -50, 3000, {.maxSpeed= 60});
  chassis.waitUntilDone();

  pros::delay(2000);
  chassis.moveToPoint(-30, -50, 3000, {.forwards = false});
  chassis.waitUntilDone();

  intakeTopGoal();
}