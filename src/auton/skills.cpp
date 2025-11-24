#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "util/colorsort.h"



void skillsAuton() {
  chassis.setPose(-48, -16, 180);
  chassis.moveToPoint(-48, -48, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-48, -48, 270);
  chassis.moveToPoint(-62, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  intakeIn();
  pros::delay(2000);
  chassis.moveToPoint(-48, -48, 2000, {.forwards = false});
  chassis.waitUntilDone();
  // chassis.turnToHeading(135, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-48, -48, 135);
  // chassis.moveToPoint(-36, -60, 2000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(-36, -60, 90);
  // chassis.moveToPoint(36, -60, 4000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(45, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(48, -48, 2000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(28, -48, 2000, {.forwards=false}); // go to long goal
  // chassis.waitUntilDone();
  // intakeTopGoal();
  // pros::delay(3000); // score
  // chassis.moveToPoint(62, -48, 2000); // go to match loader
  // chassis.waitUntilDone();
  // intakeIn();
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