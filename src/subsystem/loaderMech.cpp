#include "loaderMech.h"
#include "main.h"

pros::adi::Pneumatics loaderMechLeft(PORT_ADI_LOADER_MECH_LEFT, false);
pros::adi::Pneumatics loaderMechRight(PORT_ADI_LOADER_MECH_RIGHT, false);

void runLoaderMechToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
    loaderMechLeft.toggle();
    loaderMechRight.toggle();
  }
  
}
void disengageLoaderMech() {
  loaderMechLeft.retract();
  loaderMechRight.retract();
}

void engageLoaderMech() {
  loaderMechLeft.extend();
  loaderMechRight.extend();
}

bool isLoaderMechEngaged() {
  return loaderMechLeft.is_extended() && loaderMechRight.is_extended();
}
