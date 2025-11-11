
#include "main.h"
#include "odomLift.h"
#include "pros/misc.h"

pros::adi::Pneumatics odomLift(PORT_ADI_ODOM_LIFT, false);

void disengageOdomLift() { odomLift.retract(); }

void engageOdomLift() { odomLift.extend(); }

bool isOdomLiftEngaged() { return odomLift.is_extended(); }

void runOdomLiftToggle() {
  static bool toggle{false};
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
    if (!toggle) {
      engageOdomLift();
      toggle = !toggle;
    } else {
      disengageOdomLift();
      toggle = !toggle;
    }
  }
}