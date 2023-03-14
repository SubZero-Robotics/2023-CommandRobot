#include "subsystems/AssistSubsystem.h"

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
    Logging::logToSmartDashboard("AssistSubsystem",
                                 "X: " + std::to_string(x.value()) + "\n" +
                                     "Y: " + std::to_string(y.value()) + "\n" +
                                     "YAW: " + std::to_string(yaw) + "\n",
                                 Logging::Level::INFO);
    return temp;
}

std::vector<DetectionParser::DetectedObject> AssistSubsystem::GetObjects() {
    auto detections = frc::SmartDashboard::GetNumberArray("detections", {});

    Logging::logToStdOut("AssistSubsystem",
                         "Raw detection array has " +
                             std::to_string(detections.size()) + " elements",
                         Logging::Level::INFO);

    return DetectionParser::DetectedObject::parse(detections);
}

void AssistSubsystem::SimulationPeriodic() {
    // Implementation of subsystem simulation periodic method goes here.
}
