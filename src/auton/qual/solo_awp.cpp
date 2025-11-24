#include "auton/selector.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

void soloAWP() {
  chassis.setPose(-48, -17, 180);
  chassis.moveToPoint(-48, -48, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-48, -48, 270);
  engageLoaderMech();
  chassis.moveToPoint(-62, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  intakeIn();
  pros::delay(2000);
  chassis.moveToPoint(-24, -48, 2000, {.forwards = false}); //go to long goal
  chassis.waitUntilDone();
  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(2000); // score
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
  // chassis.setPose(-29, -41, 0); // FIND THIS POSE
  // intakeIn();
  // chassis.moveToPoint(-24, -24, 1500); //get 3 blocks
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-24, 24, 2000); //get 3 more blocks
  // chassis.waitUntilDone();
  // chassis.turnToHeading(315, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-24, 24, 315);
  // chassis.moveToPoint(12, -12, 3000, {.forwards = false}); // go to long goal
  // chassis.waitUntilDone();
  // intakeMiddle();
  // pros::delay(1500); // score
  // stopIntake();
  // chassis.moveToPoint(-48, 48, 2000);
  // chassis.waitUntilDone();
  // engageLoaderMech();
  // chassis.turnToHeading(270, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-48, 48, 270);
  // chassis.moveToPoint(-62, 48, 2000); // go to match loader
  // chassis.waitUntilDone();
  // intakeIn();
  // pros::delay(2000);
  // chassis.moveToPoint(-24, 48, 2000, {.forwards = false}); //go to long goal
  // chassis.waitUntilDone();
  // intakeTopGoal();
  // disengageLoaderMech();
  // pros::delay(2000); // score
}