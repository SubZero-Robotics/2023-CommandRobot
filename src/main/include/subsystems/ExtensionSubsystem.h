#pragma once

#include <frc/AnalogInput.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"
#include "rev/CANSparkMax.h"

class ExtensionSubsystem : public frc2::SubsystemBase {
   public:
    frc::AnalogInput m_limitSwitch{
        LimitSwitchConstants::kExtenderMagneticStopPort};

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

    void PercentOutput(double);

    bool AtLimit() {
        return m_limitSwitch.GetValue() >=
                   LimitSwitchConstants::kExtenderLimitSwitchThreshold ||
               ExtenderDistanceCm() >= kMaxArmDistance;
    }

    void ResetEncoder() { m_encoder.SetPosition(0); }

    void RunMotorHoming(double speed) {
        // todo check direction for speed
        m_extensionMotor.Set(speed);
    }

    float ExtederDistanceCm() { m_encoder.GetPosition() * kTicksPerCm; }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    // TODo: asign constants to the Can motors

    rev::CANSparkMax m_extensionMotor{CANSparkMaxConstants::kExtensionMotorID,
                                      rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kQuadrature, 4096);
};