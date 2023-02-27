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
    Execute();
}

void RotateWrist::Execute() {
    double rotation = m_rotation();

    m_effector->Rotate(rotation);
}