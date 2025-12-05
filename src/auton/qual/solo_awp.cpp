#include "auton/selector.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/gps.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/middleGoalMech.h"
//pros::Gps gps(PORT_GPS, 1.0,  1.0, true);

// gps.set_position(-1.5, -1.14, 180);

void soloAWP() {
  engageLeftWing();
  chassis.setPose(-48, -16, 180);
  chassis.moveToPoint(-48, -48, 1000, {.maxSpeed=80}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  chassis.setPose(-48, -48, 270);
  engageLoaderMech();
  pros::delay(250);
  intakeIn();
  chassis.moveToPoint(-60, -48, 2500, {.maxSpeed=80}); // go to match loader
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPoint(-26, -50, 1000, {.forwards = false}); //go to long goal
  chassis.waitUntilDone();
  intakeTopGoal();
  disengageLoaderMech();
  pros::delay(1000); // score
  chassis.turnToHeading(0,3000, {.minSpeed=100});
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPose(-26, -24, 0, 750, {.lead=.05,.maxSpeed=80}); 
  chassis.waitUntilDone();
  intakeIn();
  chassis.moveToPose(chassis.getPose().x, 20, 2000, 0, {.maxSpeed=80}); 
  chassis.waitUntilDone();
  chassis.moveToPoint(chassis.getPose().x, 26, 2000,
                      {.maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(315, 500);
  chassis.waitUntilDone();
  chassis.setPose(-23, 26, 315);
  chassis.moveToPoint(-12, 17, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(500); // score
  intakeIn();
  chassis.moveToPoint(-48, 48, 2000, {.minSpeed = 90, .earlyExitRange=8}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  jamIntake();
  chassis.setPose(-48, 48, 270);
  engageLoaderMech();
  pros::delay(250);
  chassis.moveToPoint(-66, 48, 2000, {.maxSpeed=80}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(-24, 48, 1000, {.forwards = false}); //go to long goal
  chassis.waitUntilDone();
  jamIntake();
  intakeTopGoal();
  pros::delay(2000); // score
}