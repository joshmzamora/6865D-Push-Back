#include "intake.h"
#include "globals.h"
#include "subsystem/middleGoalMech.h"

pros::Motor intake(PORT_INTAKE);
pros::Motor hood(PORT_HOOD);
IntakeState intakeState = STOPPED;
IntakeState hoodState = STOPPED;

IntakeState getIntakeState() { return intakeState; }
IntakeState getHoodState() { return hoodState; }

void setIntakeState(IntakeState intakeSt, IntakeState hoodSt) {
  intakeState = intakeSt;
  hoodState = hoodSt;
  intake.move(intakeState);
  hood.move(hoodState);
}

void intakeIn() {
  setIntakeState(IN, STOP_BLOCKS);
  disengageMiddleGoalMech();
}

void intakeOut() {
  setIntakeState(OUT, OUT);
  disengageMiddleGoalMech();
}

void doubleParkIntakeIn() {
  setIntakeState(DOUBLE_PARK_IN, STOPPED);
}

void doubleParkIntakeOut() {
  setIntakeState(DOUBLE_PARK_OUT, STOPPED);
}
void intakeMiddle() {
  setIntakeState(IN, MIDDLEGOAL);
  // engageMiddleGoalMech();
}

void intakeTopGoal() {
  setIntakeState(IN, IN);
  disengageMiddleGoalMech();
}

void stopIntake() {
  setIntakeState(STOPPED, STOP_BLOCKS);
  disengageMiddleGoalMech();
}

void runIntake() {
  if (doubleParking &&
      controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
      intakeState != BLOCKED)
    doubleParkIntakeIn();
  else if (doubleParking &&
           controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
           intakeState != BLOCKED)
    doubleParkIntakeOut();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
           intakeState != BLOCKED)
    intakeIn();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
           intakeState != BLOCKED)
    intakeOut();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) &&
           intakeState != BLOCKED)
    intakeTopGoal();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
           intakeState != BLOCKED)
    intakeMiddle();
  else if (intakeState != BLOCKED) {
    stopIntake();
  } 
}
double getIntakeRotations() { return intake.get_position(); }