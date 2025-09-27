#include "intake.h"
#include "globals.h"
#include "main.h"
#include <vector>

pros::Motor intake(PORT_INTAKE);
pros::Motor top_intake(PORT_TOP_INTAKE);
pros::Motor basket(PORT_BASKET);
pros::Motor hood(PORT_HOOD);
IntakeState intakeState = STOPPED;
IntakeState top_intakeState = STOPPED;
IntakeState basketState = STOPPED;
IntakeState hoodState = STOPPED;

void runIntake() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
      intakeState != BLOCKED)
    setIntakeState({IN, OUT, OUT, STOPPED}); // intake Middle Goal
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) &&
           intakeState != BLOCKED)
    setIntakeState({IN, IN, STOPPED, IN}); // intake Top Goal
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
           intakeState != BLOCKED)
    setIntakeState({IN, STOPPED, IN, STOPPED}); // intake Basket
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
           intakeState != BLOCKED)
    setIntakeState({OUT, STOPPED, OUT, STOPPED}); // outtake Basket
  else if (intakeState != BLOCKED)
    setIntakeState({STOPPED, STOPPED, STOPPED, STOPPED});
}

void intakeBasket() { setIntakeState({IN, STOPPED, IN, STOPPED}); }
void intakeTop() { setIntakeState({IN, IN, STOPPED, IN}); }
void intakeMiddle() { setIntakeState({IN, OUT, OUT, STOPPED}); }
void outtakeBasket() { setIntakeState({OUT, STOPPED, OUT, STOPPED}); }
void stopIntake() { setIntakeState({STOPPED, STOPPED, STOPPED, STOPPED}); }

std::vector<IntakeState> getIntakeState() {
  std::vector<IntakeState> states;
  states.push_back(intakeState);
  states.push_back(hoodState);
  states.push_back(basketState);
  states.push_back(top_intakeState);
  return states;
}

void setIntakeState(std::vector<IntakeState> state) {
  intakeState = state[0];
  top_intakeState = state[1];
  basketState = state[2];
  hoodState = state[3];
  intake.move(intakeState);
  top_intake.move(top_intakeState);
  basket.move(basketState);
  hood.move(hoodState);
}

double getIntakeRotations() { return intake.get_position(); }