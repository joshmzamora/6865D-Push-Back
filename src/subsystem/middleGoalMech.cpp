#include "middleGoalMech.h"
#include "main.h"

pros::adi::Pneumatics middleGoal(PORT_ADI_MIDDLE_GOAL, false);

void disengageMiddleGoalMech() {
  middleGoal.retract();
}

void engageMiddleGoalMech() {
  middleGoal.extend();
}

