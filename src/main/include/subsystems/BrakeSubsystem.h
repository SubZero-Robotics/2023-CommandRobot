#pragma once

#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <ctre/Phoenix.h>

class BrakeSubsystem : public frc2::SubsystemBase {
   public:
    BrakeSubsystem(WPI_TalonFX &rTalon, WPI_TalonFX &lTalon);

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

    void SetBrakeMode() {
      _lTalon.SetNeutralMode(Brake);
      _rTalon.SetNeutralMode(Brake);
    }

    void UnsetBrakeMode() {
      _lTalon.SetNeutralMode(Coast);
      _rTalon.SetNeutralMode(Coast);
    }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.

    // frc::Compressor m_compressor{9, frc::PneumaticsModuleType::CTREPCM};
    // frc::DoubleSolenoid exampleDoublePH{9, frc::PneumaticsModuleType::CTREPCM,
    //                                     1, 2};
   WPI_TalonFX &_rTalon;
   WPI_TalonFX &_lTalon;
};