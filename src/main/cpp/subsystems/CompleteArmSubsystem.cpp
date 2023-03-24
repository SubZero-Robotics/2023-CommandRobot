#include "subsystems/CompleteArmSubsystem.h"

frc2::CommandPtr CompleteArmSubsystem::Stop() {
    return frc2::InstantCommand(
               [this]() {
                   m_rotateArm->StopMovement();
                   m_wrist->StopMovement();
                   m_extension->StopMovement();
                   m_intake->Stop();
                   m_brake->UnsetBrakeMode();
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
                       case MovementType::PlaceHighCone:
                           m_leds->setOn();
                           // * Dark red
                           m_leds->setColor(142, 17, 52);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::PlaceMediumCone:
                           m_leds->setOn();
                           // * Rainbow
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::RGBFade);
                           break;
                       case MovementType::PlaceLowCone:
                           m_leds->setOn();
                           m_leds->setColor(17, 17, 170);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::PlaceHighCube:
                           m_leds->setOn();
                           // * Dark red
                           m_leds->setColor(142, 17, 52);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::PlaceMediumCube:
                           m_leds->setOn();
                           // * Rainbow
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::RGBFade);
                           break;
                       case MovementType::PlaceLowCube:
                           m_leds->setOn();
                           m_leds->setColor(17, 17, 170);
                           m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                        case MovementType::BrakeModeOn:
                            m_leds->setOn();
                            m_leds->setColor(0, 255, 0);
                            m_leds->setPattern(
                                LEDControllerSubsystem::PatternType::Chase);
                            break;
                        case MovementType::BrakeModeOff:
                            m_leds->setOn();
                            m_leds->setColor(255, 0, 0);
                            m_leds->setPattern(
                                LEDControllerSubsystem::PatternType::Chase);
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

        // Move forward slowly until we reach the cube
        .AndThen(autos::StraightBack(m_drive, -20, 0.1)
                     // TODO: Change distance based on game piece
                     // TODO: Make distance(s) a constant
                     .Until([this]() {
                         return (m_lidar->GetDistance() <= 600 &&
                                 m_lidar->IsValid());
                     })
                     .RaceWith(IntakeIn(m_intake).ToPtr()))

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
        .AndThen(SetPose({.axis = m_rotateArm,
                          .position = ArmConstants::kRotationHomeDegree}))
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHighCone() {
    return SetMovementLED(MovementType::PlaceHighCone)

        .AndThen(SetPose({.axis = m_extension, .position = 2}))
        // Move wrist to 90 degrees and arm to 135 degrees
        .AndThen(SetPose({.axis = m_rotateArm,
                          .position = ArmConstants::kRotationMaxDegree}))
        .AndThen(frc2::InstantCommand([this]() {
                     m_rotateArm->RunMotorExternal(-.07);
                 }).ToPtr())
        // Move extension all the way out
        .AndThen(SetPose(
            {.axis = m_extension, .position = ArmConstants::kMaxArmDistance}))

        .AndThen(SetPose(
            {.axis = m_wrist, .position = ArmConstants::kWristDegreeLimit}))
        // Spit out piece
        // .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        // Move wrist away so it doesn't snag
        // .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extender back in
        // .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Get ready to move again
        // .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceMediumCone() {
    return SetMovementLED(MovementType::PlaceMediumCone)

        .AndThen(SetPose({.axis = m_extension, .position = 2}))
        // Move wrist to 90 degrees and arm to 135 degrees
        .AndThen(SetPose({.axis = m_rotateArm, .position = 92}))
        .AndThen(frc2::InstantCommand([this]() {
                     m_rotateArm->RunMotorExternal(-.07);
                 }).ToPtr())
        // Move extension all the way out
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        .AndThen(SetPose({.axis = m_wrist, .position = 58}))
        // Spit out piece
        // .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        // Move wrist away so it doesn't snag
        // .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extender back in
        // .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Get ready to move again
        // .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceLowCone() {
    return SetMovementLED(MovementType::PlaceLowCone)
        .AndThen(TravelMode())
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHighCube() {
    return SetMovementLED(MovementType::PlaceHighCube)

        .AndThen(SetPose({.axis = m_extension, .position = 2}))
        // Move wrist to 90 degrees and arm to 135 degrees
        .AndThen(SetPose({.axis = m_rotateArm,
                          .position = ArmConstants::kRotationMaxDegree - 2}))
        .AndThen(frc2::InstantCommand([this]() {
                     m_rotateArm->RunMotorExternal(-.07);
                 }).ToPtr())
        // Move extension all the way out
        .AndThen(SetPose(
            {.axis = m_extension, .position = ArmConstants::kMaxArmDistance}))

        .AndThen(SetPose(
            {.axis = m_wrist, .position = ArmConstants::kWristDegreeLimit}))
        // Spit out piece
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        // Move wrist away so it doesn't snag
        .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extender back in
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Get ready to move again
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceMediumCube() {
    return SetMovementLED(MovementType::PlaceMediumCube)

        .AndThen(SetPose({.axis = m_extension, .position = 2}))
        // Move wrist to 90 degrees and arm to 135 degrees
        .AndThen(SetPose({.axis = m_rotateArm, .position = 110}))
        .AndThen(frc2::InstantCommand([this]() {
                     m_rotateArm->RunMotorExternal(-.07);
                 }).ToPtr())
        // Move extension all the way out
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        .AndThen(SetPose({.axis = m_wrist, .position = 95}))
        // Spit out piece
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        // Move wrist away so it doesn't snag
        .AndThen(SetPose({.axis = m_wrist, .position = 0}))
        // Move extender back in
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Get ready to move again
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceLowCube() {
    return SetMovementLED(MovementType::PlaceLowCube)
        .AndThen(TravelMode())
        .AndThen(SpinIntakeTimer(m_intake, 2000_ms, false).ToPtr())
        .AndThen(TravelMode())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHigh() {
    return m_leds->getCurrentColor() == LEDControllerSubsystem::Colors::Purple
               ? AutoPlaceHighCube().WithTimeout(25_s).AndThen(
                     SetMovementLED(MovementType::None))
               : AutoPlaceHighCone().WithTimeout(25_s).AndThen(
                     SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceMedium() {
    return m_leds->getCurrentColor() == LEDControllerSubsystem::Colors::Purple
               ? AutoPlaceMediumCube().WithTimeout(20_s).AndThen(
                     SetMovementLED(MovementType::None))
               : AutoPlaceMediumCone().WithTimeout(20_s).AndThen(
                     SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceLow() {
    return m_leds->getCurrentColor() == LEDControllerSubsystem::Colors::Purple
               ? AutoPlaceLowCube().WithTimeout(15_s).AndThen(
                     SetMovementLED(MovementType::None))
               : AutoPlaceLowCone().WithTimeout(15_s).AndThen(
                     SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::BrakeModeOn() {
    return SetMovementLED(MovementType::BrakeModeOn)
        .AndThen(frc2::InstantCommand([this]() {
            m_brake->SetSoftware();
            m_brake->SetHardware();
                 }).ToPtr())
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::BrakeModeOff() {
    return SetMovementLED(MovementType::BrakeModeOff)
        .AndThen(frc2::InstantCommand([this]() {
                    m_brake->UnsetHardware();
                     m_brake->UnsetBrakeMode();
                 }).ToPtr())
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