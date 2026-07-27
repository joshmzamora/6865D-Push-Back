# 6865D Push Back Project

This document explains the programming ideas behind the robot code in this repository. It is meant to give more context than the README without treating the final code snapshot as the only important part of the project.

VEX programming is a mix of software, controls, electrical hardware, mechanical design, and testing. A routine can be correct on paper and still fail because a wheel slipped, a mechanism flexed, a sensor was mounted differently, or the robot entered a movement from the wrong position. The code has to account for those physical limits while remaining simple enough to tune between matches.

The repository is one example of that process. It includes working match code, testing utilities, alternate routines, and features that were still being developed. Some sections below describe general VEX programming concepts, while others point to how those ideas appear in this project.

## What robot code is responsible for

A competition robot program usually has several jobs running under one project:

- Read controller joysticks and buttons during driver control
- Command drivetrain and mechanism motors
- Control pneumatic pistons through the V5 Brain's ADI ports
- Read sensors such as an inertial sensor, distance sensors, motor encoders, and an optical sensor
- Track the robot's estimated position on the field
- Run autonomous movements in the correct order
- Coordinate multiple mechanisms without blocking the driver loop
- Provide debugging information through the terminal, controller, or Brain screen
- Stop safely when a movement times out or the driver takes control

The difficult part is rarely making one motor spin. The harder part is making every subsystem work together repeatedly under competition conditions.

## Software stack

This project was written in C++ using PROS and several libraries commonly used in VEX V5 development.

| Tool | Version in this project | Purpose |
| --- | --- | --- |
| PROS kernel | 4.1.0 | V5 project structure, hardware API, competition functions, controller input, and tasks |
| LemLib | 0.5.4 | Chassis control, PID-based movement, odometry, and autonomous motion commands |
| LVGL | 8.3.8 | Custom user interface on the V5 Brain screen |
| C++ | PROS toolchain | Project structure, subsystem logic, state management, and autonomous routines |

The exact versions matter because robotics libraries change over time. Code written for one version may need small API or configuration changes before it builds with a newer release.

## PROS and the competition program

PROS is a development environment and hardware API for the VEX V5 system. It uses normal C and C++, so the project can use functions, classes, headers, source files, tasks, standard output, and normal version-control workflows.

A PROS competition project is organized around several entry-point functions:

```cpp
void initialize();
void disabled();
void competition_initialize();
void autonomous();
void opcontrol();
```

Each function runs at a different point in the competition lifecycle.

### `initialize()`

`initialize()` runs when the program starts. It is normally used for sensor calibration, user-interface setup, background tasks, and other work that should happen before autonomous or driver control.

In this project, `initialize()` starts the Brain-screen interface, calibrates the LemLib chassis, and starts a task that reports the robot's estimated pose.

### `disabled()`

`disabled()` can be used for behavior that should occur while the robot is disabled by the field controller. It is empty in this project.

### `competition_initialize()`

`competition_initialize()` runs after the robot is connected to competition control and before autonomous begins. Teams often use it for autonomous selection or final setup. It is also empty in this project because the Brain interface is started earlier.

### `autonomous()`

`autonomous()` contains the routine that runs without driver input. At the end of the season, this project called `left()` directly. Other routines remained in the code so they could be selected manually by editing `src/main.cpp`.

### `opcontrol()`

`opcontrol()` is the driver-control loop. It repeatedly reads controller input, updates subsystem states, and sends new commands to the robot. This project runs the loop every 20 milliseconds.

A delay inside an infinite loop is important in PROS. It gives the real-time operating system time to schedule other tasks instead of allowing one loop to use all available processing time.

## Project organization

A small robot program can be written in one file, but that becomes difficult to maintain once the robot has several mechanisms and autonomous routines. This project separates the code by responsibility.

```text
include/
  auton/
  subsystem/
  util/
  globals.h

src/
  auton/
  subsystem/
  util/
  main.cpp
```

Header files declare functions, classes, states, and shared variables. Source files contain the actual implementation. This lets `main.cpp` call functions such as `runIntake()` or `runMacros()` without containing all of their internal logic.

The main groups are:

- `src/main.cpp` for PROS competition entry points
- `src/auton/` for autonomous routines and the Brain-screen selector
- `src/subsystem/` for the drivetrain and individual mechanisms
- `src/util/` for controller shaping, heading helpers, and experimental utilities
- `include/globals.h` for hardware port assignments

