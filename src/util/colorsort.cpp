#include "auton/selector.h"
#include "subsystem/intake.h"
#include "util/colorsort.h"
#include "main.h"

pros::Optical optical(PORT_OPTICAL);

#define SORT_TIME 1000
#define BLOCK_DISTANCE 75

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
  
  while (true) {
    IntakeState prevIntakeState = getIntakeState();
    IntakeState prevHoodState = getHoodState();
    Alliance seenColor = getColor(optical.get_hue());
    std::cout << "Color: " << getColor(optical.get_hue())
              << " Proximity: " << optical.get_proximity() << std::endl;
    if (seenColor != currentAlliance && seenColor != OTHER && optical.get_proximity() < BLOCK_DISTANCE) {
      setIntakeState(STOPPED, STOPPED);
      setIntakeState(BLOCKED, BLOCKED);
      std::cout << "Sorting Color!" << std::endl;
      setIntakeState(IN, COLOR_SORT);
      pros::delay(SORT_TIME);
      setIntakeState(prevIntakeState, prevHoodState);
      }
    pros::delay(20);
  }
}