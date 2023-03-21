#include "subsystems/CompleteArmSubsystem.h"

frc2::CommandPtr CompleteArmSubsystem::Stop() {
    return frc2::InstantCommand(
               [this]() {
                   m_rotateArm->StopMovement();
                   m_wrist->StopMovement();
                   m_extension->StopMovement();
                   m_intake->Stop();
               },
               {this, m_rotateArm, m_wrist, m_extension, m_intake})
        .ToPtr()
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::SetMovementLED(MovementType type) {
    return frc2::InstantCommand(
               [this, type]() {
                   switch (type) {
                       case MovementType::None:
                           // * Current cone color
                           m_leds->setOn();
                           m_leds->setColor(m_leds->getCurrentColor());
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::SetAll,
                               true);
                           break;
                       case MovementType::HomeAll:
                           m_leds->setOn();
                           // * Light blue
                           m_leds->setColor(30, 30, 200);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::AutoIntake:
                           m_leds->setOn();
                           // * Orange
                           m_leds->setColor(200, 70, 0);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::TravelMode:
                           m_leds->setOn();
                           // * Green
                           m_leds->setColor(10, 230, 10);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::PlaceHigh:
                           m_leds->setOn();
                           // * Dark red
                           m_leds->setColor(142, 17, 52);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                   }
               },
               {this, m_leds})
        .ToPtr();
}

frc2::CommandPtr CompleteArmSubsystem::Home() {
    return SetMovementLED(MovementType::HomeAll)
        .AndThen(SetPose({.axis = m_rotateArm, .position = 100}))
        .AndThen(m_wrist->GetHomeCommand())
        .AlongWith(m_extension->GetHomeCommand())
        .AndThen(m_rotateArm->GetHomeCommand())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoIntake() {
    return TravelMode()
        .AndThen(SetMovementLED(MovementType::AutoIntake))
        .AndThen(SetPose({.axis = m_wrist, .position = 90}))
        .Until([this]() { return !m_wrist->GetIsMovingToPosition(); })
        // Start intaking
        .AndThen(IntakeIn(m_intake).ToPtr())
        // Move forward slowly until we reach the cube
        .RaceWith(m_drive->GetArcadeDriveCommand(0.1, 0))
        .Until([this]() { return m_lidar->GetDistance() <= 400; })
        // Stop moving
        .AndThen(m_drive->GetArcadeDriveCommand(0, 0))
        // ? Stop intaking- might need to keep this spinning for an extra second
        .AndThen(IntakeStop(m_intake).ToPtr())
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::TravelMode() {
    // Move wrist to 0
    return SetMovementLED(MovementType::TravelMode)
        .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        .Until([this]() { return !m_wrist->GetIsMovingToPosition(); })
        // Move extension to 0
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        .Until([this]() { return !m_extension->GetIsMovingToPosition(); })
        // Move arm to 0
        .AndThen(SetPose({.axis = m_rotateArm, .position = 0}))
        .Until([this]() { return !m_rotateArm->GetIsMovingToPosition(); })
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHigh() {
    return SetMovementLED(MovementType::PlaceHigh)
        .AndThen(SetPose({.axis = m_rotateArm, .position = ArmConstants::kRotationHomeDegree + 70})
                    .Until([this]() { return !m_rotateArm->GetIsMovingToPosition(); }))
        .AlongWith(SetPose({.axis = m_wrist, .position = 90})
                .Until([this]() { return !m_wrist->GetIsMovingToPosition(); }))
        // Move
        .AndThen(SetPose(
            {.axis = m_extension, .position = ArmConstants::kMaxArmDistance}))
        .Until([this]() { return !m_extension->GetIsMovingToPosition(); })
        // Spit out piece
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        .AndThen(SetPose({.axis = m_wrist, .position = 0})
                .Until([this]() { return !m_wrist->GetIsMovingToPosition(); }))
        .AndThen(m_drive->GetArcadeDriveCommand(-0.1, 0))
        // Stop moving
        .AndThen(m_drive->GetArcadeDriveCommand(0, 0))
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::SetPose(ArmAxisPose pose) {
    Logging::logToStdOut("completearm",
                         "set pose to " + std::to_string(pose.position));
    return frc2::InstantCommand(
               [pose]() { pose.axis->MoveToPosition(pose.position); },
               {pose.axis})
        .ToPtr();

    // ? Only allow arm to rotate downwards completely if extension is fully
    // ? retracted and wrist is not too low
}

bool CompleteArmSubsystem::GetIsMoving(ISingleAxisSubsystem* axis) const {
    return axis->GetIsMovingToPosition();
}

void CompleteArmSubsystem::Periodic() {}