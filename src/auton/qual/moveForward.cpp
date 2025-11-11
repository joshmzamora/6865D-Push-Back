
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void moveForward() {
  chassis.setPose(0, 0, 0);
  chassis.turnToHeading(45, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(90, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(270, 2000);
  chassis.waitUntilDone();
  chassis.turnToHeading(360, 2000);
  chassis.waitUntilDone();
}