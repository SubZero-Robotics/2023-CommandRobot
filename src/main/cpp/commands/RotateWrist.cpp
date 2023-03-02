// todo: grab axis of xbox joystick and move arm appropriately
// todo: home arm
// todo: get current angle
// todo: map encoder ticks to angle (constant)

#include "commands/RotateWrist.h"

#include <frc/smartdashboard/SmartDashboard.h>

RotateWrist::RotateWrist(WristSubsystem* subsystem,
                         std::function<double()> rotation)
    : m_effector{subsystem}, m_rotation{rotation} {
    // Register that this command requires the subsystem.
    AddRequirements(m_effector);
}

void RotateWrist::Execute() {
    double rotation = m_rotation();

    if (abs(rotation) < kDeadzone) rotation = 0.0;

    if (m_effector->AtLimitSwitch()) {
        if (rotation < 0) {
            m_effector->Rotate(rotation);
            m_effector->ResetWristEncoder();
        } else {
            m_effector->Rotate(0.0);
        }
    } else {
        m_effector->Rotate(rotation);
    }

    // if (m_effector->GetWristDistanceDegree() >= kWristDegreeLimit) {
    //     m_effector->Rotate(0.2);
    // }
} 