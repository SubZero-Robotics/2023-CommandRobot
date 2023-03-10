#pragma once

#include <frc/DigitalInput.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include "subsystems/BaseSingleAxisSubsystem.h"
#include <units/angle.h>

#include "constants.h"
#include "rev/CANSparkMax.h"

class RotateArmSubsystem : public BaseSingleAxisSubsystem<rev::CANSparkMax, units::degree> {
   public:
    RotateArmSubsystem();
    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    bool AtHome() { return !m_limitswitchHome.Get(); }

    bool AtMax() { return !m_limitswitchMax.Get(); }

    void ResetEncoder() { m_encoder.SetPosition(0); }

    void RunMotorHoming(double speed) {
        // todo check direction for speed
        m_leadRotationMotor.Set(speed);
    }

    float ArmRotationDegree() {
        return (m_encoder.GetPosition() / ArmConstants::kArmTicksPerDegree) +
               ArmConstants::kRotationHomeDegree;
    }

    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    void PercentOutput(double);

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    rev::CANSparkMax m_leadRotationMotor{
        CANSparkMaxConstants::kArmRotationLeadMotorID,
        rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_followRotationMotor{
        CANSparkMaxConstants::kArmRotationFollowMotorID,
        rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_leadRotationMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor, 42);
    rev::SparkMaxAbsoluteEncoder m_enc;

    frc::DigitalInput m_limitswitchHome{
        ArmConstants::kRotationLimitSwitchHomePort};
    frc::DigitalInput m_limitswitchMax{
        ArmConstants::kRotationLimitSwitchMaxPort};

    SingleAxisConfig m_config = {
        BaseSingleAxisSubsystem::AxisType::Rotational,
        frc::ProfiledPIDController<units::degree>(1.3, 0.0, 0.7,
        frc::TrapezoidProfile<units::degree>::Constraints(1.75_mps, 0.75_mps_sq)
        ),
        60,
        125,
        360,
        BaseSingleAxisSubsystem::ConfigConstants::MOTOR_DIRECTION_NORMAL,
        0,
        1,
        9,
        0.1
    };
};