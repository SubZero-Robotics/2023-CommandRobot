#include "commands/GripperStop.h"

GripperStop::GripperStop(GripperSubsystem* subsystem)
    : m_Gripper{subsystem} {
    // Register that this command requires the subsystem.
    AddRequirements(m_Gripper);
    Execute();
}

void GripperStop::Execute() {
    m_Gripper->Reverse();
}