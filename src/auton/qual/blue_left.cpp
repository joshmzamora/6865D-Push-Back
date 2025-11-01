
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
void blueLeftAuton() {  
 //Set robot's starting position and state
chassis.setPose(46, -8, -90);
intakeIn();

 // Move to grab the next 2 blocks
 chassis.moveToPose(8, -45,-180, 3000);
 chassis.waitUntilDone();
engageLoaderMech();

 chassis.moveToPoint(8, -48, 3000);
 chassis.waitUntilDone();
 pros::delay(1000); // Allow time to secure blocks
disengageLoaderMech();
 //Move back
 chassis.moveToPoint(24, -30, 3000, {.forwards = false, .maxSpeed = 70});
 chassis.waitUntilDone();

 //  Turn to face the long goal
 chassis.moveToPoint(48, -48, 3000, {.forwards=false, .maxSpeed = 70});

 chassis.waitUntilDone();
 // Approach the long goal to score
 chassis.turnToHeading(90, 1000);
 chassis.waitUntilDone();
 chassis.moveToPoint(30, -48, 3000, {.forwards = false});
 chassis.waitUntilDone();

 // Score the blocks
 engageLoaderMech();
 intakeTopGoal();
 pros::delay(3000); // Run intakes to score

 // // Move to the match loader area
 chassis.moveToPoint(60, -48, 3000);
 chassis.waitUntilDone();

 // Autonomous routine is complete


}