This separation also makes mechanical changes easier to handle. If a motor moves to a different V5 port, the port can be changed in one configuration file instead of searching through every autonomous routine.

## Hardware configuration in code

A VEX program needs an exact software representation of the robot's wiring and geometry. Motors and sensors are constructed with their V5 ports, while pneumatic devices are connected through ADI ports.

This project defines the ports in `include/globals.h`.

### Reversed motor ports

PROS allows a negative port number to indicate a reversed motor. For example:

```cpp
pros::MotorGroup driveLeft({-10, -8, -9}, pros::MotorGearset::blue);
pros::MotorGroup driveRight({19, 18, 20}, pros::MotorGearset::blue);
```

Reversing a motor in software is useful because motors mounted on opposite sides of a drivetrain often face opposite physical directions. A positive forward command should still move both sides of the robot forward.

### Motor groups

A drivetrain usually has several motors on each side. `pros::MotorGroup` lets the code treat each side as one unit. A movement command can then be sent to the entire left or right side instead of addressing every motor separately.

This project uses three motors on each side of the drivetrain and configures both groups with the blue gearset.

### ADI outputs and pneumatics

Pneumatic solenoids do not use Smart Ports. They connect to the Brain's three-wire ADI ports. Code sets an ADI digital output high or low to extend or retract a piston.

The robot included separate pneumatic controls for:

- The loader mechanism
- The middle-goal mechanism
- The left wing
- The ball lock

Treating each mechanism as its own subsystem keeps the physical action behind readable functions such as `engageLoaderMech()` or `disengageLeftWing()`.

## Driver control

Driver control needs to feel predictable. Fast response is useful, but a robot that reacts too aggressively around the joystick center can be hard to align.

### Controller input

The V5 controller provides analog joystick values from approximately -127 to 127 and digital button values that are either pressed or not pressed.

This project uses:

- Left joystick Y for forward and backward movement
- Right joystick X for turning
- Buttons for intake modes, loader control, wing control, and macros

The driver loop reads the inputs, processes them, and passes the results to LemLib's arcade-drive function.

### Arcade drive

Arcade drive combines one forward value and one turn value. A basic version can be represented as:

```text
left output  = forward + turn
right output = forward - turn
```

The library handles the final motor commands, but the idea is the same. The driver can move and turn using separate joystick axes.

### Joystick shaping

Raw joystick input is linear: moving the stick halfway gives roughly half output. Many drivers prefer a curve that gives finer control near the center while still allowing full speed near the edge.

This project applies an exponential-style mapping before calling `chassis.arcade()`. The forward and turn axes use different curve parameters because precise turning and straight driving do not always need the same response.

Joystick shaping does not make the drivetrain physically more accurate. It changes how easily the driver can command small corrections.

### Held buttons and new presses

There are two common ways to interpret a controller button:

- A held input remains active for as long as the button is pressed.
- A new-press input triggers once when the button changes from released to pressed.

The intake modes use held inputs because the mechanism should keep running while the driver holds a button. The loader uses a toggle based on a new press, because one press should change the mechanism state only once.

### Subsystem states

The intake and hood are controlled together through named states instead of scattered motor values. The code uses functions such as:

- `intakeIn()`
- `intakeOut()`
- `intakeMiddle()`
- `intakeTopGoal()`
- `stopIntake()`

Each function can coordinate the intake motor, hood motor, middle-goal mechanism, and ball lock. This is more reliable than asking the driver loop to remember every motor and piston command for each scoring mode.

The main benefit of state-based control is consistency. A top-goal command always places the related mechanisms in the same configuration.

## Feedback control and PID

Autonomous driving cannot rely only on running motors for a fixed amount of time. Battery voltage, wheel slip, carpet condition, robot weight, and starting position can all change the result.

Closed-loop control measures the robot's current state, compares it with a target, and changes the output based on the error.

A common controller is PID:

```text
output = kP(error) + kI(accumulated error) + kD(rate of error change)
```

### Proportional term

The proportional term reacts to the current error.

- Large error produces a larger correction.
- Small error produces a smaller correction.
- A value that is too low can make the robot respond slowly.
- A value that is too high can cause overshoot or oscillation.

### Integral term

The integral term reacts to error that remains over time. It can help overcome a consistent bias, but it can also build up too much correction if it is not limited. This is why many VEX drivetrain configurations use little or no integral gain unless it is needed.

The drivetrain settings in this project use a zero integral gain for both linear and angular motion.

