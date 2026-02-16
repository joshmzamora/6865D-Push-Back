#pragma once
#include "main.h"

extern bool runDoubleParkingIntake;

#define PORT_OPTICAL 15
#define PORT_INERTIAL 6
#define PORT_LEFT_DISTANCE 7
#define PORT_RIGHT_DISTANCE 17
#define PORT_FRONT_DISTANCE 14
#define PORT_BACK_DISTANCE 4

#define PORT_INTAKE (1)
#define PORT_HOOD (-11)

#define PORT_DRIVE_LEFT_FRONT (-10)  // front
#define PORT_DRIVE_LEFT_TOP (-8)    // back
#define PORT_DRIVE_LEFT_BOTTOM (-9) // middle
#define PORT_DRIVE_RIGHT_FRONT 19
#define PORT_DRIVE_RIGHT_TOP 18 // reversed
#define PORT_DRIVE_RIGHT_BOTTOM 20

#define PORT_ADI_LOADER_MECH_LEFT 'A'
#define PORT_ADI_LOADER_MECH_RIGHT 'H'
#define PORT_ADI_DOUBLE_PARK_LEFT 'Z'
#define PORT_ADI_DOUBLE_PARK_RIGHT 'Z'
#define PORT_ADI_MIDDLE_GOAL 'F'
#define PORT_ADI_WING_LEFT 'B'
#define PORT_ADI_BALL_LOCK 'G'
