#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "subsystem/middleGoalMech.h"

void leftAuton() {
  chassis.setPose(46, -15, 270);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, -24, 2000);
  chassis.waitUntilDone();
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(135, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(11, -9, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(750); // score
  intakeIn();
  chassis.moveToPoint(
      50, -50, 2000, {.maxSpeed=90, .earlyExitRange =2}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  //chassis.setPose(50, -50, chassis.getPose().theta);
  pros::delay(250);
  chassis.moveToPoint(59, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(57, -48, 2000, {.forwards=false}); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(59, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(30, -48, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  disengageLoaderMech();
  intakeTopGoal();
  pros::delay(2000); // score
  chassis.moveToPoint(36, -36, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  engageLeftWing();
  chassis.moveToPoint(10, -36, 3000);
  chassis.waitUntilDone();
  chassis.turnToHeading(280, 1000);
  chassis.waitUntilDone();
}