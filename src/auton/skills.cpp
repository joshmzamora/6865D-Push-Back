#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "util/colorsort.h"
#include "util/odomLift.h"
#include "subsystem/middleGoalMech.h"

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
pros::Gps gps(PORT_GPS, 1.0, 1.0, true);

void skillsAuton() {
  //gps.set_position(-1.21, -.4, 180);
  engageLeftWing();
  chassis.setPose(-48, -16, 180);
  chassis.moveToPoint(-48, -48, 1000, {.maxSpeed = 80}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  chassis.setPose(-48, -48, 270);
  engageLoaderMech();
  pros::delay(250);
  intakeIn();
  chassis.moveToPoint(-62, -48, 2500, {.maxSpeed = 80}); // go to match loader
  chassis.waitUntilDone();
  pros::delay(2500);
  intakeIn();
  chassis.moveToPoint(-59, -48, 1000, {.forwards = false});
  chassis.waitUntilDone();
  chassis.moveToPoint(-62, -48, 2500, {.maxSpeed = 80}); // go back to match loader
  chassis.waitUntilDone();
pros::delay(2500);
  chassis.moveToPoint(-26, -50, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(2500); // score
  jamIntake();
  intakeTopGoal();
  pros::delay(2500); // score

  //REGULAR SKILLS
  // chassis.moveToPoint(-48, -48, 1000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(180, 1000);
  // chassis.setPose(-48, -48, chassis.getPose().theta);
  // chassis.moveToPoint(-48, -62, 1000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 1000);
  // chassis.setPose(-48, -62, chassis.getPose().theta);
  // chassis.moveToPoint(48, -62, 5000, {.maxSpeed = 80});
  // chassis.waitUntilDone();
  // chassis.turnToHeading(0, 1000);
  // chassis.setPose(48, -62, chassis.getPose().theta);
  // chassis.moveToPoint(48, -48, 1000);
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 1000);
  // chassis.waitUntilDone();
  // chassis.setPose(48, -48, chassis.getPose().theta);
  // engageLoaderMech();
  // pros::delay(250);
  // intakeIn();
  // chassis.moveToPoint(60, -48, 2000, {.maxSpeed = 80});
  // chassis.waitUntilDone();

  //solo awp
  chassis.turnToHeading(0, 3000, {.minSpeed = 100});
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPose(-26, -24, 0, 2000, {.lead = .05, .maxSpeed = 30});
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPose(chassis.getPose().x, 20, 0, 10000, {.maxSpeed = 30});
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x, 26, 3000, {.maxSpeed = 30});
  chassis.waitUntilDone();
  chassis.turnToHeading(315, 500);
  chassis.waitUntilDone();
  chassis.setPose(-23, 26, 315);
  chassis.moveToPoint(-9, 15, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(5000); // score
  intakeIn();
  chassis.moveToPoint(
      -48, 48, 2000,
      {.minSpeed = 90, .earlyExitRange = 8}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  jamIntake();
  chassis.setPose(-48, 48, 270);
  engageLoaderMech();
  pros::delay(250);
  chassis.moveToPoint(-66, 48, 2000, {.maxSpeed = 80}); // go to match loader
  chassis.waitUntilDone();
  pros::delay(5000);
  // chassis.moveToPoint(-64, 48, 1000, {.forwards = false});
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-66, 48, 2500,
  //                     {.maxSpeed = 80}); // go back to match loader
  // chassis.waitUntilDone();
  // pros::delay(2500);
  chassis.moveToPoint(-24, 48, 1000, {.forwards = false}); // go to long goal
  chassis.waitUntilDone();
  disengageLoaderMech();
  jamIntake();
  intakeTopGoal();
  pros::delay(5000); // score
  chassis.setPose(-30, 48, chassis.getPose().theta);
  chassis.moveToPoint(-48, 48, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(225, 1000);
  chassis.waitUntilDone();
  chassis.setPose(-48, 48, 225);
  chassis.moveToPoint(-60, 20, 4000); // park on platform
  chassis.waitUntilDone();
  engageOdomLift();
  intakeIn();
  chassis.turnToHeading(180, 1000);
  chassis.waitUntilDone();
  chassis.moveToPoint(-60, 0, 2000);
  chassis.waitUntilDone();
}