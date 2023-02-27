#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "constants.h"
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

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    rev::CANSparkMax m_wristRotationMotor{ CANSparkMaxConstants::kWristRotationMotorID, rev::CANSparkMax::MotorType::kBrushless };
};