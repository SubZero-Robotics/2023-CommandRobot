#include "subsystems/ExtensionSubsystem.h"

ExtensionSubsystem::ExtensionSubsystem() {
    // Implementation of subsystem constructor goes here.

    m_extensionMotor.EnableSoftLimit(
        rev::CANSparkMax::SoftLimitDirection::kForward, true);

    // Default unit is number of rotations
    m_extensionMotor.SetSoftLimit(
        rev::CANSparkMax::SoftLimitDirection::kForward,
        ArmConstants::kExtenderSoftLimit);

}

void ExtensionSubsystem::Periodic() {}

void ExtensionSubsystem::SimulationPeriodic() {}

void ExtensionSubsystem::PercentOutput(double output) {
    m_extensionMotor.Set(output);
}

void ExtensionSubsystem::Extend(double speed) { m_extensionMotor.Set(speed); }