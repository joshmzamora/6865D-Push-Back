
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void blueRightAuton() {
  // // Initial configuration and starting the intake sequence
  // setIntakeState({IN, STOPPED, IN, STOPPED});

  // // 1. Set the initial pose (was -52, now 52)
  // chassis.setPose(52, 18, -90);

  // // 2. Move toward the first point (was -35, now 35)
  // chassis.moveToPoint(35, 18, 900, {.maxSpeed = 25});
  // chassis.waitUntilDone();

  // chassis.moveToPoint(30, 16, 900, {.maxSpeed = 25});
  // chassis.waitUntilDone();

  // // 3. Turn to face the next target or action area
  // chassis.turnToHeading(-115, 1000);
  // chassis.waitUntilDone();

  // // 4. Move to get the block (was -16, now 16)
  // chassis.moveToPoint(16, 20, 3000, {.maxSpeed = 25}); // get 1 block
  // chassis.waitUntilDone();

  // // 5. Turn toward the low goal
  // chassis.turnToHeading(220, 1000);
  // chassis.waitUntilDone();

  // // 6. Move to the low goal (was -22, now 22)
  // chassis.moveToPose(19, 4, 220,3000, {.maxSpeed = 25}); // go to low goal
  // chassis.waitUntilDone();

  // // 7. Outtake and stop
  // outtakeBasket();
  // pros::delay(3000);
  // stopIntake();
}