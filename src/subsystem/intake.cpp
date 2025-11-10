#include "intake.h"
#include "globals.h"
#include "main.h"
#include "subsystem/loaderMech.h"
#include "subsystem/middleGoalMech.h"


#include <vector>

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
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
      intakeState != BLOCKED)
    intakeOut();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
           intakeState != BLOCKED)
    intakeIn();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) &&
           intakeState != BLOCKED)
    intakeTopGoal();
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) &&
           intakeState != BLOCKED)
    intakeMiddle();
  else if (controller.get_digital_new_press(
               pros::E_CONTROLLER_DIGITAL_LEFT)) { // double park override and
                                                   // fine tuning
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) &&
        intakeState != BLOCKED)
      setIntakeState(DOUBLE_PARK_OUT, STOPPED);
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
             intakeState != BLOCKED)
      setIntakeState(IN, STOPPED);
  } else if (intakeState != BLOCKED) {
    stopIntake();
  }
}
double getIntakeRotations() { return intake.get_position(); }