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
  chassis.setPose(46, -12, 255);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, -24, 2000, {.maxSpeed=80});
  chassis.waitUntil(22);
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(135, 500);
  chassis.waitUntilDone();
  chassis.moveToPoint(11, -11, 1000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(500); // score
  intakeIn();
  chassis.moveToPoint(
      48, -48, 2000, {.maxSpeed=80, .earlyExitRange =23}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  //chassis.setPose(50, -50, chassis.getPose().theta);
  pros::delay(250);
  engageLoaderMech();
  chassis.moveToPoint(65, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(63, -48, 2000, {.forwards=false}); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(65, -48, 2000); // go to match loader
  chassis.waitUntilDone();
  pros::delay(125);
  chassis.moveToPoint(30, chassis.getPose().y, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  disengageLoaderMech();
  intakeTopGoal();
  pros::delay(2000); // score
  chassis.moveToPoint(36, -42, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  engageLeftWing();
  chassis.moveToPoint(10, -42, 3000);
  chassis.waitUntilDone();
}