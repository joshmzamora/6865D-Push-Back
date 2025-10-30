#include "globals.h"
#include <cmath> // For std::sin, std::cos, std::abs, std::fmod


pros::Distance goalSensor(PORT_DISTANCE);


constexpr double TARGET_STOP_DISTANCE_MM = 200.0;
constexpr double MOVEMENT_DEADBAND_MM =
    20.0; // Stop if within +/- 20mm of target
constexpr double ANGLE_TOLERANCE_DEG =
    10.0; // How close the robot's angle must be to a target angle
constexpr double TRAVEL_DISTANCE =
    5.0; // Fixed small distance (in inches) to move each iteration

/**
 * @brief Checks if the robot's current angle is close to one of the 8 target
 * goal angles.
 *
 * @param current_theta_rad The robot's current heading in radians.
 * @return The target angle (in degrees) if within tolerance, otherwise -1.0.
 */
double find_target_angle(double current_theta_rad) {
  // Convert current theta from radians to degrees and normalize to [0, 360)
  double current_theta_deg = current_theta_rad * (180.0 / M_PI);
  current_theta_deg = std::fmod(current_theta_deg, 360.0);
  if (current_theta_deg < 0) {
    current_theta_deg += 360.0;
  }

  // Define ALL 8 goal angles (Long Goals and Middle Goals)
  const std::vector<double> target_angles = {
      0.0,  90.0,  180.0, 270.0, // Long Goals
      45.0, 135.0, 225.0, 315.0  // Diagonal Goals
  };

  // Check if the current angle is close to any target
  for (double target_angle : target_angles) {
    // Calculate the shortest angular distance (accounting for wrap-around)
    double angle_diff =
        std::fmod(target_angle - current_theta_deg + 180.0, 360.0) - 180.0;

    if (std::abs(angle_diff) <= ANGLE_TOLERANCE_DEG) {
      return target_angle; // Return the specific target angle found
    }
  }

  return -1.0; // No goal angle detected
}

/**
 * @brief Uses the distance sensor to continuously update a short-range
 * moveToPoint() command to approach the goal until 200mm away,
 * provided the robot is aligned with one of the 8 goal directions.
 */
void middleGoalSense() {
  lemlib::Pose current_pose = chassis.getPose();
  double current_theta = current_pose.theta;

  // 1. Check Alignment
  double detected_angle = find_target_angle(current_theta);

  if (detected_angle == -1.0) {
    pros::lcd::set_text(1, "Not aligned with a target goal angle. Exiting.");
    return;
  }

  pros::lcd::set_text(1, "Aligned at: " + std::to_string((int)detected_angle) +
                             " deg. Seeking...");

  // 2. Main movement loop
  while (true) {
    double current_distance_mm = goalSensor.get();

    // Convert the distance error from mm to the chassis's unit (typically
    // inches)
    double distance_error_in =
        (current_distance_mm - TARGET_STOP_DISTANCE_MM) / 25.4;

    // Check break conditions
    // Stop if within the deadband or if the sensor reading is invalid/out of
    // range (<= 0)
    if (current_distance_mm <= 0 ||
        std::abs(distance_error_in) * 25.4 <= MOVEMENT_DEADBAND_MM) {
      chassis.cancelMotion();

      if (current_distance_mm > 0) {
        pros::lcd::set_text(2, "GOAL REACHED! Dist: " +
                                   std::to_string((int)current_distance_mm) +
                                   "mm");
      } else {
        pros::lcd::set_text(2, "Sensor Error/Out of Range! 🛑 Stopping.");
      }

      break;
    }

    // 3. Calculate the new target point (A small step in the current direction)

    current_pose = chassis.getPose();

    // Calculate the change in X and Y using the current heading (theta is in
    // radians) dX = distance * cos(theta) dY = distance * sin(theta)
    double target_x =
        current_pose.x + TRAVEL_DISTANCE * std::cos(current_pose.theta);
    double target_y =
        current_pose.y + TRAVEL_DISTANCE * std::sin(current_pose.theta);

    // 4. Command the chassis to move to the calculated target point
    // The movement will continuously pull the robot forward towards (x', y')
    chassis.moveToPoint(target_x, target_y, 1000);

    pros::lcd::set_text(
        2, "Moving to (" + std::to_string((int)target_x) + ", " +
               std::to_string((int)target_y) +
               ") | Dist: " + std::to_string((int)current_distance_mm) + "mm");

    // Brief delay to allow the movement tasks to process
    pros::delay(50);
  }
}