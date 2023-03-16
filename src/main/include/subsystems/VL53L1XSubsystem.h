#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/SPI.h>
#include <frc2/command/SubsystemBase.h>

#include <memory>

#include "Constants.h"

class VL53L1XController : frc2::SubsystemBase {
   public:
    VL53L1XController(frc::SPI::Port csPort)
        : _spi(std::make_unique<frc::SPI>(csPort)) {
        _spi->SetClockRate(kClockSpeedSPISlaveHz);
        _spi->SetChipSelectActiveLow();
    }

    void Periodic() override {
        // Our "send" buffer
        // Our empty receive buffer
        // Perform the SPI transaction
        // memcpy the resulting receive buffer into _currentDistance
    }

    inline double GetDistance() const {
        return (double)_currentDistance;
    } 

   private:
    std::unique_ptr<frc::SPI> _spi;
    uint16_t _currentDistance;
};

#endif