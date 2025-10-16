#include "middleGoalMech.h"
#include "main.h"

pros::adi::Pneumatics middleGoal(PORT_ADI_MIDDLE_GOAL, false);

void runMiddleGoalToggle() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
    middleGoal.toggle();
  }
  
}
void disengageMiddleGoalMech() {
  middleGoal.retract();
}

void engageMiddleGoalMech() {
  middleGoal.extend();
}

bool isDoinked() {
  return middleGoal.is_extended();
}
