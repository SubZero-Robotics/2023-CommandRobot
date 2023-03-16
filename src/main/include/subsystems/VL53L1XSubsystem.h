#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/SPI.h>
#include <frc/smartdashboard/SmartDashboard.h>
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
        uint8_t sendbuf[2] = {1, 0};
        // Our empty receive buffer
        uint8_t receivebuf[2];
        // Perform the SPI transaction
        _spi->Transaction(sendbuf, receivebuf, sizeof(uint16_t));
        // memcpy the resulting receive buffer into _currentDistance
        memcpy(&_currentDistance, receivebuf, sizeof(uint16_t));
        auto vec = std::vector<uint8_t>(receivebuf, receivebuf + 2);
        frc::SmartDashboard::PutRaw("Lidar Data", vec);
        frc::SmartDashboard::PutNumber("Lidar MM", GetDistance());
    }

    inline double GetDistance() const { return (double)_currentDistance; }

   private:
    std::unique_ptr<frc::SPI> _spi;
    uint16_t _currentDistance;
};

#endif