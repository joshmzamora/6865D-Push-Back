#include "pros/distance.hpp"
#include "util/doublePark.h"
#include "main.h"
#include "subsystem/intake.h"
#include "odomLift.h"
#include "globals.h"


pros::Distance doubleParkSensor(PORT_DISTANCE);

pros::adi::Pneumatics doubleParkLeft(PORT_ADI_DOUBLE_PARK_LEFT, false);
pros::adi::Pneumatics doubleParkRight(PORT_ADI_DOUBLE_PARK_RIGHT, false);

bool holdingBlock = false;
bool doubleParking = false;
bool runDoubleParkingIntake = false;

void doublePark() {
  if (doubleParking) {
    int distance = doubleParkSensor.get_distance();
    intakeOut();
    while (distance > 150) {
      distance = doubleParkSensor.get_distance();
      pros::delay(20);
    }
    stopIntake();
    runDoubleParkingIntake = true;
    holdingBlock = true;
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
    engageOdomLift();
    if (!doubleParking) {
      doubleParking = !doubleParking;
    }
    else {
      engageDoublePark();
      doubleParking = !doubleParking;
    }
  }
  else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    runDoubleParkingIntake = false;
    disengageDoublePark();
    //doubleParking = false;
  }
}