### Derivative term

The derivative term reacts to how quickly the error is changing. It adds damping and can reduce overshoot, but too much derivative gain can make movement noisy or slow.

### Separate linear and angular controllers

Driving to a point and turning to a heading are different control problems. LemLib therefore uses separate controller settings for lateral and angular motion.

This project configured:

- A linear controller for movement across the field
- An angular controller for turning and heading correction
- Small-error and large-error exit ranges
- Time requirements for remaining inside those ranges
- Slew limiting for linear acceleration

The numbers are specific to the robot. They should not be copied directly to a different chassis without retuning.

### Exit conditions

A movement should not wait forever for mathematically perfect zero error. Real robots vibrate, coast, flex, and receive noisy sensor readings.

LemLib controller settings include error ranges and timeouts. A motion can be considered complete after it remains within an acceptable error for a specified amount of time. This prevents an autonomous routine from becoming stuck while trying to correct a very small error.

### Slew limiting

Slew limiting controls how quickly motor output is allowed to increase. Gradual acceleration can reduce wheel slip and help odometry remain more accurate, especially when the drivetrain uses motor encoders rather than independent tracking wheels.

This project uses a nonzero slew value for linear motion and disables it for angular motion.

### Minimum speed, maximum speed, and early exit

Individual LemLib movements can also set limits such as:

- `maxSpeed` to reduce the fastest allowed command
- `minSpeed` to keep the robot moving through a transition
- `earlyExitRange` to leave a movement before full settling
- `forwards` to approach a target with the front or back of the robot
- `direction` to force a clockwise or counterclockwise turn

These options are useful when an autonomous routine values smooth flow more than stopping exactly at every intermediate point.

## Odometry and pose tracking

Odometry estimates the robot's position by combining sensor measurements over time.

A pose normally contains:

- `x`: horizontal field position
- `y`: vertical field position
- `theta`: robot heading

LemLib exposes this as `chassis.getPose()` and allows the program to update it with `chassis.setPose()`.

### Sensors used for odometry

A VEX drivetrain can estimate movement using several sensor sources:

- Internal motor encoders
- Independent vertical tracking wheels
- Independent horizontal tracking wheels
- One or more inertial sensors

This project configures an inertial sensor and relies on drivetrain motor encoders for translation. Code for separate tracking wheels remained commented out.

That setup is mechanically simpler, but it is more sensitive to drivetrain wheel slip because the motor encoder still records rotation even when the wheel is sliding instead of moving the robot the expected distance.

### Inertial sensor

The V5 inertial sensor measures orientation and rotational motion. It is calibrated during startup before the robot begins using its heading.

The IMU helps the robot maintain a heading and track rotation more reliably than estimating every turn from left and right wheel motion alone.

Calibration requires the robot to remain still. Moving the robot during calibration can produce an incorrect heading reference.

### Starting pose

An autonomous routine needs to tell the odometry system where the robot begins. This is done with `chassis.setPose(x, y, theta)`.

The selected coordinate system must stay consistent throughout the routine. If a starting position or heading is entered incorrectly, every later field coordinate will also be wrong.

### Drift and physical error

Odometry is an estimate, not a field-position sensor. Error can grow because of:

- Wheel slip during acceleration or contact
- The robot being pushed by another object
- Incorrect wheel diameter or drivetrain RPM settings
- An inaccurate track-width measurement
- Mechanisms changing the robot's weight distribution
- A starting pose that was not set consistently
- Sensor noise or a loose sensor mount

A long autonomous routine usually needs more than one strategy. Odometry handles the general route, while known field geometry or distance sensors can be used to correct accumulated error.

## LemLib chassis configuration

LemLib needs a description of the physical drivetrain before it can convert motion targets into motor commands.

The drivetrain configuration in this project includes:

- Left and right PROS motor groups
- A 12-inch track width
- `NEW_325` omniwheel geometry
- A configured drivetrain speed of 450 RPM
- A horizontal-drift value of 2
- Linear and angular controller settings
- An IMU in the odometry sensor configuration

These values are part of the control model. If the real robot changes, the software configuration should be checked again.

### Track width

Track width describes the effective distance between the left and right drive sides. It affects how the library converts wheel movement into rotation. The best value is often an experimentally measured effective track width rather than only the physical distance between wheel centers.

### Wheel size and drivetrain speed

