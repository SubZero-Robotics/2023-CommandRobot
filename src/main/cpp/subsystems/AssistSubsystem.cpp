#include "subsystems/AssistSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include <vector>

#include "utils/DetectionParser.h"
#include "utils/Logging.h"

AssistSubsystem::AssistSubsystem() {
    // Implementation of subsystem constructor goes here.
}

void AssistSubsystem::Periodic() {
    // Implementation of subsystem periodic method goes here
}

// Limelight Stuff
frc::Pose2d AssistSubsystem::GetPosition() {
    auto rawbot = nt::NetworkTableInstance::GetDefault()
                      .GetTable("limelight")
                      ->GetNumberArray("botpose", std::vector<double>(6));
    auto x = (units::meter_t)rawbot[0];
    auto y = (units::meter_t)rawbot[1];
    auto yaw = rawbot[5];
    auto temp = frc::Pose2d{x, y, frc::Rotation2d(sin(yaw), cos(yaw))};
    Logging::logToSmartDashboard("AssistSub", temp, Logging::Level::INFO);
    return temp;
}

std::vector<DetectionParser::DetectedObject> AssistSubsystem::GetObjects() {
    auto DetectionArray = frc::SmartDashboard::GetNumberArray("detections", {});
    return DetectionParser::DetectedObject::parse(DetectionArray);
}

void AssistSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
