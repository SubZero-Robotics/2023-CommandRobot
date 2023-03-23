#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/DigitalInput.h>
#include <frc/DutyCycle.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/SubsystemBase.h>

#include <memory>

#include "Constants.h"

class DutyCycleLidarSubsystem : frc2::SubsystemBase {
   public:
    DutyCycleLidarSubsystem(int dutyCyclePort, int validationPort);

    void Periodic() override;

    inline double GetDistance() const { return (double)_currentDistance; }

    inline double IsValid() const { return !_inputValid->Get(); }

   private:
    std::unique_ptr<frc::DigitalInput> _input;
    std::unique_ptr<frc::DigitalInput> _inputValid;
    std::unique_ptr<frc::DutyCycle> _dutyCycle;
    uint16_t _currentDistance;
};

#endif