Wheel diameter and drivetrain RPM determine how far the robot is expected to travel for a given encoder rotation. A wrong value produces scaling error. The robot may repeatedly travel too far or not far enough even when the controller itself is tuned well.

### Horizontal drift

Horizontal drift represents how freely a drivetrain can move sideways while turning or following curved motion. A low-friction omniwheel drivetrain behaves differently from a traction-wheel drivetrain. LemLib uses this value when modeling chassis movement.

## LemLib movement commands

LemLib provides higher-level motion functions so an autonomous routine can describe field movement instead of directly managing left and right motor power.

### `moveToPoint()`

`moveToPoint(x, y, timeout)` drives toward an absolute field coordinate. The robot continuously updates its motor output using its estimated pose and the target point.

The project uses this command throughout the autonomous routines, often with options for reverse driving, speed limits, and early exit.

```cpp
chassis.moveToPoint(20, -22, 1000);
chassis.waitUntilDone();
```

### `turnToHeading()`

`turnToHeading(theta, timeout)` rotates the robot to an absolute heading. An absolute heading is useful because it does not depend on how accurately the previous turn finished.

### `swingToHeading()`

A swing turn rotates the robot while keeping one drivetrain side locked. It changes the center of rotation and can place a mechanism or corner of the robot more accurately in some field situations.

### `moveToPose()`

LemLib also supports moving to a target position and target heading together. This can produce a curved approach. This project mainly uses separate point and heading commands because they were easier to tune for the routes in the repository.

### Relative movement helpers

Field coordinates are useful, but some actions are easier to describe relative to the robot's current pose. This project includes helper functions that convert relative distances into a new absolute target.

For example, `drive_distance()` uses the current heading with sine and cosine to calculate the next field coordinate. That lets an autonomous routine request a movement forward or backward without manually calculating a new `x` and `y` every time.

### Asynchronous movement and waits

LemLib motion commands can run asynchronously. The program can then decide when to continue using functions such as:

- `waitUntilDone()` to wait for full completion
- `waitUntil(distance)` to continue after part of the movement
- `cancelAllMotions()` to stop active and queued chassis movements

This makes it possible to overlap actions. A loader can engage after the drivetrain has traveled part of a route rather than waiting for the robot to stop completely.

It also creates a responsibility: the code must know whether a movement is still running before issuing commands that conflict with it.

### Timeouts

Every motion includes a timeout. If the robot is blocked or cannot reach the target, the routine eventually continues instead of waiting forever.

A timeout is not a substitute for tuning. It is a final limit that keeps one failed motion from consuming the entire autonomous period.

## Autonomous routine design

An autonomous routine is more than a list of coordinates. It coordinates the drivetrain, intake, loader, wings, scoring mechanisms, and sensors in a limited amount of time.

A typical sequence may include:

1. Set the starting pose.
2. Place mechanisms in a known state.
3. Start the intake before driving toward an object.
4. Move to a field coordinate.
5. Trigger a loader or pneumatic mechanism partway through the movement.
6. Turn to a scoring heading.
7. Run a scoring mode for a measured amount of time.
8. Re-establish position using a wall or distance sensor.
9. Continue to the next objective.

The routines in this project show several different autonomous styles:

- `left()` and `right()` use field coordinates for match-side routes.
- `sawp()` is a separate autonomous route with its own sequence and local pose resets.
- `skills()` is a longer routine that repeats loading and scoring actions across more of the field.

The exact paths are specific to the game and robot. The transferable part is how the routine combines motion, sensing, and mechanisms.

### Starting from known conditions

Autonomous code assumes the robot begins in a known physical state. Before a match, the team needs to confirm:

- The robot is aligned with the intended starting position.
- Pneumatic mechanisms begin extended or retracted as expected.
- The intake does not already contain an object in an unexpected position.
- The IMU has completed calibration.
- The correct autonomous routine is active.

A small mismatch at the start can become a large positional error later.

### Mechanism timing

Some scoring actions still depend on timing because a motor may need to run long enough to move an object through a mechanism. The code uses `pros::delay()` for several of these actions.

Timed mechanism control is often reasonable when the mechanism has no dedicated position sensor. The routine should still leave enough margin for variation in object position, motor speed, and friction.

### Movement overlap

Stopping after every action is easy to understand but usually wastes time. A more competitive routine can begin a mechanism action while the chassis is still moving.

LemLib's `waitUntil()` allows this. The code can wait until the robot has traveled a certain distance, trigger the next subsystem, and then allow the movement to continue.

