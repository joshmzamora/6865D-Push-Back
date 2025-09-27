#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"

// chassis.setPose(0, 0, 0);

// chassis.moveToPoint(24, 24, 3000);
// chassis.waitUntilDone();
void redRightAuton() {
  intakeBasket();
  chassis.setPose(0, 0, 0);

  chassis.moveToPoint(0, 24, 3000);
  chassis.waitUntilDone();
  // chassis.setPose(-52, -18, 90);
  // chassis.moveToPoint(-31, -18, 900);
  // chassis.waitUntilDone();

  // chassis.moveToPoint(-16, -20, 3000, {.maxSpeed = 25}); //get 1 block
  // chassis.waitUntilDone();
  // chassis.turnToHeading(115, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-13, -21, 3000); //get 2 blocks
  // chassis.waitUntilDone();
  // // pros::delay(500);
  // chassis.turnToHeading(225, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPose(-48, -48, -90, 5000, {.maxSpeed = 40});
  // chassis.waitUntilDone();

  // chassis.turnToHeading(-90, 1000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-57, -48, 5000);
  // chassis.waitUntilDone();
  // engageLoaderMech();
  // chassis.moveToPoint(-57, -48, 5000, {.maxSpeed = 20}); //get match loader blocks
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-48, -48, 5000, {.forwards = false});
  // chassis.waitUntilDone();
  // chassis.turnToHeading(90, 2000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(-31, -48, 5000); //go to long goal
  // chassis.waitUntilDone();
}