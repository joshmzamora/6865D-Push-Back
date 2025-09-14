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
extern pros::Motor hood;
extern pros::Motor basket;
extern pros::Motor top_intake;

void intakeTopGoal();
void outtakeTopGoal();
void intakeMiddleGoal();

void intakeStop();
void outtakeMiddleGoal();
void outtakeBottomGoal();
void runIntake();

IntakeState getIntakeState();
void setIntakeState(IntakeState state);

double getIntakeRotations();