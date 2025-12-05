#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "util/colorsort.h"

//WING CODE
// chassis.setPose(-48, -16, 180);
// chassis.moveToPoint(-48, -48, 2000);
// chassis.waitUntilDone();
// chassis.turnToHeading(270, 1000);
// chassis.waitUntilDone();
// chassis.setPose(-48, -48, 270);
// engageLoaderMech();
// chassis.moveToPoint(-70, -48, 2000, {.maxSpeed = 30}); // go to match loader
// chassis.waitUntilDone();
// intakeIn();
// pros::delay(2000);
// stopIntake();
// chassis.moveToPoint(-50, -48, 2000, {.forwards = false});
// chassis.waitUntilDone();
// disengageLoaderMech();
// chassis.turnToHeading(125, 1000);
// chassis.waitUntilDone();
// chassis.setPose(-50, -48, 125);
// chassis.moveToPoint(-30, -65, 2000);
// chassis.waitUntilDone();
// chassis.turnToHeading(90, 1000);
// chassis.waitUntilDone();
// chassis.setPose(-30, -65, 90);
// chassis.moveToPoint(36, -65, 4000);
// chassis.waitUntilDone();
// chassis.turnToHeading(45, 1000);
// chassis.waitUntilDone();

void skillsAuton() {
  engageLeftWing();
  chassis.setPose(-48, -17, 180);
  chassis.moveToPoint(-48, -48, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-48, -48, 270);
  engageLoaderMech();
  pros::delay(500);
  intakeIn();
  chassis.moveToPoint(-70, -48, 2500, {.maxSpeed=30}); // go to match loader
  chassis.waitUntilDone();
  // pros::delay(2000);
  // stopIntake();
  chassis.moveToPoint(-50, -48, 2000, {.forwards = false});
  chassis.waitUntilDone();
  disengageLoaderMech();
  chassis.turnToHeading(125,  1000);
  chassis.waitUntilDone();
  chassis.setPose(-50, -48, 125);
  chassis.moveToPose(-20, -59, 90, 2000);
  chassis.waitUntilDone();
  // chassis.setPose(-33, -58, 90);
  chassis.moveToPoint(36, -58, 4000);
  chassis.waitUntilDone();
  // chassis.turnToHeading(45, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(36, -58, 45);
  // chassis.moveToPoint(48, -48, 2000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(48, -48, 90);
  // chassis.moveToPoint(28, -48, 2000, {.forwards=false}); // go to long goal
  // chassis.waitUntilDone();
  // intakeTopGoal();
  // pros::delay(3000); // score
  // chassis.setPose(28, -48, 90);
  // intakeIn();
  // chassis.moveToPoint(75, -48, 3000); // go to match loader
  // chassis.waitUntilDone();
  // pros::delay(2000);
  // chassis.moveToPoint(28, -48, 2000, {.forwards=false}); // go to long goal
  // chassis.waitUntilDone();
  // intakeTopGoal();
  // pros::delay(3000); // score
  // chassis.moveToPoint(48, -48, 2000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(315, 1000);
  // chassis.waitUntilDone();
  // intakeIn();
  // chassis.setPose(48, -48, 315);
  // chassis.moveToPoint(24, -24, 2000); // get 3 blocks
  // chassis.waitUntilDone();
  // chassis.turnToHeading(270, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(24, -24, 270);
  // chassis.moveToPoint(-24, -24, 2500);
  // chassis.waitUntilDone();
}