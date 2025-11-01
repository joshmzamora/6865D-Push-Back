#include "auton/selector.h"
#include "subsystem/intake.h"
#include <vector>

pros::Optical optical(PORT_OPTICAL);
pros::Optical optical2(PORT_OPTICAL_2);

#define STOP_TIME 300
#define BLOCK_DISTANCE 50

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
  optical.set_led_pwm(0);
  
  while (true) {
    
    IntakeState prevIntakeState = getIntakeState();
    IntakeState prevHoodState = getHoodState();
    Alliance seenColor = getColor(optical.get_hue());
    // std::cout << optical.get_hue() << std::endl;
    
    if (seenColor != currentAlliance && seenColor != OTHER) {
        setIntakeState(STOPPED, STOPPED);
        setIntakeState(BLOCKED, BLOCKED);
        hood.move(127);
        intake.move(127);
        while (true) {
          pros::delay(20);
          Alliance newSeenColor = getColor(optical.get_hue());
          if (newSeenColor == currentAlliance || newSeenColor == OTHER) {
            break;
          }
        }
        setIntakeState(prevIntakeState, prevHoodState);
      }
    if (seenColor == currentAlliance) {
      setIntakeState(prevIntakeState, prevHoodState);
      }
      pros::delay(20);
  }
}

// double getOpticalColor()
// {
// return optical.get_hue();
// }

void setIntakeHold(bool hold) { holdBlock = hold; }