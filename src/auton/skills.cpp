#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
// #include "subsystem/neutral_mech.h"
#include "util/colorsort.h"

ASSET(cornerone_tworings_txt);
ASSET(top_right_goal_txt);
ASSET(bottom_right_goal_txt);

void skillsAuton() {}
// chassis.waitUntilDone();
//  	chassis.turnToHeading(25, 500); //turn to face top right stake
//  	chassis.moveToPose(64, 60, 25,2000); //put top right stake in corner
//  	chassis.waitUntilDone();
//  	chassis.moveToPose(49, -10, 25, 1500, {.forwards=false}); //move to
//  middle: clin this is probably stupid line of code to add but wtv lolsies
//  	chassis.waitUntilDone();
//  	chassis.turnToHeading(325, 750);
//  	chassis.waitUntilDone();
//  	chassis.moveToPose(64, -64, -15, 2000, {.forwards=false}); //bottom
//  right stake in corner 	chassis.waitUntilDone();