#include "loaderMech.h"
#include "main.h"

pros::adi::Pneumatics loaderMechLeft(PORT_ADI_LOADER_MECH_LEFT, false);
pros::adi::Pneumatics loaderMechRight(PORT_ADI_LOADER_MECH_RIGHT, false);

void runLoaderMechToggle() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    engageLoaderMech();
  else
    disengageLoaderMech();
}
void disengageLoaderMech() {
  loaderMechLeft.retract();
  loaderMechRight.retract();
}

void engageLoaderMech() {
  loaderMechLeft.extend();
  loaderMechRight.extend();
}

bool isDoinked() {
  return loaderMechLeft.is_extended() && loaderMechRight.is_extended();
}
