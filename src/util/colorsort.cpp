#include "auton/selector.h"
#include "main.h"
#include "pros/distance.hpp"
#include "subsystem/drivetrain.h"
#include "subsystem/intake.h"
#include <iostream>
#include <vector>

pros::Optical optical(PORT_OPTICAL);
pros::Distance distance(PORT_DISTANCE);

#define STOP_DISTANCE 50
#define WAIT_TIME 20
#define STOP_TIME 10000

bool holdBlock = false;

Alliance getColor(double hue) {
  if (hue > 120 && hue < 230)
    return BLUE;
  else if (hue > 0 && hue < 25)
    return RED;
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
    Alliance seenColor = getColor(optical.get_hue());
    int startingIntakeRot = getIntakeRotations();

    if (seenColor != currentAlliance && seenColor != OTHER) {
      bool resetSort = false;
      std::cout << "Seen Opposite Color" << std::endl;
      while (distance.get_distance() > 20) {
        if (getColor(optical.get_hue()) == currentAlliance &&
            seenColor != OTHER) {
          resetSort = true;
          break;
        }
        pros::delay(20);
      }

      if (!resetSort) {
        pros::delay(WAIT_TIME);
        // setIntakeState({STOPPED, STOPPED, STOPPED, STOPPED});
        controller.rumble(".");
        std::cout << "sorting" << std::endl;
        setIntakeState({BLOCKED, BLOCKED, BLOCKED, BLOCKED});
        intakeMiddle();
        if (distance.get_distance() < STOP_DISTANCE) {
           
          setIntakeState({states[0], states[1], states[2], states[3]});
        }
        setIntakeState({states[0], states[1], states[2], states[3]});
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