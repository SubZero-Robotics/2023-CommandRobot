#include <subsystems/IntakeSubsystem.h>

IntakeSubsystem::IntakeSubsystem() {
    // Implementation of subsystem constructor goes here.
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void IntakeSubsystem::Periodic() {}

void IntakeSubsystem::SimulationPeriodic() {}

void IntakeSubsystem::Out() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kForward);
}

void IntakeSubsystem::In() {
    exampleDoublePH.Set(frc::DoubleSolenoid::Value::kReverse);
}