#include "intake.h"
#include "drivetrain.h"
#include "globals.h"
#include "loaderMech.h"
#include "subsystem/middleGoalMech.h"
#include "subsystem/ball_lock.h"

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


void intakeIn() { setIntakeState(IN, HOOD_INTAKE); disengageBallLock(); }

void intakeOut() { setIntakeState(OUT, OUT); disengageBallLock(); }


void intakeMiddle() { setIntakeState(IN, IN); engageMiddleGoalMech(); engageBallLock();}

void intakeTopGoal() { setIntakeState(IN, IN); engageBallLock(); }

void stopIntake() {
  setIntakeState(STOPPED, STOPPED);
  engageMiddleGoalMech();
}

void colorSortIntake() {setIntakeState(IN, COLOR_SORT); }

void runIntake() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) &&
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
  else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) &&
           intakeState != BLOCKED)
    engageMiddleGoalMech();
  else if (intakeState != BLOCKED) {
    stopIntake();
  }

}

void jamIntake() {
  setIntakeState(OUT, OUT);
  pros::delay(200);
  intakeIn();
}


double getIntakeRotations() { return intake.get_position(); }