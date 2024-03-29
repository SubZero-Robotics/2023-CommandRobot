/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

// #include <units/units.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>
#include <units/length.h>
#include <units/pressure.h>
#include <units/time.h>
#include <units/velocity.h>
#include <units/voltage.h>

#include <numbers>

#include "subsystems/ISingleAxisSubsystem.h"

// uncomment this for simulation
// #define IS_SIMULATION

/**
 * The Constants header provides a convenient place for teams to hold robot-wide
 * numerical or boolean constants.  This should not be used for any other
 * purpose.
 *
 * It is generally a good idea to place constants into subsystem- or
 * command-specific namespaces within this header, which can then be used where
 * they are needed.
 */

// XboxController enums.  Since the Trigger stuff works on the base Joystick
// class, not the Xbox extension, these are undefined where we want to use them.
// So, flat-out copied them here for reference
enum Button {
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

namespace Logging {
enum class Level { VERBOSE = 0, INFO, WARNING, ERROR };
// Don't log at levels below this one
constexpr auto kMinLogLevel = Level::VERBOSE;
}  // namespace Logging

enum Axis {
    kLeftX = 0,
    kRightX = 4,
    kLeftY = 1,
    kRightY = 5,
    kLeftTrigger = 2,
    kRightTrigger = 3
};

enum class MovementType {
    None,
    HomeAll,
    AutoIntake,
    TravelMode,
    PlaceHighCone,
    PlaceMediumCone,
    PlaceLowCone,
    PlaceHighCube,
    PlaceMediumCube,
    PlaceLowCube
};

enum class PieceType { Cone, Cube };

enum class PlacementLocation { Low, Middle, High };

struct ArmAxisPose {
    ISingleAxisSubsystem* axis;
    double position;
};

/**
 * @brief Always in this order: Arm > Extension > Wrist
 *
 * @param reverseDirection Set to true to move Wrist > Extension > Arm; useful
 * for retraction movements
 */
struct WholeArmPose {
    double arm;
    double extension;
    double wrist;
    bool reverseDirection;
};

// Arm Extension Constants

namespace ArmConstants {
constexpr double kMaxRPM = 4000;
// Extension Constants
constexpr double kExtensionGearRatio = 36;
constexpr double kInPerRotation = -3.5 / kExtensionGearRatio;
constexpr double kMaxArmDistance = 12;
constexpr int kExtenderLimitSwitchPort = 2;
constexpr double kExtenderStepSize = 1;

constexpr double kExtenderSetP = 0.018386;
constexpr double kExtenderSetI = 0.0075;
constexpr double kExtenderSetD = 0.00062724;
constexpr double kExtenderSetIZone = 0.01;
constexpr double kExtenderSetFF = 0.000015;

// Arm Rotation Constants
constexpr int kRotationLimitSwitchHomePort = 0;
constexpr int kRotationLimitSwitchMaxPort = 1;
constexpr double kRotationHomeDegree = 60;
constexpr double kRotationMaxDegree = 145;
constexpr int kArmSoftLimitForwardDegrees = 65;
constexpr double kArmGearRatio = 197.14;
constexpr double kArmStepSize = 4;
constexpr double kAntiGravityPercentage = -0.05;

constexpr double kArmRotationSetP = 0.0018386;
constexpr double kArmRotationSetI = 0.0075;
constexpr double kArmRotationSetD = 0.00062724;
constexpr double kArmRotationSetIZone = 0.01;
constexpr double kArmRotationSetFF = 0.000015;

// Motor Constants
constexpr int kTicksPerMotorRotation = 42;
constexpr double kArmTicksPerDegree =
    (kTicksPerMotorRotation * kArmGearRatio) / 360.0;
constexpr double kForwardRotationsSoftLimit =
    (kArmSoftLimitForwardDegrees * kArmTicksPerDegree) / kTicksPerMotorRotation;
constexpr int kReverseRotationsSoftLimit = 0;

// Homing Speeds
constexpr double kRotationHomingSpeed = .15;
constexpr double kExtenderHomingSpeed = .66;
constexpr double kWristHomingSpeed = .33;

// Intake Constants
constexpr double kIntakeSpeed = 1.0;
constexpr double kOuttakeSpeed = 0.33;

// Wrist Constants
constexpr int kWristLimitSwitchPort = 3;
constexpr int kWristGearRatio = 125;
constexpr auto kWristDegreeLimit = 120.0;
constexpr double kWristStepSize = 4;

constexpr double kWristSetP = 0.0018386;
constexpr double kWristSetI = 0.0075;
constexpr double kWristSetD = 0.00062724;
constexpr double kWristSetIZone = 0.01;
constexpr double kWristSetFF = 0.000015;
}  // namespace ArmConstants

// Motor IDs
namespace CANSparkMaxConstants {
constexpr int kExtensionMotorID = 50;
constexpr int kArmRotationLeadMotorID = 1;
constexpr int kArmRotationFollowMotorID = 3;
constexpr int kIntakeSpinnyBoyID = 17;
constexpr int kWristRotationMotorID = 15;
}  // namespace CANSparkMaxConstants

// The deadzone for the joystick
namespace DriveConstants {
constexpr auto kTrackWidth = 0.6096_m;
constexpr int kRightLeadMotorID = 8;
constexpr int kRightFollowMotorID = 7;
constexpr int kLeftLeadMotorID = 6;
constexpr int kLeftFollowMotorID = 5;
constexpr int kEncoderCPR = 22241.28;  // Counts Per Rotation. 2048 (talonfx
                                       // cpr) multiplied by gear ratio (10.86)
constexpr float kVelocityScalingFactor = 10;
constexpr double kWheelDiameterMeters = 0.15875;
constexpr auto kWheelRadiusInches = 6_in;
constexpr double kSensorGearRatio = 10.86;
constexpr double kWheelCircumfrenceMeters =
    kWheelDiameterMeters * std::numbers::pi;
constexpr double kEncoderDistancePerPulse =
    kWheelCircumfrenceMeters / kEncoderCPR;
constexpr double kPulsesPerMeter = 1 / kEncoderDistancePerPulse;
constexpr double kCurbRotation = 0.70;
constexpr double kPrecisionModeYCoEff = .45;
constexpr double kPrecisionModeXCoEff = .525;
constexpr auto kMaxDriveVelocity = 3_mps;

// These characterization values MUST be determined either experimentally or
// theoretically for *your* robot's drive. The Robot Characterization
// Toolsuite provides a convenient tool for obtaining these values for your
// robot.
constexpr auto ks = 0.073724_V;
constexpr auto kv = 2.4311 * 1_V * 1_s / 1_m;
constexpr auto ka = 0.24865 * 1_V * 1_s * 1_s / 1_m;
inline const frc::DifferentialDriveKinematics kDriveKinematics(kTrackWidth);

constexpr double kPDriveVel = 2.9104;

constexpr double kRamseteB = 2.0;
constexpr double kRamseteZeta = 0.7;
constexpr auto kBrakeMinPressure = 60_psi;
constexpr auto kBrakeMaxPressure = 120_psi;
constexpr auto kPenumaticsModuleID = 9;
constexpr auto kSolenoidForwardChannel = 1;
constexpr auto kSolenoidReverseChannel = 2;
}  // namespace DriveConstants
constexpr double kDeadzone = 0.0;

// DriveSubsystem constants

// Turning constants
constexpr bool kGyroReversed = false;

constexpr double kStabilizationP =
    0.12;  // tune This to start oscillating.  0.2 made 0.667s periods .12
constexpr double kStabilizationI =
    0.36;  // Then tune this to stop the osolating .36
constexpr double kStabilizationD =
    0.01;  // Finaly tune this to fix final error  .01

constexpr double kTurnP =
    0.0396;  // tune This to start oscillating.  0.2 made 0.667s periods .12
constexpr double kTurnI = 0.132;    // Then tune this to stop the osolating .36
constexpr double kTurnD = 0.00297;  // Finaly tune this to fix final error  .01

constexpr auto kTurnTolerance = 5_deg;
constexpr auto kTurnRateTolerance = 10_deg_per_s;

constexpr auto kMaxSpeed = 1.5_mps;
constexpr auto kMaxAcceleration = 3_mps_sq;

constexpr auto kMaxTurnRate = 70_deg_per_s;
constexpr auto kMaxTurnAcceleration = 200_deg_per_s / 1_s;

namespace AutoConstants {
constexpr double kStraightBackDivisor = 44.95;
constexpr double kAutoDriveDistanceInches = 66;
constexpr double kAutoBackupDistanceInches = 20;
constexpr double kAutoDriveSpeed = 0.5;

namespace PlaceHigh {
constexpr uint8_t r = 142;
constexpr uint8_t g = 17;
constexpr uint8_t b = 52;
constexpr uint32_t color = (r << 16) | (g << 8) | b;
constexpr auto timeout = 25_s;

namespace Cone {
constexpr WholeArmPose OutPose = {
    .arm = 145, .extension = 12, .wrist = 120, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cone

namespace Cube {
constexpr WholeArmPose OutPose = {
    .arm = 143, .extension = 12, .wrist = 120, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cube
}  // namespace PlaceHigh

namespace PlaceMiddle {
constexpr uint8_t r = 10;
constexpr uint8_t g = 230;
constexpr uint8_t b = 10;
constexpr uint32_t color = (r << 16) | (g << 8) | b;
constexpr auto timeout = 20_s;
namespace Cone {
constexpr WholeArmPose OutPose = {
    .arm = 92, .extension = 6, .wrist = 58, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cone

namespace Cube {
constexpr WholeArmPose OutPose = {
    .arm = 110, .extension = 0, .wrist = 95, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cube
}  // namespace PlaceMiddle

namespace PlaceLow {
constexpr uint8_t r = 30;
constexpr uint8_t g = 30;
constexpr uint8_t b = 200;
constexpr uint32_t color = (r << 16) | (g << 8) | b;
constexpr auto timeout = 15_s;
namespace Cone {
constexpr WholeArmPose OutPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cone

namespace Cube {
constexpr WholeArmPose OutPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = false};

constexpr WholeArmPose InPose = {
    .arm = 0, .extension = 0, .wrist = 0, .reverseDirection = true};
}  // namespace Cube
}  // namespace PlaceLow

constexpr auto kPlaceDuration = 2000_ms;

namespace Intake {

constexpr WholeArmPose IntakePose = {
    .arm = 60, .extension = 2, .wrist = 90, .reverseDirection = false};

constexpr auto kDuration = 750_ms;

constexpr double kDistance = -10;
constexpr double kSpeed = 0.1;
}  // namespace Intake

}  // namespace AutoConstants

constexpr uint8_t kLEDCotrollerSlaveAddress = 0x01;
constexpr int kLidarInputPin = 5;
constexpr int kLidarValidationPin = 6;
// 0 to 2000mm
constexpr uint16_t kLidarMaxDistance = 2000;
