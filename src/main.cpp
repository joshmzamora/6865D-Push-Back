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
    controller.print(0, 0, "X:%.2f Y: %.2f T: %.2f", chassis.getPose().x,
                     chassis.getPose().y, chassis.getPose().theta);
    pros::delay(20);
    std::cout << "(" << chassis.getPose().x << ", " << chassis.getPose().y
              << "), " << std::endl;
  }
}

void initialize() {
  // pros::Task colorSortTask(colorSort);
  pros::Task printTask(printScreen);
  gui();
  chassis.calibrate();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  // autonSelector();
  //  skillsAuton();
  //leftAuton();
   rightAuton();
  //soloAWP();
}

void opcontrol() {
  // skillsAuton();
  // leftAuton();
   //rightAuton();
  // soloAWP();
  // moveForward();
  
  //leftAuton();
  engageLeftWing();
  //engageMiddleGoalMech();
  while (true) {
    // pidController();
    // doublePark();
    // runDoubleParkToggle();
    // colorSort();
    runIntake();
    runLoaderMechToggle();
    runWingToggle();
    // runBallLockToggle();
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