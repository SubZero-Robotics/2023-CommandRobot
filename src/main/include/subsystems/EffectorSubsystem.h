#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "constants.h"
#include "rev/CANSparkMax.h"

class EffectorSubsystem : public frc2::SubsystemBase {
   public:
    EffectorSubsystem();

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

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    rev::CANSparkMax m_leadRotationMotor{
        CANSparkMaxConstants::kLeadRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_followRotationMotor{
        CANSparkMaxConstants::kFollowRotationMotorID,
        rev::CANSparkMax::MotorType::kBrushless};
};