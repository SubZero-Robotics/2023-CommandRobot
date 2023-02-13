/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <frc/smartdashboard/SmartDashboard.h>

#include "subsystems/DriveSubsystem.h"
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>

#include "Constants.h"

using namespace DriveConstants;

DriveSubsystem::DriveSubsystem()
{
    // Implementation of subsystem constructor goes here.
    // Stuff you want to happen once, when robot code starts running

    // Initialize each motor with MotionMagic settings
    // Made this a function since we do the same thing four times
    ConfigureMotor(&RightLead);
    ConfigureMotor(&RightFollow);
    ConfigureMotor(&LeftLead);
    ConfigureMotor(&LeftFollow);

    // Invert left side, since DifferentialDrive no longer does it for us
    LeftLead.SetInverted(true);
    LeftFollow.SetInverted(true);

    // Drive train motor grouping start
    RightFollow.Follow(RightLead);
    LeftFollow.Follow(LeftLead);

    // Start encoders at zero
    ResetEncoders();

    // setup for trajectories
    //trajectoryConfig = new frc::TrajectoryConfig(kMaxSpeed,
                                                 //kMaxAcceleration);
    // Create a voltage constraint to ensure we don't accelerate too fast
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            DriveConstants::ks, DriveConstants::kv, DriveConstants::ka),
        DriveConstants::kDriveKinematics, 10_V);

    // Add kinematics to ensure max speed is actually obeyed
    trajectoryConfig->SetKinematics(DriveConstants::kDriveKinematics);
    // Apply the voltage constraint
    trajectoryConfig->AddConstraint(autoVoltageConstraint);

    frc::SmartDashboard::PutNumber("KpDistance", -2);
}

void DriveSubsystem::DisabledInit()
{

}

void DriveSubsystem::TeleopInit()
{
    ConfigureMotor(&RightLead);
    ConfigureMotor(&RightFollow);
    ConfigureMotor(&LeftLead);
    ConfigureMotor(&LeftFollow);
}

void DriveSubsystem::SetCoast(WPI_TalonFX *_talon)
{
    _talon->SetNeutralMode(Coast);
}

void DriveSubsystem::Periodic()
{
    // Implementation of subsystem periodic method goes here.
    // Things that happen while robot is running */

    m_odometry.Update(m_gyro.GetRotation2d(),
                      units::meter_t(lEncoder * kEncoderDistancePerPulse),
                      units::meter_t(rEncoder * kEncoderDistancePerPulse));

    // Do things when first enabled or disabled
    if (frc::DriverStation::IsDisabled() != true && frc::DriverStation::IsAutonomousEnabled() == false)
    {
        if (!EnteredDisabled)
        {
            EnteredDisabled = true;
            EnteredEnabled = false;
            // set coast since we are disabled and not in auto
            DisabledInit(); // yep!
        }
    }
    else
    {
        if (!EnteredEnabled)
        {
            // we are enabled or in auto, so set brake mode
            EnteredEnabled = true;
            EnteredDisabled = false;
            TeleopInit();
        }
    }
}

void DriveSubsystem::ArcadeDrive(double fwd, double rot)
{
    double currentPercentage = fwd;
    if (abs(currentPercentage) > previousPercentage)
    { // speeding up, accel filter
        m_drive.ArcadeDrive(accelfilter.Calculate(currentPercentage), rot, true);
        decelfilter.Calculate(currentPercentage);
    }
    else
    { // slowing down, decel filter
        m_drive.ArcadeDrive(decelfilter.Calculate(currentPercentage), rot, true);
        accelfilter.Calculate(currentPercentage);
    }
    previousPercentage = abs(currentPercentage);
}

void DriveSubsystem::TankDriveVolts(units::volt_t left, units::volt_t right)
{
    LeftLead.SetVoltage(-left);
    RightLead.SetVoltage(-right);
    m_drive.Feed();
}

void DriveSubsystem::ResetEncoders()
{
    LOffset = -((LeftLead.GetSelectedSensorPosition() + LeftFollow.GetSelectedSensorPosition()) / 2.0);
    ROffset = -((RightLead.GetSelectedSensorPosition() + RightFollow.GetSelectedSensorPosition()) / 2.0);
}

// return the average of the two left encoders
double DriveSubsystem::GetLeftEncoder()
{
    return (-((LeftLead.GetSelectedSensorPosition() + LeftFollow.GetSelectedSensorPosition()) / 2.0) - LOffset);
}

// return the NEGATIVE average of the two right encoders
// Because it's inverted.  Maybe not needed?
double DriveSubsystem::GetRightEncoder()
{
    return (-((RightLead.GetSelectedSensorPosition() + RightFollow.GetSelectedSensorPosition()) / 2.0) - ROffset);
}

// return the average of left and right encoder sets, in feet
double DriveSubsystem::GetAverageEncoderDistance()
{
    return kEncoderDistancePerPulse * ((GetLeftEncoder() + GetRightEncoder()) / 2.0);
}

void DriveSubsystem::SetMaxOutput(double maxOutput)
{
    m_drive.SetMaxOutput(maxOutput);
}

units::degree_t DriveSubsystem::GetHeading()
{
    return m_gyro.GetRotation2d().Degrees();
}

double DriveSubsystem::GetTurnRate()
{
    return gyroRate * (kGyroReversed ? -1.0 : 1.0);
}

frc::Pose2d DriveSubsystem::GetPose()
{
    return m_odometry.GetPose();
}

frc::DifferentialDriveWheelSpeeds DriveSubsystem::GetWheelSpeeds()
{
    return {units::meters_per_second_t(-(RightLead.GetSelectedSensorVelocity() * 10 * kEncoderDistancePerPulse + RightFollow.GetSelectedSensorVelocity() * 10 * kEncoderDistancePerPulse) / 2.0),
            units::meters_per_second_t(-(LeftLead.GetSelectedSensorVelocity() * 10 * kEncoderDistancePerPulse + LeftFollow.GetSelectedSensorVelocity() * 10 * kEncoderDistancePerPulse) / 2.0)};
}

void DriveSubsystem::ResetOdometry(frc::Pose2d pose)
{
    ResetEncoders();
    m_odometry.ResetPosition(currentrobotAngle, 0_m, 0_m, pose);
}

void DriveSubsystem::ConfigureMotor(WPI_TalonFX *_talon)
{
    // Looking at this example:
    // https://github.com/CrossTheRoadElec/Phoenix-Examples-Languages/blob/master/C%2B%2B%20Talon%20FX%20(Falcon%20500)/MotionMagic/src/main/cpp/Robot.cpp
    // Sets up MotionMagic parameters inside the motor
    // The exact numbers need to be determined!
    // we're not playing with MotionMagic here though, this comment left in in case we do later

    // A reference to all the methods you could call in these motors is:
    // https://store.ctr-electronics.com/content/api/cpp/html/classctre_1_1phoenix_1_1motorcontrol_1_1can_1_1_talon_f_x.html

    // set motor to factory default each time the robot starts,
    // so that we don't have unexpected things left over
    _talon->ConfigFactoryDefault();

    // Choose the sensor we're using for PID 0 to be the built-in encoders
    // This should be the default anyway, but we'll be sure
    _talon->ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor,
                                         0,
                                         10);

    _talon->SetNeutralMode(Brake);

    /* Zero the sensor */
    _talon->SetSelectedSensorPosition(0, 0, 10);
}