#include "main.h"
#include "auton/auton.h"
#include "auton/selector.h"
#include "pros/rtos.h"
#include "util/odomLift.h"
#include "pros/misc.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/wing.h"
#include "subsystem/loaderMech.h"
#include "util/goalSense.h"
#include "util/doublePark.h"
#include <iostream>


void printScreen() {
  while (true) {
    controller.print(0, 0, "X:%.2f Y: %.2f T: %.2f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    pros::delay(20);
    std::cout << "(" << chassis.getPose().x << ", " << chassis.getPose().y
              << "), " << std::endl;
  }
}

void initialize() {
 //pros::Task colorSortTask(colorSort);
 // pros::Task redirectTask(macro_redirect);
 // pros::Task collapseTask(macro_collapse);
 pros::Task printTask(printScreen);

 gui();
 chassis.calibrate();
}

void disabled() {}

void competition_initialize() {}

void autonomous() { blueLeftAuton(); }

void opcontrol() {

  // moveForward();
  //redRightAuton();

  while (true) {
    //pidController();
    runIntake();
    runLoaderMechToggle();
    runOdomLiftToggle();
    runGoalSense();
    runDoubleParkToggle();
    doublePark();
    runWingToggle();     
    int yAxis = returnExponential(
        controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), 1, 10);
    int xAxis = returnExponential(
        controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 2, 6);
    chassis.arcade(yAxis, xAxis);
    // printf("X: %.2f Y: %.2f Theta: %.2f\n", chassis.getPose().x,
    //        chassis.getPose().y, formatHeading(chassis.getPose().theta));
    pros::delay(20);
  }
}
