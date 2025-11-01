#include "subsystem/wing.h"
#include "main.h"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "subsystem/drivetrain.h"

pros::adi::Pneumatics wingRight(PORT_ADI_WING_RIGHT, false);

void runWingToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    wingRight.toggle();
    disengageRightWing();
  }
}

void engageRightWing() { wingRight.extend(); }

void disengageRightWing() { wingRight.retract(); }

bool isWinged() { return wingRight.is_extended(); }
