#pragma once

#include <frc/DigitalInput.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"
#include "rev/CANSparkMax.h"

class ExtensionSubsystem : public frc2::SubsystemBase {
   public:
    ExtensionSubsystem();

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    void Extend(double);

    void PercentOutput(double);

    void ResetEncoder() { m_encoder.SetPosition(0); }

    void RunMotorHoming(double speed) {
        // todo check direction for speed
        m_extensionMotor.Set(speed);
    }

    float GetExtenderDistanceIn() {
        return m_encoder.GetPosition() * ArmConstants::kTicksPerIn;
    }

    bool AtLimit() {
        return !m_limitSwitch.Get() ||
               GetExtenderDistanceIn() >= ArmConstants::kMaxArmDistanceIn;
    }

   private:
    frc::DigitalInput m_limitSwitch{ArmConstants::kExtenderLimitSwitchPort};

    rev::CANSparkMax m_extensionMotor{CANSparkMaxConstants::kExtensionMotorID,
                                      rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor,
        ArmConstants::kTicksPerMotorRotation);
    // 4096 is default ctre ticks
};