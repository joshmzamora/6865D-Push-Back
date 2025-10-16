#include "loaderMech.h"
#include "main.h"

pros::adi::Pneumatics loaderMech(PORT_ADI_LOADER_MECH, false);

void runLoaderMechToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
    loaderMech.toggle();
  }
  
}
void disengageLoaderMech() {
  loaderMech.retract();
}

void engageLoaderMech() {
  loaderMech.extend();
}

bool isLoaderMechEngaged() {
  return loaderMech.is_extended();
}
