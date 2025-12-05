#pragma once
#include "intake.h"
#include "main.h"
#include "pros/motor_group.hpp"

enum IntakeState {
  IN = 127,
  MIDDLEGOAL = 96,
  OUT = -127,
  COLOR_SORT = -100,
  STOPPED = 0,
  DOUBLE_PARK_IN = 25,
  DOUBLE_PARK_OUT = -25,
  BLOCKED
};

extern pros::Motor intake;
extern pros::Motor hood;
void intakeIn();
void intakeOut();
void doubleParkIntakeIn();
void doubleParkIntakeOut();
void intakeTopGoal();
void intakeMiddle();
void stopIntake();
void runIntake();
void jamIntake();
IntakeState getIntakeState();
IntakeState getHoodState();
void setIntakeState(IntakeState intake, IntakeState hood);

double getIntakeRotations();