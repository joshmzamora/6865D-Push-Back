#include "main.h"
#include "auton/auton.h"
#include "auton/selector.h"
#include "subsystem/middleGoalMech.h"
#include "pros/misc.h"
#include "subsystem/ball_lock.h"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/loaderMech.h"
#include "subsystem/wing.h"
#include "util/colorsort.h"
#include "util/doublePark.h"

void printScreen() {
  while (true) {
    controller.print(0, 0, "X:%.1f Y:%.1f T:%.1f", chassis.getPose().x,
                     chassis.getPose().y,
                     formatHeading(chassis.getPose().theta));
    pros::delay(50);
    std::cout << "(" << chassis.getPose().x << ", " << chassis.getPose().y << ", " << chassis.getPose().theta
              << "), " << std::endl;
  }
}

void initialize() {
  pros::Task printTask(printScreen);
  pros::Task *macroTask = nullptr;
  gui();
  chassis.calibrate();
  
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  //left();
  //right();
   sawp();
}

void opcontrol() {
  engageLeftWing();
  engageBallLock();
  // chassis.setPose(0,0,0);
  // chassis.moveToPoint(0, 24, 24000);
  // chassis.waitUntilDone();
  // chassis.moveToPoint(0, 0, 24000, {.forwards=false});
  // chassis.waitUntilDone();
  while (true) {
    runMacros();
    runIntake();
    runLoaderMechToggle();
    runWingToggle();

    //macro code

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