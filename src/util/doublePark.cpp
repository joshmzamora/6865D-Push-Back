#include "pros/distance.hpp"
#include "util/doublePark.h"
#include "main.h"
#include "subsystem/intake.h"
#include "odomLift.h"

pros::Distance doubleParkSensor(PORT_DISTANCE);

pros::adi::Pneumatics doubleParkLeft(PORT_ADI_DOUBLE_PARK_LEFT, false);
pros::adi::Pneumatics doubleParkRight(PORT_ADI_DOUBLE_PARK_RIGHT, false);

bool holdingBlock = true;
void doublePark() {
  while (!holdingBlock) {
    double distance = doubleParkSensor.get_distance();
    if (distance < 125) { //first stage
        setIntakeState(STOPPED, STOPPED);
        engageOdomLift();
        holdingBlock = true;
    }
    pros::delay(20);
  }
}

void engageDoublePark() {
    doubleParkLeft.extend();
    doubleParkRight.extend();
}
void disengageDoublePark() {
    doubleParkLeft.retract();
    doubleParkRight.retract();
}

void runDoubleParkToggle() {
  static bool toggle{false};
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    if (!toggle) {
      engageDoublePark();
      toggle = !toggle;
    } else {
      disengageDoublePark();
      toggle = !toggle;
    }
  }
  // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
  //   doubleParkLeft.extend();
  //   doubleParkRight.extend();
  //   // if (holdingBlock){
  //   //     setIntakeState(STOPPED, STOPPED);
  //   //     doubleParkLeft.extend();
  //   //     doubleParkRight.extend();
  //   //     engageOdomLift();
  //   // } else {
  //   //     doublePark();
  //   // }
  // }
}