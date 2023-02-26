#include "subsystems/GripperSubsystem.h"

GripperSubsystem::GripperSubsystem() {
    // Implementation of subsystem constructor goes here.
    m_compressor.EnableAnalog(60_psi, 120_psi);
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void GripperSubsystem::Periodic() {
}

void GripperSubsystem::SimulationPeriodic() {
}

void GripperSubsystem::Forward() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void GripperSubsystem::Reverse() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kReverse);
}