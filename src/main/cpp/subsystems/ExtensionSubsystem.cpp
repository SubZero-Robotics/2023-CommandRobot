#include "subsystems/ExtensionSubsystem.h"

ExtensionSubsystem::ExtensionSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void ExtensionSubsystem::Periodic() {
}

void ExtensionSubsystem::SimulationPeriodic() {
}

void ExtensionSubsystem::PercentOutput(double output) {
    m_extensionMotor.Set(output);
}