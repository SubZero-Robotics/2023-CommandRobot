// todo: grab axis of xbox joystick and move arm appropriately
// todo: home arm
// todo: get current angle
// todo: map encoder ticks to angle (constant)

#include "commands/MoveArm.h"

MoveArm::MoveArm(EffectorSubsystem* subsystem, std::function<double()> rotation)
    : m_effector{subsystem}, m_rotation{rotation} {
    // Register that this command requires the subsystem.
    AddRequirements(m_effector);
    Execute();
}

void MoveArm::Execute() {
    double rotation = m_rotation();

    m_effector->PercentOutput(rotation);
}