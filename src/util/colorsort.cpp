#include "auton/selector.h"
#include "main.h"
#include "pros/distance.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include "subsystem/middleGoalMech.h"
#include <iostream>
#include <vector>

pros::Optical optical(PORT_OPTICAL);

#define STOP_TIME 400

bool holdBlock = false;

Alliance getColor(double hue) {
  if (hue > 170 && hue < 230) {
    return BLUE;  
  } else if (hue > 0 && hue < 10 || hue > 350 && hue < 360) {
    return RED;
  }
  return OTHER;
}

void colorSort() {
  optical.set_led_pwm(100);
  
  while (true) {
    std::vector<IntakeState> states = getIntakeState();
    IntakeState prevIntakeState = states[0];
    IntakeState prevHoodState = states[1];
    Alliance seenColor = getColor(optical.get_hue());
    //std::cout << optical.get_hue() << std::endl;
    if (seenColor != currentAlliance && seenColor != OTHER) {
        setIntakeState({STOPPED, STOPPED});
        setIntakeState({BLOCKED, BLOCKED});
        hood.move(127);
        intake.move(127);
        pros::delay(STOP_TIME);
        setIntakeState({prevIntakeState, prevHoodState});
      }
    if (seenColor == currentAlliance) {
      setIntakeState({prevIntakeState, prevHoodState});
      }
      pros::delay(20);
  }
}

// double getOpticalColor()
// {
// return optical.get_hue();
// }

void setIntakeHold(bool hold) { holdBlock = hold; }