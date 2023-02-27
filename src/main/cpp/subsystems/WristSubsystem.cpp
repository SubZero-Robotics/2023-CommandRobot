#include "subsystems/WristSubsystem.h"

WristSubsystem::WristSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void WristSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here.
}

void WristSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}

void WristSubsystem::Rotate(double speed) {
    m_wristRotationMotor.Set(speed);
}