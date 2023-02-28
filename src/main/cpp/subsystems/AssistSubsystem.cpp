#include <frc/smartdashboard/SmartDashboard.h>

#include "subsystems/AssistSubsystem.h"
#include "utils/DetectionParser.h"

AssistSubsystem::AssistSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void AssistSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here
}

// Limelight Stuff
frc::Pose3d AssistSubsystem::GetPosition() {
    nt::NetworkTableInstance::GetDefault().GetTable("limelight")->GetNumberArray("botpose",std::vector<double>(6));
}

std::vector<DetectionParser::DetectedObject> AssistSubsystem::GetObjects() {
    auto DetectionArray = frc::SmartDashboard::GetNumberArray("detections", {});
    auto DetectedObjects =
        DetectionParser::DetectedObject::parse(DetectionArray);
}

void AssistSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
