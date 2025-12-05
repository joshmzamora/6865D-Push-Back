#include "auton/selector.h"
#include "subsystem/intake.h"
#include "util/colorsort.h"
#include <vector>

pros::Optical optical(PORT_OPTICAL);

#define STOP_TIME 1000
#define BLOCK_DISTANCE 50

bool holdBlock = false;

Alliance getColor(double hue) {
  if (hue > 170 && hue < 230) {
    return BLUE;  
  } else if (hue > 0 && hue < 30 || hue > 350 && hue < 360) {
    return RED;
  }
  return OTHER;
}

void intakeColorSort() {
  // optical.set_led_pwm(100);
  
  // while (true) {
  //   IntakeState prevIntakeState = getIntakeState();
  //   IntakeState prevHoodState = getHoodState();
  //   Alliance seenColor = getColor(optical.get_hue());
  //   // std::cout << optical.get_hue() << std::endl;
  //   // std::cout << "test2" << std::endl;
  //   if (seenColor != currentAlliance && seenColor != OTHER) {
  //     setIntakeState(STOPPED, STOPPED);
  //     setIntakeState(BLOCKED, BLOCKED);
  //     setIntakeState(IN, COLOR_SORT);
  //     pros::delay(1000);
  //     setIntakeState(prevIntakeState, prevHoodState);
  //     }
  //   pros::delay(20);
  // }
}

void longGoalColorSort() {

}
// double getOpticalColor()
// {
// return optical.get_hue();
// }

void setIntakeHold(bool hold) { holdBlock = hold; }