The tradeoff is complexity. Overlapping actions are faster but create more interactions to test.

## Distance sensors and field correction

The robot has distance sensors facing the front, back, left, and right. A VEX distance sensor reports the measured range in millimeters, so the code converts readings to inches before comparing them with field coordinates.

Distance sensors are useful when the robot is near a known wall or field object. Instead of trusting accumulated odometry alone, the program can compare the measured wall distance with the expected distance.

This project includes a `drive_distance_from_wall()` helper that:

- Reads the front or back distance sensor
- Rejects readings outside a basic valid range
- Converts millimeters to inches
- Calculates the remaining distance from the requested wall offset
- Commands a correction movement
- Stops when the reading is within an allowed range or the timeout expires

There is also experimental code that compares two sensor readings to estimate heading error. If two sensors mounted a known distance apart read different wall distances, the difference can be used to estimate the robot's angle relative to the wall.

Field correction is useful, but it needs defensive checks. A distance sensor can see a game object, another robot, or nothing at all. A bad reading should not immediately overwrite the entire robot pose without validation.

## Optical sensing and color sorting

The robot includes an optical sensor and early color-sorting code. An optical sensor can measure values such as hue and proximity, allowing the program to distinguish game-object colors under controlled lighting.

A reliable color-sorting system normally needs:

- A consistent sensor position
- A light source or sensor LED setting that reduces environmental variation
- A proximity check so empty space is not treated as an object
- Tested hue ranges for each alliance color
- A known delay between detection and the object reaching the rejection mechanism
- Logic that avoids repeatedly rejecting the same object

The color-sorting code in this repository should be read as experimentation rather than a complete production system.

## Mechanism coordination

A robot mechanism is rarely independent from the rest of the machine. An intake state may require a piston to move, a ball lock to engage, or another subsystem to remain disabled.

### Intake and hood

The intake and hood are represented by two PROS motor objects and controlled together through `setIntakeState()`.

The named modes coordinate more than motor direction:

- Intake mode disengages the middle-goal mechanism and engages the ball lock.
- Middle-goal mode changes the ball-lock state.
- Top-goal mode engages the middle-goal mechanism and ball lock.
- Stop mode returns the related mechanisms to a known condition.

This reduces the number of conflicting combinations the driver can accidentally create.

### Pneumatic mechanisms

Pneumatics are naturally state-based because a piston is normally in one of two positions. Separate engage and disengage functions make autonomous code easier to read and make the physical intent clear.

### Toggle logic

A toggle stores the previous state. Each new button press reverses it. Toggle code should use a new-press event rather than a continuously held input; otherwise one held press can switch the mechanism many times in a fraction of a second.

### Jam recovery

The intake code includes a basic jam-recovery function that briefly reverses the motors before returning to intake mode. Even a short recovery sequence is easier to reuse as a function than to duplicate inside multiple routines.

## Tasks and nonblocking macros

PROS runs on a real-time operating system and supports multiple tasks. Tasks allow a robot to keep reading driver input while another sequence is running.

This project uses a separate `pros::Task` for a wing-scoring macro. Pressing B starts the macro if another copy is not already active.

The important design choice is driver interruption. While the macro is running, the normal driver loop checks all joystick axes with a deadzone of 10. If the driver moves a joystick:

1. The macro task is removed.
2. Its dynamically allocated task object is deleted.
3. The stored task pointer is reset.
4. LemLib chassis motions are cancelled.
5. The controller rumbles to confirm the interruption.

This prevents the automated movement from continuing to fight the driver.

Killing a task alone does not guarantee that the drivetrain stops. The last motor or chassis command may remain active, which is why the code separately calls `cancelAllMotions()`.

### Task timing

Any repeated task should include a delay or block while waiting for an event. Without that pause, one task can consume too much processing time and interfere with the rest of the system.

### Shared state

Tasks create concurrency concerns. Two tasks should not send contradictory commands to the same mechanism at the same time. This project uses the `macroTask` pointer and subsystem states to reduce those conflicts, but larger systems may also use mutexes, task notifications, command queues, or a central scheduler.

## Brain-screen interface

The project uses LVGL to build a custom interface on the V5 Brain.

The interface code includes:

- Autonomous mode buttons
- Left-side, right-side, skills, and solo-AWP options
- Red and blue alliance state
- A long-press alliance toggle
- Auton, debug, and configuration tabs
- Live pose labels for `x`, `y`, and heading
- Custom button and border styles
- Event callbacks for selections and menu navigation

