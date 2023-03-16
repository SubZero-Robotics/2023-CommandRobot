#pragma once

#include <ctre/Phoenix.h>
#include <frc/Compressor.h>
#include <frc/DoubleSolenoid.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "Constants.h"

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

    void Set();
    void Unset();

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

    // frc::Compressor m_compressor{DriveConstants::kPenumaticsModuleID,
    //                              frc::PneumaticsModuleType::CTREPCM};
    // frc::DoubleSolenoid exampleDoublePH{
    //     DriveConstants::kPenumaticsModuleID, frc::PneumaticsModuleType::CTREPCM,
    //     DriveConstants::kSolenoidForwardChannel,
    //     DriveConstants::kSolenoidReverseChannel};
    WPI_TalonFX &_rTalon;
    WPI_TalonFX &_lTalon;
};