#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"
#include "subsystem/Wing.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"

void redRightAuton() {
  chassis.setPose(0, 0, 0);
  setIntakeState({IN, IN, IN, IN});
  chassis.moveToPoint(0, 52, 5000, {.minSpeed = 25});
}