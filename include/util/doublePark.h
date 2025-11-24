#pragma once
#include "main.h"

enum DoubleParkState {
  IDLE,             // 0. Not active
  SEARCHING,        // 1. Intake is out, waiting for distance sensor
  WAITING_FOR_STOP, // 2. Short delay before engaging pneumatics
  ENGAGED_COMPLETE, // 3. Pneumatics are extended, routine is done
  DISENGAGED        // 4. Pneumatics are retracted, ready for next run
};
extern DoubleParkState dpState;

void runDoubleParkToggle();
void engageDoublePark();
void disengageDoublePark();
void doublePark();

