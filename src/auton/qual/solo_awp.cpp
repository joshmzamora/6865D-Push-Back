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
  chassis.moveToPoint(-56, -48, 2000); 
  chassis.waitUntilDone();
  chassis.moveToPoint(-60, -48, 500, {.maxSpeed=20}); // go to match loader
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
  chassis.moveToPose(-24, 20, 2000, 0, {.maxSpeed=80}); 
  chassis.waitUntilDone();
  chassis.moveToPoint(-24, 26, 2000,
                      {.maxSpeed = 80});
  chassis.waitUntilDone();
  chassis.turnToHeading(315, 500);
  chassis.waitUntilDone();
  chassis.setPose(chassis.getPose().x, chassis.getPose().y, 315);
  chassis.moveToPoint(-10, 12, 3000, {.forwards = false}); // go to middle goal
  chassis.waitUntilDone();
  setIntakeState(IN, IN);
  engageMiddleGoalMech();
  pros::delay(500); // score
  intakeIn();
  chassis.moveToPoint(-48, 48, 2000, {.minSpeed = 90, .earlyExitRange=14}); // go to match loader
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 500);
  chassis.waitUntilDone();
  jamIntake();
  chassis.setPose(chassis.getPose().x, chassis.getPose().y, 270);
  engageLoaderMech();
  pros::delay(250);
  chassis.moveToPoint(-62, 48, 2000, {.maxSpeed=100}); // go to match loader
  chassis.waitUntilDone();
  chassis.moveToPoint(-24, 48, 1000, {.forwards = false, .minSpeed=100}); //go to long goal
  chassis.waitUntilDone();
  intakeTopGoal();
  pros::delay(1000); // score
  jamIntake();
  intakeTopGoal();
  pros::delay(1000); // score
}