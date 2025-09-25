#include "subsystem/wing.h"
#include "main.h"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "subsystem/drivetrain.h"

pros::adi::Pneumatics wingLeft(PORT_ADI_WING_LEFT, false);
pros::adi::Pneumatics wingRight(PORT_ADI_WING_RIGHT, false);

void runWingToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    wingLeft.toggle();
  }
  else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    wingRight.toggle();
  }
}

void engageLeftWing() { wingLeft.extend(); }

void engageRightWing() { wingRight.extend(); }

void disengageLeftWing() {
  wingLeft.retract();
}

void disengageRightWing() {
  wingRight.retract();
}

bool isWinged() { return wingLeft.is_extended(); }