LVGL represents interface elements as objects referenced by `lv_obj_t` pointers. Buttons, labels, containers, and tab views can be styled and connected to event callbacks.

The interface demonstrates how a Brain screen can become a real testing tool instead of only displaying a logo.

One limitation is important: in the final snapshot, `autonomous()` still calls `left()` directly. The selector updates `selectedAuton`, but the selected value is not used to dispatch the autonomous routine. The user interface and autonomous execution would need to be connected before relying on the selector at a competition.

## Debugging and telemetry

Robot programming is difficult to debug because many problems only appear when code is running on the physical machine. Useful telemetry makes those problems easier to isolate.

### Terminal output

PROS can send `std::cout` or `printf()` output to a connected computer. Running `pros terminal` allows the programmer to watch sensor values, controller gains, state changes, and pose estimates.

This project prints the chassis pose repeatedly and includes additional output during wall correction and PID testing.

### Controller output

The controller can display short text and provide rumble feedback. The project prints pose information to the controller and uses a rumble when a driver macro is cancelled.

Controller display updates are slower than normal code execution, so they should not be treated like a high-frequency telemetry screen.

### Brain-screen debug page

The LVGL interface updates labels with the current pose. This is useful when testing on the field without keeping a computer connected.

### PID tuning controls

The drivetrain file contains a tuning utility that changes linear `kP` and `kD` values with controller buttons, prints the values, resets the pose, and commands test movements.

The purpose of an interactive tuner is to shorten the test cycle:

1. Change one gain.
2. Run the same movement.
3. Watch overshoot, settling, and repeatability.
4. Record the result.
5. Repeat with a controlled adjustment.

Only one variable should be changed at a time when possible. Otherwise it becomes difficult to know which change improved or harmed the motion.

## A practical testing process

A reliable autonomous routine is normally built in layers.

### 1. Verify hardware

Before tuning software, confirm that:

- Every motor is on the expected port.
- Reversed motors turn in the correct direction.
- Gearsets match the code.
- Sensors are secure and return reasonable values.
- Pneumatic tubing and solenoids are connected correctly.
- The drivetrain moves freely without severe mechanical drag.

Software cannot compensate for every mechanical problem.

### 2. Verify driver control

Test straight driving, turning, braking, intake direction, and every pneumatic button. This confirms that the basic hardware map is correct before autonomous testing begins.

### 3. Verify sensor signs and units

Move the robot by hand and watch the reported pose. Forward movement should change the expected coordinate, and clockwise or counterclockwise rotation should change heading in the expected direction.

Distance-sensor output should be converted consistently. PROS reports millimeters, while the autonomous coordinate system in this project uses inches.

### 4. Tune simple movements

Tune a straight drive and a turn before building a full route. Test the same distance in both directions and at different battery levels.

### 5. Build the route in sections

Add one movement or mechanism action at a time. A long routine is much easier to debug when the failure can be narrowed to the latest section.

### 6. Test from realistic starting variation

A routine that only works from one exact hand-placed position is fragile. Test small changes in starting position and angle to understand the available margin.

### 7. Test repeated runs

One successful run does not prove repeatability. Run the routine several times without changing the code or setup and record where the error begins to grow.

### 8. Test after mechanical changes

Changing wheel compression, gear ratios, mechanism weight, or sensor mounts can invalidate earlier tuning. Software should be retested whenever the physical robot changes significantly.

## Reliability and safety

Competition code should fail in a controlled way.

Useful safeguards include:

- Motion timeouts
- Valid ranges for sensor data
- Joystick deadzones
- Macro cancellation
- Known startup states for pneumatics
- Avoiding two tasks controlling the same motor at once
- Delays inside repeated loops
- Controller or screen feedback when a mode changes
- A final check of the selected autonomous routine

Readable code is also a reliability feature. During a competition, a programmer may need to understand and change a routine quickly. Named subsystem functions and centralized port definitions reduce the chance of editing the wrong command.

## Tradeoffs in this project

This repository reflects real development decisions rather than one ideal architecture.

### Motor encoders instead of active tracking wheels

Using drivetrain motor encoders reduces hardware complexity, but it makes position tracking more sensitive to wheel slip. The code includes commented tracking-wheel configuration, showing that a different odometry setup was considered or tested.

### Direct autonomous selection

Calling `left()` directly is simple and removes uncertainty about which routine will run. A finished on-screen selector is more flexible but adds another system that has to be tested.

