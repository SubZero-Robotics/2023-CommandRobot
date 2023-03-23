#include "subsystems/AssistSubsystem.h"

AssistSubsystem::AssistSubsystem(CompleteArmSubsystem* arm, LEDControllerSubsystem* leds, IntakeSubsystem* intake,
    DriveSubsystem* drive) :
    m_arm(arm),
    m_leds(leds),
    m_intake(intake),
    m_drive(drive) {
}

frc2::CommandPtr AssistSubsystem::GetAutoPlaceCommand(PlacementLocation location) {
    auto cmd = m_arm->TravelMode();

    auto piece = m_leds->getCurrentColor() == LEDControllerSubsystem::Colors::Purple ? PieceType::Cube : PieceType::Cone;

    switch (location) {
        case PlacementLocation::High:
            cmd = std::move(cmd)
                .AndThen(m_leds->SetMovementLED(AutoConstants::PlaceHigh::color, LEDControllerSubsystem::PatternType::SetAll))
                .AndThen(GetPlacementHigh(piece))
                .WithTimeout(AutoConstants::PlaceHigh::timeout);
            break;
        case PlacementLocation::Middle:
            cmd = std::move(cmd)
                .AndThen(m_leds->SetMovementLED(AutoConstants::PlaceMiddle::color, LEDControllerSubsystem::PatternType::SetAll))
                .AndThen(GetPlacementMiddle(piece))
                .WithTimeout(AutoConstants::PlaceMiddle::timeout);
            break;
        case PlacementLocation::Low:
            cmd = std::move(cmd)
                .AndThen(m_leds->SetMovementLED(AutoConstants::PlaceLow::color, LEDControllerSubsystem::PatternType::SetAll))
                .AndThen(GetPlacementLow(piece))
                .WithTimeout(AutoConstants::PlaceLow::timeout);
            break;
    }

    return std::move(cmd)
        // Spit out piece
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        .AndThen(m_arm->TravelMode())
        .AndThen(m_leds->DisplayCurrentColor());
}

frc2::CommandPtr AssistSubsystem::AutoIntake() {
    return m_arm->TravelMode()
        .AndThen(m_leds->SetMovementLED(0x00ea8604, LEDControllerSubsystem::PatternType::Blink))
        // TODO: Change angle based on game piece
        .AndThen(m_arm->SetPose({.arm=60,.extension=2,.wrist=90,.reverseDirection=false}))
        // Move forward slowly until we reach the cube
        .AndThen(autos::StraightBack(m_drive, -10, 0.1))
        .RaceWith(IntakeIn(m_intake).ToPtr())
        // Intake a little bit more
        .AndThen(SpinIntakeTimer(m_intake, 750_ms, true).ToPtr())
        .AndThen(m_arm->TravelMode())
        .AndThen(m_leds->DisplayCurrentColor());
}

// Limelight Stuff
frc::Pose2d AssistSubsystem::GetPosition() {
    auto rawbot = nt::NetworkTableInstance::GetDefault()
                      .GetTable("limelight")
                      ->GetNumberArray("botpose", std::vector<double>(6));
    auto x = (units::meter_t)rawbot[0];
    auto y = (units::meter_t)rawbot[1];
    auto yaw = rawbot[5];
    auto temp = frc::Pose2d{x, y, frc::Rotation2d(units::degree_t(yaw))};
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

frc2::CommandPtr AssistSubsystem::GetPlacementHigh(PieceType piece) {
    switch (piece) {
        case PieceType::Cone:
            return m_arm->SetPose(AutoConstants::PlaceHigh::Cone::OutPose);
        case PieceType::Cube:
            return m_arm->SetPose(AutoConstants::PlaceHigh::Cube::OutPose);
    }

    return m_arm->TravelMode();
}

frc2::CommandPtr AssistSubsystem::GetPlacementMiddle(PieceType piece) {
    switch (piece) {
        case PieceType::Cone:
            return m_arm->SetPose(AutoConstants::PlaceMiddle::Cone::OutPose);
        case PieceType::Cube:
            return m_arm->SetPose(AutoConstants::PlaceMiddle::Cube::OutPose);
    }

    return m_arm->TravelMode();
}

frc2::CommandPtr AssistSubsystem::GetPlacementLow(PieceType piece) {
    switch (piece) {
        case PieceType::Cone:
            return m_arm->SetPose(AutoConstants::PlaceLow::Cone::OutPose);
        case PieceType::Cube:
            return m_arm->SetPose(AutoConstants::PlaceLow::Cube::OutPose);
    }

    return m_arm->TravelMode();
}
