#include "subsystem/wing.h"
#include "main.h"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "subsystem/drivetrain.h"

pros::adi::Pneumatics ballLock(PORT_ADI_BALL_LOCK, false);

void runBallLockToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    ballLock.toggle();
  }
}

void engageBallLock() { ballLock.extend(); }

void disengageBallLock() { ballLock.retract(); }

bool isBallLocked() { return ballLock.is_extended(); }