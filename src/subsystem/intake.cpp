#include "intake.h"
#include "globals.h"
#include "main.h"
#include <vector>

pros::Motor intake(PORT_INTAKE);
pros::Motor hood(PORT_HOOD);
pros::adi::Pneumatics middleGoal(PORT_ADI_MIDDLE_GOAL, false);
IntakeState intakeState = STOPPED;
IntakeState hoodState = STOPPED;

std::vector<IntakeState> getIntakeState() {
  std::vector<IntakeState> states;
  states.push_back(intakeState);
  states.push_back(hoodState);
  return states;
}

void setIntakeState(std::vector<IntakeState> state) {
  intakeState = state[0];
  hoodState = state[1];
  intake.move(intakeState);
  hood.move(hoodState);
}

void intakeIn() {
  setIntakeState({IN, STOPBLOCKS});
  middleGoal.retract();
}

void intakeOut() {
  setIntakeState({OUT, OUT});
  middleGoal.retract();
}

void intakeMiddle() {
  setIntakeState({IN, OUT});
  middleGoal.extend();
}

void intakeTopGoal() {
  setIntakeState({IN, IN});
  middleGoal.retract();
}

void stopIntake() { setIntakeState({STOPPED, STOPBLOCKS}); }

void runIntake() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
      intakeState != BLOCKED)
    intakeOut();

  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
           intakeState != BLOCKED)
    intakeIn();

  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) &&
           intakeState != BLOCKED)
    intakeTopGoal();

  else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1) &&
           intakeState != BLOCKED)
    intakeMiddle();

  else if (intakeState != BLOCKED) {
    stopIntake();
  }
}
double getIntakeRotations() { return intake.get_position(); }