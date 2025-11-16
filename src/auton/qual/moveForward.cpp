
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void moveForward() {
  chassis.setPose(0, 0, 180);
  //chassis.waitUntilDone();
  chassis.moveToPoint(0, -1, 2000);
  chassis.waitUntilDone();
}