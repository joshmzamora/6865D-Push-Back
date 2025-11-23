#include "middleGoalMech.h"
#include "main.h"

pros::adi::Pneumatics middleGoal(PORT_ADI_MIDDLE_GOAL, false);

void disengageMiddleGoalMech() {
  middleGoal.extend();
}

void engageMiddleGoalMech() {
  middleGoal.retract();
}

bool isDoinked() {
  return middleGoal.is_extended();
}

