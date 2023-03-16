#include "subsystems/BrakeSubsystem.h"

BrakeSubsystem::BrakeSubsystem(WPI_TalonFX &rTalon, WPI_TalonFX &lTalon)
    : _rTalon(rTalon), _lTalon(lTalon) {
    // Implementation of subsystem constructor goes here.
    // m_compressor.EnableAnalog(DriveConstants::kBrakeMinPressure,
    //                           DriveConstants::kBrakeMaxPressure);
    // exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::Periodic() {}

void BrakeSubsystem::SimulationPeriodic() {}

void BrakeSubsystem::Set() {
    SetBrakeMode();
    // exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::Unset() {
    UnsetBrakeMode();
    // exampleDoublePH.Set(frc::DoubleSolenoid::Value::kReverse);
}