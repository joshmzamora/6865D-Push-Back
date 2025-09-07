
#include "globals.h"
#include "main.h"

pros::Motor intake(PORT_INTAKE);
pros::Motor hooks(PORT_HOOKS);
pros::Motor basket(PORT_BASKET);
IntakeState intakeState = STOPPED;

void intakeTopGoal(){
    intakeState = IN;
    intake.move(IN);
    hooks.move(IN);
    basket.move(OUT);
}

void intakeMiddleGoal(){
    intakeState = IN;
    intake.move(IN);
    hooks.move(OUT);
    basket.move(OUT);
}



void intakeStop(){
    intakeState = STOPPED;
    intake.move(STOPPED);
    hooks.move(STOPPED);
    basket.move(STOPPED);
}

void outtakeMiddleGoal(){
    intakeState = IN;
    intake.move(IN);
    hooks.move(OUT);
    basket.move(OUT);
}

void intakeBasket(){
    intakeState = IN;
    intake.move(IN);
    basket.move(IN);
}

void outtakeBottomGoal(){
    intakeState = IN;
    intake.move(OUT);
    basket.move(OUT);
}

void runIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ) intakeTopGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) ) intakeMiddleGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) outtakeBottomGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) intakeBasket(); 
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) outtakeMiddleGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))  intakeTopGoal();
    else if (intakeState != BLOCKED) intakeStop();
}

IntakeState getIntakeState() {
    return intakeState;
}

void setIntakeState(IntakeState state) {
    intakeState = state;
    hooks.move(state);
}

double getIntakeRotations() {
    return hooks.get_position();
}