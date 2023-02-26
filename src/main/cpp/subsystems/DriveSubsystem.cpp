/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include "subsystems/DriveSubsystem.h"

#include <frc/geometry/Rotation2d.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include <units/constants.h>

#include "Constants.h"

using namespace DriveConstants;

DriveSubsystem::DriveSubsystem(WPI_TalonFX& rightLead, WPI_TalonFX& rightFollow,
                               WPI_TalonFX& leftLead, WPI_TalonFX& leftFollow)
    : RightLead(rightLead),
      RightFollow(rightFollow),
      LeftLead(leftLead),
      LeftFollow(leftFollow),
      RightLeadSim(rightLead.GetSimCollection()),
      LeftLeadSim(leftLead.GetSimCollection()) {
    // Implementation of subsystem constructor goes here.
    // Stuff you want to happen once, when robot code starts running

    // Initialize each motor with MotionMagic settings
    // Made this a function since we do the same thing four times
    TeleopInit();

    rightEncoders = {.lead = &RightLead, .follow = &RightFollow};
    leftEncoders = {.lead = &LeftLead, .follow = &LeftFollow};

    InvertSide(leftEncoders);

    // Drive train motor grouping start
    RightFollow.Follow(RightLead);
    LeftFollow.Follow(LeftLead);

    // Start encoders at zero
    ResetEncoders();

    // setup for trajectories
    trajectoryConfig = new frc::TrajectoryConfig(kMaxSpeed, kMaxAcceleration);
    // Create a voltage constraint to ensure we don't accelerate too fast
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            DriveConstants::ks, DriveConstants::kv, DriveConstants::ka),
        DriveConstants::kDriveKinematics, 10_V);

    // Add kinematics to ensure max speed is actually obeyed
    trajectoryConfig->SetKinematics(DriveConstants::kDriveKinematics);
    // Apply the voltage constraint
    trajectoryConfig->AddConstraint(autoVoltageConstraint);
}

/**
 * Returns a 2D representation of the game field for dashboards.
 */
frc::Field2d& DriveSubsystem::GetField() { return field; }

void DriveSubsystem::DisabledInit() {}

void DriveSubsystem::TeleopInit() {
    ConfigureMotor(RightLead);
    ConfigureMotor(RightFollow);
    ConfigureMotor(LeftLead);
    ConfigureMotor(LeftFollow);
}

void DriveSubsystem::SetCoast(WPI_TalonFX* talon) {
    talon->SetNeutralMode(Coast);
}

void DriveSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here.
    // Things that happen while robot is running */

    currentrobotAngle = Get2dAngle();

    gyroAngle = ahrs.GetYaw();
    frc::SmartDashboard::PutNumber("gyroAngle", gyroAngle);

    gyroRate = ahrs.GetRate();
    frc::SmartDashboard::PutNumber("gyroRate", gyroRate);

    rEncoder = GetRightEncoder();
    lEncoder = GetLeftEncoder();

    m_odometry.Update(currentrobotAngle,
                      units::meter_t(lEncoder * kEncoderDistancePerPulse),
                      units::meter_t(rEncoder * kEncoderDistancePerPulse));

    frc::SmartDashboard::PutNumber("Pose X", (double)m_odometry.GetPose().X());
    frc::SmartDashboard::PutNumber("Pose Y", (double)m_odometry.GetPose().Y());
    frc::SmartDashboard::PutNumber(
        "Pose Degrees", (double)m_odometry.GetPose().Rotation().Degrees());

    // Do things when first enabled or disabled
    if (!frc::DriverStation::IsDisabled() &&
        !frc::DriverStation::IsAutonomousEnabled()) {
        if (EnteredEnabled) {
            EnteredEnabled = false;
            // set coast since we are disabled and not in auto
            DisabledInit();  // yep!
        }
    } else {
        if (!EnteredEnabled) {
            // we are enabled or in auto, so set brake mode
            EnteredEnabled = true;
            TeleopInit();
        }
    }
}

