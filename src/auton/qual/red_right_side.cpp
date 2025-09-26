#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"

void redRightAuton() {
  chassis.setPose(-47, -16, 90);
  setIntakeState({IN, IN, IN, IN});
  chassis.moveToPose(-22, -22, 70, 5000);
  chassis.waitUntilDone();
}