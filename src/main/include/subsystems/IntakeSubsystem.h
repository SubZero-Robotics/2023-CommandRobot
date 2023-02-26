#pragma once

#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

class IntakeSubsystem : public frc2::SubsystemBase {
   public:
    IntakeSubsystem();

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    void Out();
    void In();

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    frc::DoubleSolenoid exampleDoublePH{9, frc::PneumaticsModuleType::CTREPCM,
                                        7, 6};
};