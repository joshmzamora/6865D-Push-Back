#include "auton/selector.h"
#include "subsystem/intake.h"
#include "util/colorsort.h"
#include "main.h"

pros::Optical optical(PORT_OPTICAL);

#define SORT_TIME 250
#define BLOCK_DISTANCE 30

Alliance getColor(double hue) {
  if (hue > 170 && hue < 230) {
    return BLUE;  
  } else if (hue > 0 && hue < 30 || hue > 350 && hue < 360) {
    return RED;
  }
  return OTHER;
}

void colorSort() {
  optical.set_led_pwm(100);
    IntakeState prevIntakeState = getIntakeState();
    IntakeState prevHoodState = getHoodState();
    Alliance seenColor = getColor(optical.get_hue());
    std::cout << "Color: " << seenColor
              << std::endl;
    if (seenColor != currentAlliance && seenColor != OTHER) {
      // setIntakeState(STOPPED, STOPPED);
      setIntakeState(BLOCKED, BLOCKED);
      std::cout << "sorting" << std::endl;
      colorSortIntake(); //setIntakeState(IN, COLOR_SORT);
      pros::delay(SORT_TIME);
      std::cout << " done sorting" << std::endl;
      setIntakeState(prevIntakeState, prevHoodState);
      std::cout << " set states" << std::endl;
      }
    pros::delay(20);
  }
