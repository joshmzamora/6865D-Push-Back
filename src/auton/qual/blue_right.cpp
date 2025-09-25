
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void blueRightAuton() {

  chassis.setPose(47, -17, 270);
  setIntakeState({IN, IN, IN, IN});
  chassis.moveToPose(22, -22, 270, 3000); // get 3 blocks
  chassis.waitUntilDone();
//   chassis.moveToPose(8, -38, 0, 3000);
//   chassis.waitUntilDone();
//   chassis.moveToPose(6, -42, 20, 3000); // get 2 blocks
//     chassis.waitUntilDone();
//     chassis.moveToPoint(26, -24, 3000, {.forwards = false});
//     chassis.waitUntilDone();
//     chassis.moveToPose(54, -46, 90, 3000);
//     chassis.waitUntilDone();
//   engageLoaderMech();
//   pros::delay(500);
//     chassis.moveToPose(60, -46, 90, 3000); // go in match loader
//     chassis.waitUntilDone();
//     chassis.moveToPose(46, -46, 90, 3000, {.forwards = false});
//     chassis.waitUntilDone();
//   disengageLoaderMech();
//   chassis.waitUntilDone();
//   chassis.turnToHeading(0, 2000);
//   chassis.waitUntilDone();
//   chassis.moveToPose(26, -46, 0, 3000);
//   chassis.waitUntilDone();
}