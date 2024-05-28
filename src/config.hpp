// config.hpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

// Motor Ports
const int FRONT_LEFT_MOTOR_PORT = 11;
const int MIDDLE_LEFT_MOTOR_PORT = 2;
const int BACK_LEFT_MOTOR_PORT = 20;
const int FRONT_RIGHT_MOTOR_PORT = -1;
const int MIDDLE_RIGHT_MOTOR_PORT = -5;
const int BACK_RIGHT_MOTOR_PORT = -10;

// Motor Gearing
const pros::v5::MotorGears FRONT_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears MIDDLE_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears BACK_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears FRONT_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears MIDDLE_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears BACK_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;

// Drivetrain Settings
const float WHEEL_DIAMETER = lemlib::Omniwheel::NEW_4;
const float TRACK_WIDTH = 10;
const float DRIVETRAIN_RPM = 360;
const float HORIZONTAL_DRIFT = 2;

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);



#endif // CONFIG_HPP