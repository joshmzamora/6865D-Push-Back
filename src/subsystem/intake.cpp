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
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ) setIntakeState({IN, OUT, OUT, STOPPED}); //intake Middle Goal
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y) ) setIntakeState({IN, IN, OUT, IN}); //intake Top Goal
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) ) setIntakeState({IN, IN, OUT, OUT}); //outtake Top Goal
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) setIntakeState({IN, STOPPED, IN, STOPPED}); //intake Basket
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) setIntakeState({OUT, STOPPED, OUT, STOPPED}); //outtake Basket
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) setIntakeState({OUT, IN, IN, STOPPED}); //outtake Middle Goal
    else if (intakeState != BLOCKED) setIntakeState({STOPPED, STOPPED, STOPPED, STOPPED});
}

std::vector<IntakeState> getIntakeState() {
    std::vector<IntakeState> states;
    states.push_back(intakeState);
    states.push_back(hoodState);
    states.push_back(basketState);
    states.push_back(top_intakeState);
    return states;
}

void setIntakeState( std::vector<IntakeState> state) {
    intake.move(state[0]);
    hood.move(state[1]);
    basket.move(state[2]);
    top_intake.move(state[3]);
}

double getIntakeRotations() {
    return intake.get_position();
}