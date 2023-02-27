#include <frc/smartdashboard/SmartDashboard.h>

#include "subsystems/ExampleSubsystem.h"

ExampleSubsystem::ExampleSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void ExampleSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here.
    frc::SmartDashboard::GetNumberArray("detections", {});
}

void ExampleSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