void DriveSubsystem::ArcadeDrive(double currentPercentage, double rotation) {
    if (abs(currentPercentage) >
        previousPercentage) {  // speeding up, accel filter
        m_drive.ArcadeDrive(accelfilter.Calculate(currentPercentage), rotation,
                            true);
        decelfilter.Calculate(currentPercentage);
    } else {  // slowing down, decel filter
        m_drive.ArcadeDrive(decelfilter.Calculate(currentPercentage), rotation,
                            true);
        accelfilter.Calculate(currentPercentage);
    }
    previousPercentage = abs(currentPercentage);
}

void DriveSubsystem::TankDriveVolts(units::volt_t left, units::volt_t right) {
    LeftLead.SetVoltage(-left);
    RightLead.SetVoltage(-right);
    m_drive.Feed();
}

void DriveSubsystem::ResetEncoders() {
    LOffset = -AverageEncoderPosition(leftEncoders);
    ROffset = -AverageEncoderPosition(rightEncoders);
}

// return the average of the two left encoders
double DriveSubsystem::GetLeftEncoder() {
    return GetEncoder(leftEncoders, LOffset);
}

// return the NEGATIVE average of the two right encoders
// Because it's inverted.  Maybe not needed?
double DriveSubsystem::GetRightEncoder() {
    return GetEncoder(rightEncoders, ROffset);
}

// return the average of left and right encoder sets, in feet
double DriveSubsystem::GetAverageEncoderDistance() {
    double leftEncoder = GetLeftEncoder();
    double rightEncoder = GetRightEncoder();

    frc::SmartDashboard::PutNumber("left", leftEncoder);
    frc::SmartDashboard::PutNumber("right", rightEncoder);

    return kEncoderDistancePerPulse * ((leftEncoder + rightEncoder) / 2.0);
}

void DriveSubsystem::SetMaxOutput(double maxOutput) {
    if (maxOutput < 0 || maxOutput > 100) return;
    m_drive.SetMaxOutput(maxOutput);
}

units::degree_t DriveSubsystem::GetHeading() {
    // make sure it fits in +/- 180.  Yaw does this, so should be ok.
    return units::degree_t((gyroAngle) * (kGyroReversed ? -1.0 : 1.0));
}

double DriveSubsystem::GetTurnRate() {
    return gyroRate * (kGyroReversed ? -1.0 : 1.0);
}

frc::Pose2d DriveSubsystem::GetPose() { return m_odometry.GetPose(); }

frc::DifferentialDriveWheelSpeeds DriveSubsystem::GetWheelSpeeds() {
    return {AverageEncoderVelocity(rightEncoders),
            AverageEncoderVelocity(leftEncoders)};
}

void DriveSubsystem::ResetOdometry(frc::Pose2d pose) {
    ResetEncoders();
    m_odometry.ResetPosition(currentrobotAngle, 0_m, 0_m, pose);
}

void DriveSubsystem::ConfigureMotor(WPI_TalonFX& talon) {
    // Looking at this example:
    // https://github.com/CrossTheRoadElec/Phoenix-Examples-Languages/blob/master/C%2B%2B%20Talon%20FX%20(Falcon%20500)/MotionMagic/src/main/cpp/Robot.cpp
    // Sets up MotionMagic parameters inside the motor
    // The exact numbers need to be determined!
    // we're not playing with MotionMagic here though, this comment left in in
    // case we do later

    // A reference to all the methods you could call in these motors is:
    // https://store.ctr-electronics.com/content/api/cpp/html/classctre_1_1phoenix_1_1motorcontrol_1_1can_1_1talon_f_x.html

    // set motor to factory default each time the robot starts,
    // so that we don't have unexpected things left over
    talon.ConfigFactoryDefault();

    // Choose the sensor we're using for PID 0 to be the built-in encoders
    // This should be the default anyway, but we'll be sure
    talon.ConfigSelectedFeedbackSensor(FeedbackDevice::IntegratedSensor, 0, 10);

    talon.SetNeutralMode(Brake);

    /* Zero the sensor */
    talon.SetSelectedSensorPosition(0, 0, 10);
}

