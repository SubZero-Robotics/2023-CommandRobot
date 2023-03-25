#include <subsystems/IntakeSubsystem.h>

IntakeSubsystem::IntakeSubsystem(LEDControllerSubsystem* subsystem)
    : m_ledSubsystem(subsystem) {}

void IntakeSubsystem::Periodic() {}

void IntakeSubsystem::SimulationPeriodic() {}

void IntakeSubsystem::Out() {
    if (m_ledSubsystem->getCurrentColor() ==
        LEDControllerSubsystem::Colors::Yellow) {
        m_intakeSpinnyBoy.Set(-ArmConstants::kOuttakeSpeed);
    } else {
        m_intakeSpinnyBoy.Set(ArmConstants::kOuttakeSpeed * 2);
    }
}

void IntakeSubsystem::In() {
    if (m_ledSubsystem->getCurrentColor() ==
        LEDControllerSubsystem::Colors::Purple) {
        m_intakeSpinnyBoy.Set(-ArmConstants::kIntakeSpeed);
    } else {
        m_intakeSpinnyBoy.Set(ArmConstants::kIntakeSpeed);
    }
}

void IntakeSubsystem::Stop() { m_intakeSpinnyBoy.Set(0.0); }