// todo: grab axis of xbox joystick and move arm appropriately
// todo: home arm
// todo: get current angle
// todo: map encoder ticks to angle (constant)

#include "commands/RotateArm.h"

#include <frc/smartdashboard/SmartDashboard.h>

RotateArm::RotateArm(RotateArmSubsystem* subsystem,
                     std::function<double()> rotation)
    : m_effector{subsystem}, m_rotation{rotation} {
    // Register that this command requires the subsystem.
    AddRequirements(m_effector);
}

void RotateArm::Execute() {
    double rotation = m_rotation();

    if (!m_effector->AtHome()) {
        m_effector->PercentOutput(rotation);
    } else {
        m_effector->PercentOutput(-0.1);
        m_effector->ResetEncoder();
    }

    if (m_effector->AtLimit() ) {
        m_effector->PercentOutput(0.1);
    }
}