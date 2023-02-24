#include "subsystems/VL53L1XController.h"

VL53L1XController::VL53L1XController(uint8_t inputPin) :
    // pin is pulled HIGH internally
    _input(std::make_unique<frc::DigitalInput>(inputPin)) {}

bool VL53L1XController::getDistanceValid() {
    // active LOW
    return !_input->Get();
}