#include <subsystems/IntakeSubsystem.h>

IntakeSubsystem::IntakeSubsystem(LEDControllerSubsystem* subsystem)
    : m_ledsubsystem(subsystem) {
    // Implementation of subsystem constructor goes here.
}

void IntakeSubsystem::Periodic() {}

void IntakeSubsystem::SimulationPeriodic() {}

void IntakeSubsystem::Out() {
    if (m_ledsubsystem->getCurrentColor() ==
        LEDControllerSubsystem::Colors::Yellow) {
        m_intakeSpinnyBoy.Set(-1.0);
    } else {
        m_intakeSpinnyBoy.Set(1.0);
    }
}

void IntakeSubsystem::In() {
    if (m_ledsubsystem->getCurrentColor() ==
        LEDControllerSubsystem::Colors::Purple) {
        m_intakeSpinnyBoy.Set(-1.0);
    } else {
        m_intakeSpinnyBoy.Set(1.0);
    }
}

void IntakeSubsystem::Stop() { m_intakeSpinnyBoy.Set(0.0); }