### Timed scoring actions

Delays are straightforward when a mechanism has no position sensor. They are less adaptive than sensor-based completion and need enough margin for physical variation.

### Large autonomous functions

Keeping a complete route in one function makes the sequence easy to read from top to bottom. As routines grow, reusable actions and smaller route sections can reduce duplication and make testing easier.

### Experimental code in the main repository

Keeping tuning utilities and unfinished sensor ideas near the match code makes experimentation convenient. It also means the active path should be reviewed carefully before competition use.

## What the project demonstrates

The important part of VEX programming is the connection between code and the physical robot. This project demonstrates experience with:

- C++ project organization
- The PROS competition framework
- Motor groups and controller input
- Pneumatic subsystem control
- State-based mechanism logic
- Closed-loop drivetrain control
- PID tuning
- Odometry and pose tracking
- Coordinate-based autonomous movement
- Sensor-based field correction
- Multitasking and driver-interruptible macros
- LVGL interface development
- Terminal and on-robot debugging
- Iterative testing under mechanical constraints

The final code is a snapshot of the season. The larger engineering work was the repeated cycle of building, testing, finding the source of an error, adjusting either hardware or software, and testing again.

## Reading the repository

A useful order for reviewing the code is:

1. Start with `src/main.cpp` to see the competition flow.
2. Open `include/globals.h` to understand the hardware ports.
3. Read `src/subsystem/drivetrain.cpp` for the LemLib and PID configuration.
4. Read the subsystem files to see how mechanisms are represented as states and functions.
5. Read `src/auton/auton.cpp` to see how movements and mechanisms are sequenced.
6. Read `src/auton/selector.cpp` to see the LVGL Brain interface.
7. Review `src/util/` for controller shaping and experimental utilities.

## Terms used in this project

| Term | Meaning |
| --- | --- |
| ADI | Three-wire ports on the V5 Brain used for devices such as pneumatic solenoids |
| Autonomous | Competition period in which the robot runs without driver input |
| Closed-loop control | Control that uses measured error to adjust output |
| Deadzone | Small input range ignored to prevent joystick drift or sensor noise from causing movement |
| Encoder | Sensor that measures rotation; V5 motors contain internal encoders |
| Heading | Direction the robot is facing, usually measured in degrees |
| IME | Integrated motor encoder |
| IMU | Inertial measurement unit used to measure orientation and rotation |
| LVGL | Graphics library used to build the Brain-screen interface |
| Odometry | Estimation of robot position from motion sensors |
| PID | Proportional, integral, and derivative feedback controller |
| Pose | Robot position and heading, normally represented as `x`, `y`, and `theta` |
| PROS | C/C++ development environment and API for VEX V5 |
| Slew | Limit on how quickly the commanded output can change |
| Task | Independently scheduled unit of code managed by the PROS real-time operating system |
| Timeout | Maximum time allowed for an operation before the code continues |
| Tracking wheel | Independent encoder wheel used to measure robot translation |

## Official references

The following documentation provides the general technical background used in this project:

- [PROS first-time users guide](https://pros.cs.purdue.edu/v5/pros-4/new-users.html)
- [PROS project structure](https://pros.cs.purdue.edu/v5/pros-4/project-structure.html)
- [PROS controller tutorial](https://pros.cs.purdue.edu/v5/tutorials/topical/controller.html)
- [PROS multitasking tutorial](https://pros.cs.purdue.edu/v5/tutorials/topical/multitasking.html)
- [PROS C++ motor-group API](https://pros.cs.purdue.edu/v5/api/cpp/motor_groups.html)
- [PROS C++ inertial-sensor API](https://pros.cs.purdue.edu/v5/api/cpp/imu.html)
- [PROS C++ distance-sensor API](https://pros.cs.purdue.edu/v5/api/cpp/distance.html)
- [LemLib configuration guide](https://lemlib.readthedocs.io/en/stable/tutorials/2_configuration.html)
- [LemLib PID tuning guide](https://lemlib.readthedocs.io/en/stable/tutorials/4_pid_tuning.html)
- [LemLib lateral-motion guide](https://lemlib.readthedocs.io/en/stable/tutorials/6_lateral_motion.html)
- [LemLib chassis API](https://lemlib.readthedocs.io/en/stable/api/chassis.html)
- [LVGL 8.3 documentation](https://docs.lvgl.io/8.3/)
