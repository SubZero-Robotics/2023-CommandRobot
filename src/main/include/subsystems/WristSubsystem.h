#pragma once

#include <frc/DigitalInput.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include <iostream>

#include "Constants.h"
#include "rev/CANSparkMax.h"

class WristSubsystem : public frc2::SubsystemBase {
   public:
    WristSubsystem();

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;
    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    void Rotate(double);

    void ResetWristEncoder() { m_encoder.SetPosition(0); }

    void RunMotorHoming(double speed) { m_wristRotationMotor.Set(speed); }

    double GetWristDistanceDegree() {
        return m_encoder.GetPosition() * ArmConstants::kWristTicksPerDegree;
    }

    bool AtLimit() {
        return AtLimitSwitch();
        //        GetWristDistanceDegree() >= kWristDegreeLimit;
    }

    bool AtLimitSwitch() { return !m_limitSwitch.Get(); }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    frc::DigitalInput m_limitSwitch{ArmConstants::kWristLimitSwitchPort};

    rev::CANSparkMax m_wristRotationMotor{
        CANSparkMaxConstants::kWristRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_wristRotationMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor, 42);
};