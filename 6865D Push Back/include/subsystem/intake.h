#pragma once
#include "main.h"
#include "pros/motor_group.hpp"

enum IntakeState {
    IN = 127,
    OUT = -127,
    STOPPED = 0,
    BLOCKED
};

extern pros::Motor intake;
extern pros::Motor hooks;

void intakeTopGoal();
void intakeMiddleGoal();
void intakeStop();
void outtakeMiddleGoal();
void outtakeBottomGoal();
void runIntake();

IntakeState getIntakeState();
void setIntakeState(IntakeState state);

double getIntakeRotations();