#include "main.h"
#include "pros/rtos.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "subsystem/intake.h"
#include "subsystem/latch.h"
#include "subsystem/drivetrain.h"


void redRightAuton() {
	chassis.setPose(0, 0, 0);
	setIntakeState({IN, IN, IN, IN});
	chassis.moveToPoint(0, 52,5000, {.minSpeed = 25});
}