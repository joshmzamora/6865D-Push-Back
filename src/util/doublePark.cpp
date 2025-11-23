#include "pros/distance.hpp"
#include "util/doublePark.h"
#include "main.h"
#include "subsystem/intake.h"
#include "odomLift.h"
#include "globals.h"


pros::Distance doubleParkSensor(PORT_DISTANCE);

pros::adi::Pneumatics doubleParkLeft(PORT_ADI_DOUBLE_PARK_LEFT, false);
pros::adi::Pneumatics doubleParkRight(PORT_ADI_DOUBLE_PARK_RIGHT, false);

bool doubleParking = false;
bool runDoubleParkingIntake = false;
bool hasDoubleParked = false;

void doublePark() {
  int startTime = pros::millis();
  if (doubleParking) {
    //runDoubleParkingIntake = true;
    int distance = doubleParkSensor.get_distance();
    intakeOut();
    while (distance > 100 || pros::millis() - startTime > 2000) {
      distance = doubleParkSensor.get_distance();
      pros::delay(20);
    }
    pros::delay(200);
    stopIntake();
    engageDoublePark();
    doubleParking = false;
    hasDoubleParked = true;
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
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    if (!doubleParking) {
      doubleParking = !doubleParking;
    }
    if (hasDoubleParked) {
      disengageDoublePark();
      //doubleParking = true;
    }
  }
}
