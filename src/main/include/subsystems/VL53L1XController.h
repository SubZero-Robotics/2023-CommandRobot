#ifndef VL53L1X_CONTROLLER_H
#define VL53L1X_CONTROLLER_H

#include <frc/DigitalInput.h>
#include <memory>

class VL53L1XController {
    public:
    VL53L1XController(uint8_t inputPin);

    /**
     * @brief Get if lidar distance is in valid range
     * 
     * @return bool true if valid
     */
    bool getDistanceValid();

    private:
    std::unique_ptr<frc::DigitalInput> _input;
};

#endif