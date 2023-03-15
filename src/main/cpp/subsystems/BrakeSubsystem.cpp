#include "subsystems/BrakeSubsystem.h"

BrakeSubsystem::BrakeSubsystem(WPI_TalonFX &rTalon, WPI_TalonFX &lTalon) : _rTalon(rTalon), _lTalon(lTalon) {
    // Implementation of subsystem constructor goes here.
    // m_compressor.EnableAnalog(60_psi, 120_psi);
    // exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::Periodic() {}

void BrakeSubsystem::SimulationPeriodic() {}

void BrakeSubsystem::Forward() {
    //exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void BrakeSubsystem::Reverse() {
    //exampleDoublePH.Set(frc::DoubleSolenoid::Value::kReverse);
}