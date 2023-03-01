#include <subsystems/IntakeSubsystem.h>

IntakeSubsystem::IntakeSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void IntakeSubsystem::Periodic() {}

void IntakeSubsystem::SimulationPeriodic() {}

void IntakeSubsystem::Out() { m_intakeSpinnyBoy.Set(-1.0); }

void IntakeSubsystem::In() { m_intakeSpinnyBoy.Set(1.0); }

void IntakeSubsystem::Stop() { m_intakeSpinnyBoy.Set(0.0); }