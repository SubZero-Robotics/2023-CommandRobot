#include "subsystems/WristSubsystem.h"

WristSubsystem::WristSubsystem() {
    // Implementation of subsystem constructor goes here.

    m_wristRotationMotor.EnableSoftLimit(
        rev::CANSparkMax::SoftLimitDirection::kForward, true);

    // Default unit is number of rotations
    m_wristRotationMotor.SetSoftLimit(
        rev::CANSparkMax::SoftLimitDirection::kForward, kWristSoftLimit);
}

void WristSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here.
}

void WristSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}

void WristSubsystem::Rotate(double speed) { m_wristRotationMotor.Set(speed); }