#ifndef VL53L1X_CONTROLLER_H
#define VL53L1X_CONTROLLER_H

#include <frc/SPI.h>

#include <memory>

#include "Constants.h"

class VL53L1XController {
   public:
    VL53L1XController(Frc::SPI::Port CSPin)
        : _spi(std::make_unique<frc::SPI>(CSPin)) {
        _spi->SetClockRate(kClockSpeedSPISlaveHZ);
        _spi->SetChipSelectActiveHigh();
    }

    /**
     * @brief Get if lidar distance is in valid range.
     * ! Valid when signal is LOW (active low logic)
     *
     * @return bool true if valid
     */
    bool getDistanceValid();

   private:
    std::unique_ptr<Frc::SPI> _spi;
};

#endif