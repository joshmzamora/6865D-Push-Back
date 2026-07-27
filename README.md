# 6865D Push Back

Robot code for VEX Team 6865D during the 2025-2026 Push Back season.

This repository contains the code we used and tuned throughout the season. It includes autonomous routines, driver controls, intake and pneumatic subsystems, a Brain-screen autonomous selector, and testing tools for odometry and PID tuning.

## Project setup

- PROS for the V5 project and competition framework
- LemLib for chassis movement, PID control, and odometry
- LVGL for the Brain-screen autonomous and debug menus
- IMU and distance sensors for positioning and autonomous corrections
- Separate subsystem files for the drivetrain, intake, loader, wing, ball lock, and middle-goal mechanism

## How the code runs

`initialize()` starts the Brain UI, calibrates the chassis, and starts a task that prints the robot's pose for debugging.

`autonomous()` is currently set to run `left()`. The repository also contains `right()`, `sawp()`, and `skills()` routines. The active routine should always be checked in `src/main.cpp` before a competition.

During driver control, `opcontrol()` handles macros, intake controls, the loader mechanism, the wing, and arcade drive.

## Driver controls

| Control | Action |
| --- | --- |
| Left joystick Y | Drive forward and backward |
| Right joystick X | Turn |
| B | Start the wing scoring macro |
| Any joystick movement | Cancel an active macro |
| R1 | Retract the left wing while held |
| R2 | Intake in |
| L1 | Intake out |
| L2 | Top-goal intake mode |
| Y | Toggle the loader mechanism |
| Right or Down | Middle-goal intake mode |

The wing macro runs in a separate `pros::Task`, so it does not freeze the rest of driver control. Moving either joystick cancels the task, stops the current chassis motion, and gives the driver controller feedback.

## Hardware map

Negative motor ports indicate that the motor is reversed in PROS.

| Component | Port(s) |
| --- | --- |
| Left drive | -10, -8, -9 |
| Right drive | 19, 18, 20 |
| Intake | 1 |
| Hood | -11 |
| Optical sensor | 15 |
| Inertial sensor | 16 |
| Left distance sensor | 7 |
| Right distance sensor | 17 |
| Front distance sensor | 14 |
| Back distance sensor | 5 |
| Loader mechanism | ADI A, ADI H |
| Middle-goal mechanism | ADI F |
| Left wing | ADI B |
| Ball lock | ADI G |

Port assignments are kept in `include/globals.h`.

## Code structure

| File | Purpose |
| --- | --- |
| `src/main.cpp` | PROS entry points, active autonomous routine, and driver loop |
| `src/auton/auton.cpp` | Autonomous routines, movement helpers, and macro task logic |
| `src/auton/selector.cpp` | Brain-screen autonomous selector and odometry debug display |
| `src/subsystem/drivetrain.cpp` | Motor groups, LemLib chassis setup, PID values, and odometry |
| `src/subsystem/intake.cpp` | Intake and hood state control |
| `src/subsystem/loaderMech.cpp` | Loader pneumatic control |
| `src/subsystem/wing.cpp` | Left-wing control |
| `src/subsystem/ball_lock.cpp` | Ball-lock pneumatic control |
| `src/subsystem/middleGoalMech.cpp` | Middle-goal pneumatic control |
| `src/util/util.cpp` | Joystick curves and heading helpers |
| `src/util/colorsort.cpp` | Color-sorting work in progress |

## Build and upload

Install the PROS toolchain, clone the repository, and run:

```bash
git clone https://github.com/joshmzamora/6865D-Push-Back.git
cd 6865D-Push-Back
pros make
pros upload
```

For terminal output and pose debugging:

```bash
pros terminal
```

## Notes

The repository contains both match-ready code and code that was still being tested during the season. Check the active autonomous routine, sensor configuration, and PID values before using it on a different robot or at a competition.
