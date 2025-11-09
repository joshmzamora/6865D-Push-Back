#include "pros/distance.hpp"
#include "util/doublePark.h"
#include "main.h"
#include "subsystem/intake.h"
#include "odomLift.h"

pros::Distance doubleParkSensor(PORT_DISTANCE);

pros::adi::Pneumatics doubleParkLeft(PORT_ADI_DOUBLE_PARK_LEFT, false);
pros::adi::Pneumatics doubleParkRight(PORT_ADI_DOUBLE_PARK_RIGHT, false);

bool holdingBlock = false;
void doublePark() {
  while (!holdingBlock) {
    double distance = doubleParkSensor.get_distance();
    if (distance < 50) { //first stage
        setIntakeState(STOPPED, STOPPED);
        holdingBlock = true;
        engageOdomLift();
    }
    pros::delay(20);
  }
}

void runDoubleParkToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    if (holdingBlock){
        setIntakeState(STOPPED, STOPPED);
        doubleParkLeft.extend();
        doubleParkRight.extend();
        engageOdomLift();
    } else {
        doublePark();
    }
  }
}