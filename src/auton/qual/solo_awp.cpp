
#include "auton/selector.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "util/odomLift.h"

void soloAWP() {
  chassis.setPose(47, -17, 255);
  // chassis.setPose(-46, 0, 270);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, -24, 2000);
  chassis.waitUntilDone();
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(220, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, -24, 220); // reset
  chassis.waitUntilDone();

// //   // // Back to elim code
  // intakeIn();
  chassis.moveToPoint(11, -44, 2000);
  chassis.waitUntilDone();
//   pros::delay(250);
  chassis.moveToPoint(24, -24, 2000, {.forwards = false});
  chassis.waitUntilDone();
  chassis.turnToHeading(135, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, -24, 135); // reset should be 135
  chassis.waitUntilDone();
  chassis.moveToPoint(7, -11, 2000, {.forwards = false});
  chassis.waitUntilDone(); 

  // chassis.moveToPoint(48, -48, 3000);
  // chassis.waitUntilDone();
  // engageLoaderMech();
  // chassis.moveToPoint(60, -48, 2000);
  // chassis.waitUntilDone();
//   // chassis.moveToPose(48, -48, 225, 3000, {.forwards = false, .maxSpeed=80});
//   // // Note: This line was commented out in the original. The values have been
//   // transformed. chassis.waitUntilDone();

//   chassis.turnToHeading(85, 1000);
//   chassis.waitUntilDone();
//   chassis.setPose(48, -48, 90); // reset
//   chassis.waitUntilDone();

//   // // // Approach the long goal to score
//   engageOdomLift();

//   intakeIn();
  // chassis.moveToPoint(28, -49, 2000, {.forwards = false});
  // chassis.waitUntilDone();
//   disengageOdomLift();
//   // //  // Score the blocks
//   engageLoaderMech();
//   intakeTopGoal();
//   pros::delay(2500); // Run intakes to score
//   // // // Move to the match loader area
//   chassis.moveToPoint(67, -46, 3000);
//   chassis.waitUntilDone();
//   intakeIn();
//   pros::delay(1500);
//   chassis.setPose(60, -46, 90);
//   chassis.waitUntilDone();
//   chassis.moveToPoint(30, -48, 3000, {.forwards = false});
//   chassis.waitUntilDone();

//   intakeTopGoal();
//   pros::delay(3000);
}