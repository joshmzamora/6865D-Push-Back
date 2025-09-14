
#include "intake.h"
#include "globals.h"
#include "main.h"

pros::Motor intake(PORT_INTAKE);
pros::Motor top_intake(PORT_TOP_INTAKE);
pros::Motor basket(PORT_BASKET);
pros::Motor hood(PORT_HOOD);
IntakeState intakeState = STOPPED;

void intakeTopGoal(){
    intakeState = IN;
    intake.move(IN);
    top_intake.move(IN);
    basket.move(OUT);
    hood.move(IN);
}

void intakeMiddleGoal(){
    intakeState = IN;
    intake.move(IN);
    top_intake.move(OUT);
    basket.move(OUT);
}



void outtakeTopGoal(){
    intakeState = OUT;
    intake.move(IN);
    top_intake.move(IN);
    basket.move(OUT);
    hood.move(OUT);
}

void intakeStop(){
    intakeState = STOPPED;
    intake.move(STOPPED);
    top_intake.move(STOPPED);
    basket.move(STOPPED);
    hood.move(STOPPED);
}

void outtakeMiddleGoal(){
    intakeState = OUT;
    intake.move(IN);
    top_intake.move(OUT);
    basket.move(OUT);
}

void intakeBasket(){
    intakeState = IN;
    intake.move(IN);
    basket.move(IN);
}

void outtakeBottomGoal(){
    intakeState = OUT;
    intake.move(OUT);
    basket.move(OUT);
}

void runIntake() {
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ) intakeMiddleGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y) ) intakeTopGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) ) outtakeTopGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) intakeBasket(); 
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) outtakeBottomGoal(); 
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) outtakeMiddleGoal();
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))  outtakeTopGoal();
    else if (intakeState != BLOCKED) intakeStop();
}

IntakeState getIntakeState() {
    return intakeState;
}

void setIntakeState(IntakeState state) {
    intakeState = state;
    hood.move(state);
}

double getIntakeRotations() {
    return intake.get_position();
}