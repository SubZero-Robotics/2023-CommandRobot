/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

// #include <units/units.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/length.h>
#include <units/time.h>
#include <units/velocity.h>
#include <units/voltage.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>

/**
 * The Constants header provides a convenient place for teams to hold robot-wide
 * numerical or boolean constants.  This should not be used for any other
 * purpose.
 *
 * It is generally a good idea to place constants into subsystem- or
 * command-specific namespaces within this header, which can then be used where
 * they are needed.
 */

// The deadzone for the joystick
namespace DriveConstants
{
    constexpr auto kTrackWidth = 0.6096_m;
    constexpr int kEncoderCPR = 29020.16; // Counts Per Rotation. To get this, multiply gear ratio by encoder ticks. 14.17*2048 = 2920.16
    constexpr auto kWheelDiameterMeters = 0.1524_m;
    constexpr double kEncoderDistancePerPulse = 0.00001699323;

    // These characterization values MUST be determined either experimentally or
    // theoretically for *your* robot's drive. The Robot Characterization
    // Toolsuite provides a convenient tool for obtaining these values for your
    // robot.
    constexpr auto ks = 0.12305_V;
    constexpr auto kv = 6.4668 * 1_V * 1_s / 1_m;
    constexpr auto ka = 0.72235 * 1_V * 1_s * 1_s / 1_m;
    inline const frc::DifferentialDriveKinematics kDriveKinematics(kTrackWidth);

    constexpr double kPDriveVel = 7.9072;

    constexpr double kRamseteB = 2.0;
    constexpr double kRamseteZeta = 0.7;
}
constexpr double kDeadzone = 0;

// DriveSubsystem constants

// Turning constants
constexpr bool kGyroReversed = false;

constexpr double kStabilizationP = 0.12; // tune This to start oscillating.  0.2 made 0.667s periods .12
constexpr double kStabilizationI = 0.36; // Then tune this to stop the osolating .36
constexpr double kStabilizationD = 0.01; // Finaly tune this to fix final error  .01

constexpr double kTurnP = 0.10; // tune This to start oscillating.  0.2 made 0.667s periods .12
constexpr double kTurnI = 0.30; // Then tune this to stop the osolating .36
constexpr double kTurnD = 0.01; // Finaly tune this to fix final error  .01

constexpr auto kTurnTolerance = 5_deg;
constexpr auto kTurnRateTolerance = 10_deg_per_s;

constexpr auto kMaxSpeed = 1.5_mps;
constexpr auto kMaxAcceleration = 3_mps_sq;

constexpr auto kMaxTurnRate = 70_deg_per_s;
constexpr auto kMaxTurnAcceleration = 200_deg_per_s / 1_s;

// XboxController enums.  Since the Trigger stuff works on the base Joystick class, not the
// Xbox extension, these are undefined where we want to use them.  So, flat-out copied them
// here for reference
enum Button
{
    kBumperLeft = 5,
    kBumperRight = 6,
    kStickLeft = 9,
    kStickRight = 10,
    kA = 1,
    kB = 2,
    kX = 3,
    kY = 4,
    kBack = 7,
    kStart = 8
};

enum Axis
{
    kLeftX = 0,
    kRightX = 4,
    kLeftY = 1,
    kRightY = 5,
    kLeftTrigger = 2,
    kRightTrigger = 3
};
