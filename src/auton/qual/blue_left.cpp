
#include "subsystem/drivetrain.h"
#include "subsystem/loaderMech.h"
void blueLeftAuton() {
  chassis.setPose(0, 0, 0);
  chassis.moveToPose(24, 24, 0, 1000);
}