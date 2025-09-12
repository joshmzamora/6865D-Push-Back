#pragma once
#include "intake.h"
#include "main.h"
#include "pros/motor_group.hpp"

enum IntakeState {
    IN = 127,
    OUT = -127,
    STOPPED = 0,
    BLOCKED
};

extern pros::Motor intake;
extern pros::Motor top_intake;

void intakeTopGoal();
void intakeMiddleGoal();
void intakeHood();
void outtakeHood();
void intakeStop();
void outtakeMiddleGoal();
void outtakeBottomGoal();
void runIntake();

IntakeState getIntakeState();
void setIntakeState(IntakeState state);

double getIntakeRotations();