double DriveSubsystem::AverageEncoderPosition(Encoders encoders) {
    auto encoderSum = encoders.lead->GetSelectedSensorPosition() +
                      encoders.follow->GetSelectedSensorPosition();

    return encoderSum / 2.0;
}

double DriveSubsystem::GetEncoder(Encoders encoders, double offset) {
    auto average = -AverageEncoderPosition(encoders);

    return average - offset;
}

units::meters_per_second_t DriveSubsystem::AverageEncoderVelocity(
    Encoders encoders) {
    auto scaledLead = encoders.lead->GetSelectedSensorVelocity() *
                      kVelocityScalingFactor * kEncoderDistancePerPulse;
    auto scaledFollow = encoders.follow->GetSelectedSensorVelocity() *
                        kVelocityScalingFactor * kEncoderDistancePerPulse;

    auto velocitySum = -(scaledLead + scaledFollow);

    return units::meters_per_second_t(velocitySum / 2.0);
}

void DriveSubsystem::InvertSide(Encoders encoders) {
    encoders.lead->SetInverted(true);
    encoders.follow->SetInverted(true);
}

units::degree_t DriveSubsystem::Get2dAngle() {
    return -(units::degree_t)ahrs.GetAngle();
}

void DriveSubsystem::SimulationPeriodic() {
    LeftLeadSim.SetBusVoltage(frc::RobotController::GetInputVoltage());
    RightLeadSim.SetBusVoltage(frc::RobotController::GetInputVoltage());

    driveSim.SetInputs(-LeftLeadSim.GetMotorOutputLeadVoltage() * 1_V,
                       RightLeadSim.GetMotorOutputLeadVoltage() * 1_V);

    driveSim.Update(20_ms);

    LeftLeadSim.SetIntegratedSensorRawPosition(
        DistanceToNativeUnits(driveSim.GetLeftPosition()));
    LeftLeadSim.SetIntegratedSensorVelocity(
        VelocityToNativeUnits(driveSim.GetLeftVelocity()));
    RightLeadSim.SetIntegratedSensorRawPosition(
        DistanceToNativeUnits(-driveSim.GetRightPosition()));
    RightLeadSim.SetIntegratedSensorVelocity(
        VelocityToNativeUnits(-driveSim.GetRightVelocity()));

#ifdef IS_SIMULATION
    m_odometry.Update(
        (frc::Rotation2d(units::degree_t(m_gyroSim.GetAngle()))),
        NativeUnitsToDistanceMeters(LeftLead.GetSelectedSensorPosition()),
        NativeUnitsToDistanceMeters(LeftLead.GetSelectedSensorPosition()));
    field.SetRobotPose(m_odometry.GetPose());
#endif
}

// Helper methods to convert between meters and native units

int DriveSubsystem::DistanceToNativeUnits(units::meter_t position) {
    double wheelRotations = position / (2 * units::constants::pi *
                                        DriveConstants::kWheelRadiusInches);
    double motorRotations = wheelRotations * DriveConstants::kSensorGearRatio;
    int sensorCounts = (int)(motorRotations * DriveConstants::kEncoderCPR);
    return sensorCounts;
}

int DriveSubsystem::VelocityToNativeUnits(units::meters_per_second_t velocity) {
    auto wheelRotationsPerSecond =
        velocity /
        (2 * units::constants::pi * DriveConstants::kWheelRadiusInches);
    auto motorRotationsPerSecond =
        wheelRotationsPerSecond * DriveConstants::kSensorGearRatio;
    double motorRotationsPer100ms = motorRotationsPerSecond * 1_s / 10;
    int sensorCountsPer100ms =
        (int)(motorRotationsPer100ms * DriveConstants::kEncoderCPR);
    return sensorCountsPer100ms;
}

units::meter_t DriveSubsystem::NativeUnitsToDistanceMeters(
    double sensorCounts) {
    double motorRotations = (double)sensorCounts / DriveConstants::kEncoderCPR;
    double wheelRotations = motorRotations / DriveConstants::kSensorGearRatio;
    units::meter_t position =
        wheelRotations *
        (2 * units::constants::pi * DriveConstants::kWheelRadiusInches);
    return position;
}