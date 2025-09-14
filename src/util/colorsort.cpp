#include "auton/selector.h"
#include "main.h"
#include "pros/distance.hpp"
#include "subsystem/intake.h"
#include <iostream>


pros::Optical optical(PORT_OPTICAL);
pros::Distance distance(PORT_DISTANCE);

#define STOP_DISTANCE 50
#define WAIT_TIME 500

bool holdBlock = false;

Alliance getColor(double hue) {
  if (hue > 190 && hue < 230)
    return BLUE;
  else if (hue > 0 && hue < 25)
    return RED;
  return OTHER;
}

void colorSort() {
  optical.set_led_pwm(100);
  while (true) {
    IntakeState prevIntakeState = getIntakeState();
    Alliance seenColor = getColor(optical.get_hue());
    int startingIntakeRot = getIntakeRotations();
    bool seen = false;
     if (seenColor != currentAlliance && seenColor != OTHER) {
     
     
      
       seen = true;
      // pros::delay(300);
       
       while (distance.get()<STOP_DISTANCE) {
         pros::delay(20);
          intake.move(OUT);
        hood.move(OUT);
        }
        setIntakeState(BLOCKED);

        pros::delay(WAIT_TIME);
        setIntakeState(prevIntakeState);
        seen=false;
      
    }
    else if (holdBlock && seenColor == currentAlliance && seenColor != OTHER) {
      setIntakeState(STOPPED);
  }
    // if (seenColor != currentAlliance && seenColor != OTHER) {
    //   while (distance.get()<STOP_DISTANCE) {
    //     pros::delay(20);
    //     top_intake.move(IN);
    //     controller.rumble("........");
    //   }
    //   setIntakeState(BLOCKED);

    //   pros::delay(WAIT_TIME);
    //   setIntakeState(prevIntakeState);
    // }
    // else if (holdBlock && seenColor == currentAlliance && seenColor != OTHER) {
    //   setIntakeState(STOPPED);
    // }
    // pros::delay(20);
    
    
}
}
// double getOpticalColor()
// {
//   return optical.get_hue();
// }

void setIntakeHold (bool hold)
{
    holdBlock = hold;
}

