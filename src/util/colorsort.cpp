#include "auton/selector.h"
#include "main.h"
#include "pros/distance.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/middleGoalMech.h"
#include <iostream>
#include <vector>

pros::Optical optical(PORT_OPTICAL);
pros::Distance sortSensor(PORT_DISTANCE);

#define STOP_DISTANCE 180
#define WAIT_TIME 20
#define STOP_TIME 1000

bool holdBlock = false;

Alliance getColor(double hue) {
  if (hue > 170 && hue < 230) {
    std::cout << "BLUE" << std::endl;
    return BLUE;  
  } else if (hue > 0 && hue < 25) {
    std::cout << "RED" << std::endl;
    return RED;
  }
  return OTHER;
}

void colorSort() {
  optical.set_led_pwm(100);
  while (true) {
    // controller.print(1, 1, "%f %f %f %f", getIntakeState()[0],
    // getIntakeState()[1], getIntakeState()[2], getIntakeState()[3]);
    // IntakeState prevIntakeState = getIntakeState();
    // std::vector<int> states = getIntakeState();
    // std::cout << "\rIntake State: " << states[0] << " " << states[1] << " "
    // << states[2] << " " << states[3] << std::endl;
    // std::cout << "\rDistance: " << distance.get() << std::endl;
    // controller.print(1, 1, "D:%.2d", distance.get());
    std::vector<IntakeState> states = getIntakeState();
    IntakeState prevIntakeState = states[0];
    IntakeState prevHoodState = states[1];
    Alliance seenColor = getColor(optical.get_hue());
    int startingIntakeRot = getIntakeRotations();
    if (seenColor != currentAlliance && seenColor != OTHER) {
      bool resetSort = false;
      
      while (sortSensor.get_distance() > 20) {
        
        if (getColor(optical.get_hue()) == currentAlliance &&
        seenColor != OTHER) {
          resetSort = true;
          break;
        }
        pros::delay(20);
      }
      if (!resetSort) {
        pros::delay(WAIT_TIME);

        setIntakeState({STOPPED, STOPPED});
        setIntakeState({BLOCKED, BLOCKED});
        engageMiddleGoalMech();
        //setIntakeState({BLOCKED, IN});
        pros::delay(STOP_TIME);
        disengageMiddleGoalMech();
        setIntakeState({prevIntakeState, prevHoodState});
      }
    
      pros::delay(20);
    }
  }
}
// double getOpticalColor()
// {
// return optical.get_hue();
// }

void setIntakeHold(bool hold) { holdBlock = hold; }