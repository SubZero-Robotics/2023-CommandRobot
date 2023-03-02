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

    if (abs(rotation) < kDeadzone) rotation = 0.0;

    if (m_effector->AtHome()) {
        if (rotation < 0){
            m_effector->ResetEncoder();
            m_effector->PercentOutput(rotation);
        } else {
            m_effector->PercentOutput(0.0);
        }
    } else if (m_effector->AtMax() ) {
        if (rotation > 0){
            m_effector->PercentOutput(rotation);
        } else {
            m_effector->PercentOutput(0.0);
        }
    } else {
        m_effector->PercentOutput(rotation);
    }
}