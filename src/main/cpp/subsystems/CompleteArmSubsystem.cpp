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
        // TODO: Change angle based on game piece
        .AndThen(SetPose({.axis = m_wrist, .position = 90}))
        // Start intaking
        .AndThen(IntakeIn(m_intake).ToPtr())
        // Move forward slowly until we reach the cube
        .RaceWith(m_drive->GetArcadeDriveCommand(0.1, 0))
        // TODO: Change distance based on game piece
        .Until([this]() { return (m_lidar->GetDistance() <= 400 && m_lidar->IsValid()); })
        // Stop moving
        .AndThen(m_drive->GetArcadeDriveCommand(0, 0))
        // Intake a little bit more
        .AndThen(SpinIntakeTimer(m_intake, 750_ms, true).ToPtr())
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::TravelMode() {
    return SetMovementLED(MovementType::TravelMode)
        // Move wrist to 0
        .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extension to 0
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Move arm to 0
        .AndThen(SetPose({.axis = m_rotateArm, .position = 0}))
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHigh() {
    return SetMovementLED(MovementType::PlaceHigh)
        // Move wrist to 90 degrees and arm to 135 degrees
        .AndThen(SetPose({.axis = m_wrist, .position = 90}))
        .AlongWith(SetPose({.axis = m_rotateArm,
                            .position = ArmConstants::kRotationMaxDegree - 10}))
        // Move extension all the way out
        .AndThen(SetPose(
            {.axis = m_extension, .position = ArmConstants::kMaxArmDistance}))
        // Spit out piece
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        // Move wrist away so it doesn't snag
        .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extender back in
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Take a step back
        .AndThen(autos::StraightBack(m_drive, 40))
        // Get ready to move again
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::SetPose(ArmAxisPose pose) {
    Logging::logToStdOut("completearm",
                         "set pose to " + std::to_string(pose.position));

    return frc2::FunctionalCommand(
               [pose] { pose.axis->MoveToPosition(pose.position); },
               // The subsystem updates this for us
               [pose] {},
               // Stop all movement on end
               [pose](bool interrupted) { pose.axis->StopMovement(); },
               // Finish once no longer moving to a position
               [pose] { return !pose.axis->GetIsMovingToPosition(); },
               {pose.axis})
        .ToPtr();

    // ? Only allow arm to rotate downwards completely if extension is fully
    // ? retracted and wrist is not too low
}

bool CompleteArmSubsystem::GetIsMoving(ISingleAxisSubsystem* axis) const {
    return axis->GetIsMovingToPosition();
}

void CompleteArmSubsystem::Periodic() {}