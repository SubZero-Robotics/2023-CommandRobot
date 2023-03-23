#include "subsystems/DutyCycleLidarSubsystem.h"

DutyCycleLidarSubsystem::DutyCycleLidarSubsystem(int dutyCyclePort, int validationPort) {
    _input = std::make_unique<frc::DigitalInput>(dutyCyclePort);
    _inputValid = std::make_unique<frc::DigitalInput>(validationPort);
    _dutyCycle = std::make_unique<frc::DutyCycle>(_input.get());
}

void DutyCycleLidarSubsystem::Periodic() {
    auto pwmRatio = _dutyCycle->GetOutput();
    _currentDistance = kLidarMaxDistance * pwmRatio;
    frc::SmartDashboard::PutNumber("Lidar MM", GetDistance());
    frc::SmartDashboard::PutBoolean("Lidar Valid", IsValid());
}