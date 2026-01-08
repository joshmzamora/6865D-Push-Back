#pragma once
#include "main.h"
#include "pros/motor_group.hpp"

enum IntakeState {
  IN = 127,
  MIDDLEGOAL = 96,
  OUT = -127,
  COLOR_SORT = -100,
  HOOD_INTAKE = -100,
  STOPPED = 0,
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
void colorSortIntake();
void runIntake();
void jamIntake();
IntakeState getIntakeState();
IntakeState getHoodState();
void setIntakeState(IntakeState intake, IntakeState hood);

double getIntakeRotations();