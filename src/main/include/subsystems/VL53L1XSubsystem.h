#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/DigitalInput.h>
#include <frc/DutyCycle.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/SubsystemBase.h>

#include <memory>

#include "Constants.h"

class VL53L1XController : frc2::SubsystemBase {
   public:
    VL53L1XController(int dutyCyclePort, int validationPort) {
        _input = std::make_unique<frc::DigitalInput>(dutyCyclePort);
        _inputValid = std::make_unique<frc::DigitalInput>(validationPort);
        _dutyCycle = std::make_unique<frc::DutyCycle>(_input.get());
    }

    void Periodic() override {
        auto pwmRatio = _dutyCycle->GetOutput();
        _currentDistance = kLidarMaxDistance * pwmRatio;
        frc::SmartDashboard::PutNumber("Lidar MM", GetDistance());
        frc::SmartDashboard::PutBoolean("Lidar Valid", IsValid());
    }

    inline double GetDistance() const { return (double)_currentDistance; }

    inline double IsValid() const { return !_inputValid->Get(); }

   private:
    std::unique_ptr<frc::DigitalInput> _input;
    std::unique_ptr<frc::DigitalInput> _inputValid;
    std::unique_ptr<frc::DutyCycle> _dutyCycle;
    uint16_t _currentDistance;
};

#endif