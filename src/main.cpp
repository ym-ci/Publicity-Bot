#include "main.h"
#include "lemlib/api.hpp"
#include "config.hpp"
#include "main.h"
#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.h"
#include "util/slewRateLimiter.hpp"
#include "util/triggerUtil.hpp"


// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motor Ports
// pros::Motor front_left_motor(FRONT_LEFT_MOTOR_PORT, FRONT_LEFT_MOTOR_GEAR); 
// pros::Motor middle_left_motor(MIDDLE_LEFT_MOTOR_PORT, MIDDLE_LEFT_MOTOR_GEAR); 
//pros::Motor back_left_motor(BACK_LEFT_MOTOR_PORT, BACK_LEFT_MOTOR_GEAR); 
//pros::Motor front_right_motor(FRONT_RIGHT_MOTOR_PORT, FRONT_RIGHT_MOTOR_GEAR); 
// pros::Motor middle_right_motor(MIDDLE_RIGHT_MOTOR_PORT, MIDDLE_RIGHT_MOTOR_GEAR); 
// pros::Motor back_right_motor(BACK_RIGHT_MOTOR_PORT, BACK_RIGHT_MOTOR_GEAR); 
// Drive Motor Group
pros::MotorGroup left_mg({FRONT_LEFT_MOTOR_PORT, BACK_LEFT_MOTOR_PORT});
pros::MotorGroup right_mg({FRONT_RIGHT_MOTOR_PORT, BACK_RIGHT_MOTOR_PORT});

// Drivetrain Settings

// drivetrain settings
lemlib::Drivetrain drivetrain(
	&left_mg, // left motor group
    &right_mg, // right motor group
    TRACK_WIDTH, // 10 inch track width
    WHEEL_DIAMETER, // using new 4" omnis
    DRIVETRAIN_RPM, // drivetrain rpm is 360
    HORIZONTAL_DRIFT// horizontal drift is 2 (for now)
);

// IMU
// pros::Imu imu_sensor(1);

// Odometry
lemlib::OdomSensors odom_sensors(
	nullptr,
	nullptr,
  	nullptr,
   	nullptr,
    nullptr
);

// Chassis
lemlib::Chassis chassis(
	drivetrain, // drivetrain
	lateral_controller, // lateral controller
	angular_controller, // angular controller
	odom_sensors // odom sensors
);

// Speed Toggle
Toggle speedToggle(
	[]() -> bool { return master.get_digital(DIGITAL_X); },
	TriggerMode::RISING_EDGE
);

// Arrtificial acceleration
SlewRateLimiter accel(0.1);

// Artificial acceleration toggle
Toggle accelToggle(
	[]() -> bool { return master.get_digital(DIGITAL_Y); },
	TriggerMode::RISING_EDGE
);


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Move the contoller out to variable
		double leftJoystick = master.get_analog(ANALOG_LEFT_Y);
		double rightJoystick = master.get_analog(ANALOG_RIGHT_Y);

		// Speed Multiplier
		double speedMultiplier = speedToggle.getState() ? 0.5 : 1.0;

		// Apply the speed multiplier
		leftJoystick *= speedMultiplier;
		rightJoystick *= speedMultiplier;

		// Artificial acceleration
		if (accelToggle.getState()) {
			leftJoystick = accel.slew(leftJoystick, leftJoystick);
			rightJoystick = accel.slew(rightJoystick, rightJoystick);
		}

		// Print the Analog Joystick values
		pros::lcd::print(0, "Left Joystick: %d", leftJoystick);
		pros::lcd::print(1, "Right Joystick: %d", rightJoystick);
		
		// Drive the robot
		chassis.arcade(leftJoystick, rightJoystick);

		pros::delay(20);                               // Run for 20 ms then update
	}
}