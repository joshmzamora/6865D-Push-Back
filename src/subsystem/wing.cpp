#include "subsystem/wing.h"
#include "main.h"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "subsystem/drivetrain.h"

pros::adi::Pneumatics wingLeft(PORT_ADI_WING_LEFT, false);

void runWingToggle() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    wingLeft.extend();
  }
  else {
    wingLeft.retract();
  }
}

void engageLeftWing() { wingLeft.extend(); }

void disengageLeftWing() { wingLeft.retract(); }

bool isWinged() { return wingLeft.is_extended(); }
