#include "subsystems/BrakeSubsystem.h"

BrakeSubsystem::BrakeSubsystem(WPI_TalonFX &rTalon, WPI_TalonFX &lTalon)
    : _rTalon(rTalon), _lTalon(lTalon) {
    // Implementation of subsystem constructor goes here.
     m_compressor.EnableAnalog(DriveConstants::kBrakeMinPressure,
                                DriveConstants::kBrakeMaxPressure);
     exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::Periodic() {}

void BrakeSubsystem::SimulationPeriodic() {}

void BrakeSubsystem::SetSoftware() {
    SetBrakeMode();
}

void BrakeSubsystem::UnsetSoftware() {
    UnsetBrakeMode();
}

void BrakeSubsystem::SetHardware() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::UnsetHardware() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kReverse);
}