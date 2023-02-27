#include <frc/smartdashboard/SmartDashboard.h>

#include "subsystems/ExampleSubsystem.h"
#include "utils/DetectionParser.h"

ExampleSubsystem::ExampleSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void ExampleSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here.
    auto DetectionArray = frc::SmartDashboard::GetNumberArray("detections", {});
    auto DetectedObjects =
        DetectionParser::DetectedObject::parse(DetectionArray);
}

void ExampleSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
