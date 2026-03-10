# 🤖 6865D | Push-Back 2025-2026
Official robot code for **VEX Robotics Team 6865D** for the 2025-2026 competition season, **Push-Back**.

This project is built using the **PROS Kernel** and utilizes **LemLib** for advanced motion profiling and PID-controlled movement.

---

## 🚀 Key Software Features

### 🧠 Intelligent Macro System
Our codebase features a non-blocking macro system that allows complex autonomous sequences to run during the Driver Control period without freezing the robot.
* **Task-Based Execution:** Macros run on a separate `pros::Task` thread.
* **Joystick Interrupt:** Using the `hasAnyInput()` method, the driver can instantly kill a running macro by moving the joysticks, ensuring total safety.
* **State Synchronization:** Uses `extern pros::Task* macroTask` and boolean states to prevent manual buttons from fighting the autonomous macros.

### 🏎️ Drivetrain & Motion
* **LemLib Integration:** High-accuracy movements using PID and Pure Pursuit.
* **Odometer Tracking:** Real-time position tracking via the Inertial Sensor and wheel encoders.
* **Arcade Drive:** Custom-mapped curves for precise driver control.

---

## 🛠️ Hardware Configuration

| Component | Port | Details |
| :--- | :--- | :--- |
| **Left Drive** | 1, 2, 3 | 600 RPM - Blue Cartridges |
| **Right Drive** | 4, 5, 6 | 600 RPM - Blue Cartridges |
| **Intake Motor** | 10 | 600 RPM - High Torque |
| **Left Wing** | ADI A | Pneumatic Piston |
| **Right Wing** | ADI B | Pneumatic Piston |
| **Inertial Sensor**| 15 | 3-Axis Gyroscope |

---

## 🎮 Controller Mappings

### Primary Driver (Master)
* **Left Joystick:** Forward/Backward movement.
* **Right Joystick:** Turning (Arcade style).
* **Button B:** Execute "Wing Left" Scoring Macro.
* **Button R1:** Manual Toggle - Left Wing.
* **Button L1:** Manual Toggle - Right Wing.
* **Any Joystick Input:** Interrupts and cancels currently active macro.

---

## 📂 File Structure

* `src/main.cpp`: Entry point for Autonomous, OpControl, and Global Task handling.
* `src/subsystem/wing.cpp`: Logic for pneumatic wing toggles and hardware state.
* `src/subsystem/intake.cpp`: Intake motor controls and macro functions.
* `include/main.h`: Global declarations (includes the `extern` Task pointers).

---

## 🛠️ Installation & Setup

1. **Prerequisites:** * Install the [PROS Toolchain](https://pros.cs.purdue.edu/).
   * Ensure [LemLib](https://lemlib.github.io/LemLib/) is included in your project template.

2. **Clone and Build:**
   ```bash
   git clone [https://github.com/YourUsername/6865D-PushBack.git](https://github.com/YourUsername/6865D-PushBack.git)
   pros make
   pros upload
