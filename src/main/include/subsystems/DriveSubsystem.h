// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <hal/SimDevice.h>
#include <hal/simulation/SimDeviceData.h>

#include <ctre/Phoenix.h>
#include <frc/AnalogInput.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTableEntry.h>
#include <frc/DriverStation.h>

#include <frc/drive/DifferentialDrive.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <AHRS.h>

#include "Constants.h"

#include <frc/geometry/Pose2d.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>

#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/filter/SlewRateLimiter.h>
#include <frc2/command/RamseteCommand.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>

class DriveSubsystem : public frc2::SubsystemBase
{
public:
    DriveSubsystem();

    void DisabledInit();

    void TeleopInit();

    void SetCoast(WPI_TalonFX *_talon);

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    // Subsystem methods go here.

    /**
     * Drives the robot using arcade controls.
     *
     * @param fwd the commanded forward movement
     * @param rot the commanded rotation
     */
    void ArcadeDrive(double fwd, double rot);

    /**
     * Controls each side of the drive directly with a voltage.
     *
     * @param left the commanded left output
     * @param right the commanded right output
     */
    void TankDriveVolts(units::volt_t left, units::volt_t right);

    /**
     * Resets the drive encoders to currently read a position of 0.
     */
    void ResetEncoders();

    /**
     * Gets the left drive encoder.
     *
     * @return the left drive encoder
     */
    double GetLeftEncoder();

    /**
     * Gets the right drive encoder.
     * @return the right drive encoder
     */
    double GetRightEncoder();
    /**
     * Gets the average distance traveled by both sides, in feet.
     * @return the average of left and right encoders, in feet
     */
    double GetAverageEncoderDistance();

    /**
     * Sets the max output of the drive.  Useful for scaling the drive to drive
     * more slowly.
     *
     * @param maxOutput the maximum output to which the drive will be constrained
     */
    void SetMaxOutput(double maxOutput);

    /**
     * Returns the heading of the robot.
     *
     * @return the robot's heading in degrees, from -180 to 180
     */
    units::degree_t GetHeading();

    /**
     * Returns the turn rate of the robot.
     *
     * @return The turn rate of the robot, in degrees per second
     */
    double GetTurnRate();

    /**
     * Returns the currently-estimated pose of the robot.
     *
     * @return The current pose of the robot
     */
    frc::Pose2d GetPose();

    /**
     * Returns the current wheel speeds of the robot.
     *
     * @return The current wheel speeds.
     */
    frc::DifferentialDriveWheelSpeeds GetWheelSpeeds();

    /**
     * Resets the odometry to the specified pose.
     *
     * @param pose The pose to which to set the odometry.
     */
    void ResetOdometry(frc::Pose2d pose);
    /**
     * set up a motor.  Call this in Init for each motor
     */
    void ConfigureMotor(WPI_TalonFX *_talon);

    frc::DifferentialDrive m_drive{RightLead, LeftLead};

private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    // right motor controllers
    WPI_TalonFX RightLead{12};
    WPI_TalonFX RightFollow{13};
    // left motor controllers
    WPI_TalonFX LeftLead{10};
    WPI_TalonFX LeftFollow{11};

    // The default (starting) values for the encoder
    double lEncoder = 0.0;
    double rEncoder = 0.0;
    double ROffset = 0.0;
    double LOffset = 0.0;

    double AverageEncoderDistance = 0.0;

    frc::SlewRateLimiter<units::scalar> decelfilter{2 / 1_s};
    frc::SlewRateLimiter<units::scalar> accelfilter{3 / 1_s};
    double previousPercentage = 0.0;

    // Odometry class for tracking robot pose
    frc::Rotation2d currentrobotAngle; // is zeroed by default
    frc::Pose2d currentRobotPose;      // is also zeroed by default
    // so now use those to initialize odemetry at zero too
    frc::DifferentialDriveOdometry m_odometry{currentrobotAngle, 0_m, 0_m, currentRobotPose};

    // navx
    double gyroAngle = 0.0; // What is the angle (degrees) from the gyro?
    double gyroRate = 0.0;  // What is angle change (deg/sec)

    AHRS m_gyro{frc::SPI::Port::kMXP};

    // TurnToAnglePID
    frc2::PIDController TurnToAngle{0.0396, 0.132, 0.00297};

    // pointer to network tables for limelight stuff
    std::shared_ptr<nt::NetworkTable> table;

    // The drive's config for trajectory
    frc::TrajectoryConfig *trajectoryConfig;

    // State transition variables
    bool EnteredEnabled = false;
    bool EnteredDisabled = false;
};