#include "util/doublePark.h"
#include "main.h"
#include "pros/distance.hpp"
#include "subsystem/intake.h"


#include "globals.h"
#include <cstddef>

pros::Distance doubleParkSensor(PORT_LEFT_DISTANCE);

pros::adi::Pneumatics doubleParkLeft(PORT_ADI_DOUBLE_PARK_LEFT, false);
pros::adi::Pneumatics doubleParkRight(PORT_ADI_DOUBLE_PARK_RIGHT, false);

bool doubleParking = false;
bool runDoubleParkingIntake = false;
bool hasDoubleParked = false;
extern DoubleParkState dpState;
DoubleParkState dpState = IDLE;
void doublePark() {
  static int startTime = 0; // Tracks time locally for the waiting step

  switch (dpState) {
  case IDLE:
    // Do nothing, waiting for runDoubleParkToggle() to set state to SEARCHING.
    break;

  case SEARCHING: {
    int distance = doubleParkSensor.get_distance();
    intakeOut(); // Keep intake running out

    if (distance < 150) {
      // Block detected! Move to the next state.

      startTime = pros::millis(); // Start the timer for the delay
      dpState = WAITING_FOR_STOP;
    }
    // If not detected, the function exits and opcontrol() continues.
    break;
  }

  case WAITING_FOR_STOP:
    // Wait 200ms before engaging pneumatics
    if (pros::millis() - startTime >= 500) {
      stopIntake();
      engageDoublePark();
      dpState = ENGAGED_COMPLETE;
    }
    break;

  case ENGAGED_COMPLETE:
    // Routine is complete, do nothing until disengaged.
    break;

  case DISENGAGED:
    // Ready for a new cycle. The toggle will handle the rest.
    break;
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

    if (dpState == IDLE || dpState == DISENGAGED) {
      // Initiate the sequence: Start SEARCHING for the block
      disengageDoublePark(); // Ensure pneumatics are retracted before starting
      dpState = SEARCHING;

    } else if (dpState == ENGAGED_COMPLETE) {
      // Disengage the mechanism, making it ready for the next run
      disengageDoublePark();
      dpState = IDLE; // Back to IDLE state, allowing the next press to start
                      // the search
    }
  }
}
