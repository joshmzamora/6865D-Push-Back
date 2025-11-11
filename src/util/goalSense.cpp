#include "auton/selector.h"
#include "globals.h"
#include "main.h"
#include <algorithm>
#include <cmath>
#include <vector>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

pros::Distance goalSensor(PORT_DISTANCE);

constexpr double MM_TO_INCH = 25.4;
constexpr double TARGET_STOP_DISTANCE_MM = 200.0;
constexpr double ANGLE_TOLERANCE_DEG = 20.0;
constexpr double MAX_DISTANCE_MM = 900.0;
constexpr double MIN_DISTANCE_MM = 100.0;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double find_target_angle(double current_theta_rad) {
  double current_theta_deg = current_theta_rad * (180.0 / M_PI);

  current_theta_deg = std::fmod(current_theta_deg, 360.0);
  if (current_theta_deg < 0) {
    current_theta_deg += 360.0;
  }

  const std::vector<double> target_angles = {45.0, 135.0, 225.0, 315.0};
  for (double target_angle : target_angles) {
    double angle_diff =
        std::fmod(target_angle - current_theta_deg + 180.0, 360.0) - 180.0;
    if (std::abs(angle_diff) <= ANGLE_TOLERANCE_DEG) {
      return target_angle;
    }
  }
  return -1.0;
}

void middleGoalSense() {
  lemlib::Pose initial_pose = chassis.getPose();
  double current_theta_rad = initial_pose.theta;

  double detected_angle_deg = find_target_angle(current_theta_rad) ;

  if (detected_angle_deg == -1.0) {
    return;
  }

  double target_theta_rad = detected_angle_deg * (M_PI / 180.0);

  chassis.turnToHeading(target_theta_rad, 2000);

  double current_distance_mm = goalSensor.get_distance();

  // if (current_distance_mm < MIN_DISTANCE_MM ||
  //     current_distance_mm > MAX_DISTANCE_MM) {
  //   return;
  // }

  double distance_to_travel_mm = current_distance_mm - TARGET_STOP_DISTANCE_MM;
  double distance_to_travel_in = distance_to_travel_mm / MM_TO_INCH;
  double target_x;
  double target_y;
  if (currentAlliance == RED) {
    if (current_theta_rad == 315) {
      target_x =
          initial_pose.x + distance_to_travel_in * std::cos(target_theta_rad);
      target_y =
          initial_pose.y - distance_to_travel_in * std::sin(target_theta_rad);
    } else if (current_theta_rad == 225) {
      target_x =
          initial_pose.x + distance_to_travel_in * std::cos(target_theta_rad);
      target_y =
          initial_pose.y + distance_to_travel_in * std::sin(target_theta_rad);
    } else if (current_theta_rad == 135) {
      target_x =
          initial_pose.x - distance_to_travel_in * std::cos(target_theta_rad);
      target_y =
          initial_pose.y + distance_to_travel_in * std::sin(target_theta_rad);
    } else if (current_theta_rad == 45) {
      target_x =
          initial_pose.x - distance_to_travel_in * std::cos(target_theta_rad);
      target_y =
          initial_pose.y - distance_to_travel_in * std::sin(target_theta_rad);
    }
  } 
  chassis.moveToPoint(target_x, target_y, 4000, {.forwards = false});
}

void runGoalSense() {
  if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
    middleGoalSense();
  }
}