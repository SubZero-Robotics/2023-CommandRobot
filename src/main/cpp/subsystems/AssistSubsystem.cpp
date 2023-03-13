#include "subsystems/AssistSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include <vector>

#include "utils/DetectionParser.h"

AssistSubsystem::AssistSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void AssistSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here
}

// Limelight Stuff
frc::Pose2d AssistSubsystem::GetPosition() {
    auto rawPose = nt::NetworkTableInstance::GetDefault()
                      .GetTable("limelight")
                      ->GetNumberArray("botpose", std::vector<double>(6));
    auto x = units::meter_t(rawPose[0]);
    auto y = units::meter_t(rawPose[1]);
    auto yaw = rawPose[5];
    return frc::Pose2d{x, y, frc::Rotation2d(units::degree_t(yaw))};
}

std::vector<DetectionParser::DetectedObject> AssistSubsystem::GetObjects() {
    auto DetectionArray = frc::SmartDashboard::GetNumberArray("detections", {});
    return DetectionParser::DetectedObject::parse(DetectionArray);
}

void AssistSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
