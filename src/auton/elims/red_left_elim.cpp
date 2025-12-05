#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "util/odomLift.h"
#include "subsystem/middleGoalMech.h"


void redLeftElimAuton() {
  chassis.setPose(50, -17, 255);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(24, -22, 2000, {.maxSpeed = 80});
  chassis.waitUntil(21);
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(315, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, -24, 315); // reset
  chassis.waitUntilDone();
  chassis.moveToPoint(12, -17, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(500); // score
  intakeIn();
  chassis.moveToPoint(
      48, -48, 2000,
      {.minSpeed = 90, .earlyExitRange = 8}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  jamIntake();
  chassis.setPose(48, -48, 270);
  engageLoaderMech();
  pros::delay(250);
  chassis.moveToPoint(66, -48, 2000, {.maxSpeed = 80}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(24, -48, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  jamIntake();
  intakeTopGoal();
  pros::delay(2000); // score
}