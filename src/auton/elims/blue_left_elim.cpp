#include "auton/auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "util/odomLift.h"
#include "subsystem/middleGoalMech.h"

void blueLeftElimAuton() {
  chassis.setPose(50, -17, 255);
  intakeIn();
  // Move to grab the next 2 blocks
  chassis.moveToPoint(22, -22, 2000, {.maxSpeed = 80});
  chassis.waitUntil(21);
  engageLoaderMech();
  pros::delay(500); // Allow time to secure blocks
  disengageLoaderMech();
  chassis.turnToHeading(143, 500);
  chassis.waitUntilDone();
  chassis.setPose(22, -22, chassis.getPose().theta);
  chassis.moveToPoint(11, -9, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(500); // score
  intakeIn();
  chassis.moveToPoint(
      48, -48, 2000, {.maxSpeed=90, .earlyExitRange =2}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  jamIntake();
  chassis.setPose(48, -48, 90);
  engageLoaderMech();
  pros::delay(250);
  chassis.moveToPoint(66, -48, 2000, {.maxSpeed = 80}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(24, chassis.getPose().y-3, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  disengageLoaderMech();
  jamIntake();
  intakeTopGoal();
  pros::delay(2000); // score
  chassis.moveToPoint(48, -48, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(45, 1000);
  chassis.waitUntilDone();
  engageLeftWing();
  chassis.moveToPoint(24, -60, 3000, {.forwards=false, .maxSpeed=80});
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  chassis.setPose(24, -60, 90);
  disengageLeftWing();
  chassis.moveToPoint(12,-60,2000,{.forwards=false,.maxSpeed=80});
  chassis.waitUntilDone();
  chassis.turnToHeading(80, 1000);
  chassis.waitUntilDone();
}