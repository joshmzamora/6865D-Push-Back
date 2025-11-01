
#include "auton/selector.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
void blueLeftAuton() {  
 //Set robot's starting position and state
chassis.setPose(47, -8, -90);
intakeIn();
// Move to grab the next 2 blocks
chassis.moveToPose(8, -44,-180, 3000);
chassis.waitUntilDone();
engageLoaderMech();

intakeIn();
 chassis.moveToPoint(8, -46, 3000);
 chassis.waitUntilDone();
 pros::delay(500); // Allow time to secure blocks
disengageLoaderMech();
 //Move back
 chassis.moveToPoint(24, -30, 3000, {.forwards = false, .maxSpeed = 70});
 chassis.waitUntilDone();
//  //  Turn to face the long goal
 chassis.moveToPoint(53, -53, 3000, {.forwards=false, .maxSpeed = 70});
 chassis.waitUntilDone();

 chassis.turnToHeading(90, 1000);
 chassis.waitUntilDone();

//  //  chassis.setPose(48,-48,90);
//  //  // Approach the long goal to score
chassis.moveToPoint(30, -55, 3000, {.forwards = false});
chassis.waitUntilDone();

 //  // Score the blocks
 engageLoaderMech();
 setIntakeState(IN, OUT);
 pros::delay(3000); // Run intakes to score
 intakeIn();
// // Move to the match loader area
 chassis.moveToPoint(55, -57, 3000);
 chassis.waitUntilDone();
 
 pros::delay(2000);
 chassis.moveToPoint(30, -57, 3000, {.forwards = false});
 chassis.waitUntilDone();
 
 setIntakeState(IN, OUT);
 // Autonomous routine is complete


}