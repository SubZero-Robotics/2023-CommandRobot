// todo: map triggers to arm Gripper and retraction distance
// todo: maintain state for limits
// todo: homing
// todo: encoder ticks to distance mapping

#include "commands/GripperGrip.h"

Gripper::Gripper(GripperSubsystem* subsystem) : m_Gripper{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Gripper);
}

void Gripper::Execute() { m_Gripper->Forward(); }