#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/DutyCycle.h>
#include <frc/DigitalInput.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/SubsystemBase.h>

#include <memory>

#include "Constants.h"

class VL53L1XController : frc2::SubsystemBase {
   public:
    VL53L1XController(int digitalPort) {
        _input = std::make_unique<frc::DigitalInput>(digitalPort);
        _dutyCycle = std::make_unique<frc::DutyCycle>(_input.get());
    }

    void Periodic() override {
        auto pwmRatio = _dutyCycle->GetOutput();
        _currentDistance = kLidarMaxDistance * pwmRatio;
        frc::SmartDashboard::PutNumber("Lidar MM", GetDistance());
    }

    inline double GetDistance() const { return (double)_currentDistance; }

   private:
    std::unique_ptr<frc::DigitalInput> _input;
    std::unique_ptr<frc::DutyCycle> _dutyCycle;
    uint16_t _currentDistance;
};

#endif