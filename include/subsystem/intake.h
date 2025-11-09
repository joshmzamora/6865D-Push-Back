#pragma once
#include "intake.h"
#include "main.h"
#include "pros/motor_group.hpp"

enum IntakeState {
  IN = 127,
  MIDDLEGOAL = 64,
    STOPBLOCKS = -16,
    OUT = -127,
    STOPPED = 0,
    BLOCKED
};

extern pros::Motor intake;
extern pros::Motor hood;
void intakeIn(); 
void intakeOut(); 
void intakeTopGoal();
void intakeMiddle(); 
void stopIntake();
void runIntake();

IntakeState getIntakeState();
IntakeState getHoodState();
void setIntakeState(IntakeState intake, IntakeState hood);

double getIntakeRotations();