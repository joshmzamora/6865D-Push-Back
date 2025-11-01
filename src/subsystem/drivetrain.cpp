#include "drivetrain.h"
#include "auton/selector.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/misc.h"
#include "pros/rotation.hpp"
#include "subsystem/drivetrain.h"
#include <iostream>


pros::Controller controller(pros::E_CONTROLLER_MASTER);

/*
################
-  DRIVETRAIN  -
################
 */

pros::MotorGroup driveLeft({PORT_DRIVE_LEFT_FRONT, PORT_DRIVE_LEFT_TOP,
                            PORT_DRIVE_LEFT_BOTTOM},
                           pros::MotorGearset::blue);
pros::MotorGroup driveRight({PORT_DRIVE_RIGHT_FRONT, PORT_DRIVE_RIGHT_TOP,
                             PORT_DRIVE_RIGHT_BOTTOM},
                            pros::MotorGearset::blue);

// drivetrain settings
lemlib::Drivetrain drivetrain(
    &driveLeft,                 // left motor group
    &driveRight,                // right motor group
    13,                         // 13 inch track width
    lemlib::Omniwheel::NEW_325, // using new 4" omnis
    450,                        // drivetrain rpm is 450
    2 // chase power is 2. If we had traction wheels, it would have been 8
);

/*
###############
-  DRIVE PID  -
###############
*/

// lateral motion controller
lemlib::ControllerSettings
    linearController(9,  // proportional gain (kP)
                     0,   // integral gain (kI)
                     45,   // derivative gain (kD)
                     3,   // anti windup
                     1,   // small error range, in inches
                     100, // small error range timeout, in milliseconds
                     3,   // large error range, in inches
                     500, // large error range timeout, in milliseconds
                     20   // maximum acceleration (slew)
    );

// angular motion controller
lemlib::ControllerSettings
    angularController(2,   // proportional gain (kP)
                      0,   // integral gain (kI)
                      10,  // derivative gain (kD)
                      3,   // anti windup
                      1,   // small error range, in inches
                      100, // small error range timeout, in milliseconds
                      3,   // large error range, in inches
                      500, // large error range timeout, in milliseconds
                      0    // maximum acceleration (slew)
    );

/*
##############
-  ODOMETRY  -
##############
*/

// pros::ADIEncoder xEncoder = pros::ADIEncoder(PORT_ADI_XENCODER_TOP,
// PORT_ADI_XENCODER_BOTTOM, true); pros::ADIEncoder yEncoder =
// pros::ADIEncoder(PORT_ADI_YENCODER_TOP, PORT_ADI_YENCODER_BOTTOM, false);

// lemlib::TrackingWheel xTracker(&xEncoder, 2.75, -1.00393701, 1);
// lemlib::TrackingWheel yTracker(&yEncoder, 2.75, -0.0196850394, 1);

pros::Imu imu = pros::Imu(PORT_INERTIAL);
pros::Rotation horizontalTracker(PORT_ROTATION_HORIZONTAL);
pros::Rotation verticalTracker(PORT_ROTATION_VERTICAL);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontalTracker,
                                                lemlib::Omniwheel::NEW_2,
                                                4);
lemlib::TrackingWheel vertical_tracking_wheel(&verticalTracker,
                                              lemlib::Omniwheel::NEW_2,
                                              .875);

lemlib::OdomSensors odomSensors(
    &vertical_tracking_wheel, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
    &horizontal_tracking_wheel, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a
             // second one
    &imu     // inertial sensor
);

lemlib::Chassis chassis = lemlib::Chassis(drivetrain, linearController,
                                          angularController, odomSensors);

void arcadeDrive(double leftAxis, double rightAxis) {
  driveLeft.move(leftAxis);
  driveRight.move(rightAxis);
}

/*
###############
-  PID CONTROLLER -
###############
*/

void pidController() {
  
  // --- TUNING PARAMETERS ---
  const double KP_STEP = 0.5;
  const double KD_STEP = 5.0;

  // Get the CURRENT lateral controller settings by accessing the object
  // directly 'linearController' is defined globally in your provided code.
  lemlib::ControllerSettings &currentLinear =
      linearController; // Use reference (&) to modify the global object

  double newKp = currentLinear.kP;
  double newKd = currentLinear.kD;

  bool pidChanged = false;

  // --- KP ADJUSTMENTS (X and A buttons) ---
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
    newKp += KP_STEP;
    pidChanged = true;
    std::cout << "Lateral kP increased to: " << newKp << std::endl;
  } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
    newKp -= KP_STEP;
    pidChanged = true;
    std::cout << "Lateral kP decreased to: " << newKp << std::endl;
  }
  // --- KD ADJUSTMENTS (Y and B buttons) ---
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
    newKd += KD_STEP;
    pidChanged = true;
    std::cout << "Lateral kD increased to: " << newKd << std::endl;
  } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
    newKd -= KD_STEP;
    pidChanged = true;
    std::cout << "Lateral kD decreased to: " << newKd << std::endl;
  } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    chassis.setPose(0,0,0);
  } 
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    chassis.moveToPoint(0,24,10000);
    //chassis.turnToHeading(90, 10000);
  } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
    chassis.moveToPoint(0,0,10000, {.forwards=false});
    //chassis.turnToHeading(0, 10000);
    }

  // --- APPLY NEW SETTINGS ---
  if (pidChanged) {
    // 1. Update the global linearController struct directly
    currentLinear.kP = newKp;
    currentLinear.kD = newKd;

    // 2. The LemLib chassis object is typically initialized only once,
    // but by updating the 'linearController' object (which the chassis uses),
    // the changes should take effect immediately in older versions of the
    // library. If the changes don't take effect immediately, you may need to
    // re-initialize the chassis or re-pass the controller objects, depending on
    // your LemLib version.
  }
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    lemlib::ControllerSettings &lateralSettings = linearController;
        std::cout << "kP: " << lateralSettings.kP << std::endl;
    std::cout << "kD: " << lateralSettings.kD << std::endl;
    }
}

void odometryPosition() {
  while (true) {
    std::cout << "(" << chassis.getPose().x << ", " << chassis.getPose().y
              << ", " << chassis.getPose().theta << ")" << std::endl;
  }
}