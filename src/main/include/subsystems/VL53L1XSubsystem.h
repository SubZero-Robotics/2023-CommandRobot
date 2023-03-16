#ifndef VL53L1X_SUBSYSTEM_H
#define VL53L1X_SUBSYSTEM_H

#include <frc/SPI.h>
#include <frc2/command/SubsystemBase.h>
#include <frc/smartdashboard/SmartDashboard.h>

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
        uint8_t sendbuf[8] = {1, 0, 0, 0, 0, 0, 0, 0};
        // Our empty receive buffer
        uint8_t receivebuf[8];
        // Perform the SPI transaction
        _spi->Transaction(sendbuf, receivebuf, sizeof(double));
        // memcpy the resulting receive buffer into _currentDistance
        memcpy(&_currentDistance, receivebuf, sizeof(double));
        auto vec = std::vector<uint8_t>(receivebuf, receivebuf + 8);
        frc::SmartDashboard::PutRaw("Lidar Data", vec);
        frc::SmartDashboard::PutNumber("Lidar MM", GetDistance());
    }

    inline double GetDistance() const { return _currentDistance; }

   private:
    std::unique_ptr<frc::SPI> _spi;
    double _currentDistance;
};

#endif