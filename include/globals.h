#pragma once
#include "main.h"

extern bool runDoubleParkingIntake;
#define PORT_OPTICAL 2
#define PORT_INERTIAL 7
// #define PORT_ROTATION_HORIZONTAL 7
// #define PORT_ROTATION_VERTICAL (3)
#define PORT_LEFT_DISTANCE 10
#define PORT_RIGHT_DISTANCE 11
#define PORT_GPS 8 //wrong

#define PORT_INTAKE (1)
#define PORT_HOOD (-11)

#define PORT_DRIVE_LEFT_FRONT (-10)  // front
#define PORT_DRIVE_LEFT_TOP (-5)    // back
#define PORT_DRIVE_LEFT_BOTTOM (-6) // middle
#define PORT_DRIVE_RIGHT_FRONT 19
#define PORT_DRIVE_RIGHT_TOP 15 // reversed
#define PORT_DRIVE_RIGHT_BOTTOM 13

#define PORT_ADI_LOADER_MECH_LEFT 'D'
#define PORT_ADI_LOADER_MECH_RIGHT 'A'
#define PORT_ADI_DOUBLE_PARK_LEFT 'Z'
#define PORT_ADI_DOUBLE_PARK_RIGHT 'Z'
#define PORT_ADI_MIDDLE_GOAL 'B'
#define PORT_ADI_WING_LEFT 'C'
#define PORT_ADI_BALL_LOCK 'E'
