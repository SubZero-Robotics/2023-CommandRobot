#pragma once

#include <ctre\Phoenix.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>

#include "constants.h"
#include "subsystems/LEDControllerSubsystem.h"

using namespace CANSparkMaxConstants;

class IntakeSubsystem : public frc2::SubsystemBase {
   public:
    IntakeSubsystem(LEDControllerSubsystem* subsystem);

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
    void Stop();

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.
    LEDControllerSubsystem* m_ledSubsystem;
    WPI_TalonSRX m_intakeSpinnyBoy{kIntakeSpinnyBoyID};
};