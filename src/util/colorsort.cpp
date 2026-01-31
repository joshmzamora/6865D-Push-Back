#include "auton/selector.h"
#include "subsystem/intake.h"
#include "util/colorsort.h"
#include "main.h"

pros::Optical optical(PORT_OPTICAL);

#define SORT_TIME 400
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
  }
