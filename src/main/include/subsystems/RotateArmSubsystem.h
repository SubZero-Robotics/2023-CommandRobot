#pragma once

#include <frc/AnalogInput.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "constants.h"
#include "rev/CANSparkMax.h"

class RotateArmSubsystem : public frc2::SubsystemBase {
   public:
    RotateArmSubsystem();
    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    bool AtLimit() {
        return m_limitswitch.GetValue() >=
                   LimitSwitchConstants::kExtenderLimitSwitchThreshold ||
               ArmRotationDegree() >= kRotationMaxDegree;
    }

    void ResetEncoder() { m_encoder.SetPosition(0); }

    void RunMotorHoming(double speed) {
        // todo check direction for speed
        m_leadRotationMotor.Set(speed);
    }

    float ArmRotationDegree() {
        return (m_encoder.GetPosition() / kArmTicksPerDegree) + kRotationHomeDegree;
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
        CANSparkMaxConstants::kLeadRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_followRotationMotor{
        CANSparkMaxConstants::kFollowRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_leadRotationMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kQuadrature, 4096);

    frc::AnalogInput m_limitswitch{kRotationMagneticStopPort};
};