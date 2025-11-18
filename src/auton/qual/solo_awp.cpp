
#include "auton/selector.h"
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
void soloAWP() {  
 //Set robot's starting position and state
chassis.setPose(47, -8, -90);
intakeIn();
// Move to grab the next 2 blocks
chassis.moveToPose(9, -44,-180, 3000);
chassis.waitUntilDone();
intakeIn();
 chassis.moveToPoint(9, -48, 3000);
 chassis.waitUntilDone();
 pros::delay(500); // Allow time to secure blocks

 //Move back
 chassis.moveToPoint(24, -30, 3000, {.forwards = false});
 chassis.waitUntilDone();
//  //  Turn to face the long goal
 chassis.moveToPoint(53, -48, 3000, {.forwards=false});
 chassis.waitUntilDone();

//  chassis.turnToHeading(90, 1000);
//  chassis.waitUntilDone();

// //  //  chassis.setPose(48,-48,90);
// //  //  // Approach the long goal to score
// chassis.moveToPoint(30, -48, 3000, {.forwards = false});
// chassis.waitUntilDone();

//  //  // Score the blocks
//  engageLoaderMech();
//  intakeTopGoal();
//  pros::delay(4000); // Run intakes to score
//  intakeIn();
// // // Move to the match loader area
//  chassis.moveToPoint(55, -48, 3000);
//  chassis.waitUntilDone();
 
//  pros::delay(2000);
//  chassis.moveToPoint(30, -48, 3000, {.forwards = false});
//  chassis.waitUntilDone();
 
// intakeTopGoal();
 // Autonomous routine is complete


}