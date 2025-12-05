
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void moveForward() {
  //move forward
  // chassis.setPose(0, 0, 180);
  // chassis.moveToPoint(0, -1, 2000);
  // chassis.waitUntilDone();

  //test
  chassis.setPose(0, 0, 0);
  chassis.turnToHeading(90, 1000);
  chassis.waitUntilDone();
  chassis.turnToHeading(180, 1000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 1000);
  chassis.waitUntilDone();
  chassis.turnToHeading(0, 1000);
  chassis.waitUntilDone();
}