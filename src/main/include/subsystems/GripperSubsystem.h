#pragma once

#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

class GripperSubsystem : public frc2::SubsystemBase {
   public:
    GripperSubsystem();

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    void Forward();
    void Reverse();

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    frc::Compressor m_compressor{9, frc::PneumaticsModuleType::CTREPCM};
    frc::DoubleSolenoid exampleDoublePH{9, frc::PneumaticsModuleType::CTREPCM,
                                        0, 3};
};