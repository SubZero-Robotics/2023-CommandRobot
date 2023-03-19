#include "subsystems/CompleteArmSubsystem.h"

frc2::CommandPtr CompleteArmSubsystem::Stop() {
    return frc2::InstantCommand([this]() {
               this->m_rotateArm->StopMovement();
               this->m_wrist->StopMovement();
               this->m_extension->StopMovement();
               this->m_intake->Stop();
           })
        .ToPtr()
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::SetMovementLED(MovementType type) {
    return frc2::InstantCommand(
               [this, type]() {
                   switch (type) {
                       case MovementType::None:
                           // * Current cone color
                           this->m_leds->setOn();
                           this->m_leds->setColor(
                               this->m_leds->getCurrentColor());
                           this->m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::SetAll,
                               true);
                           break;
                       case MovementType::HomeAll:
                           this->m_leds->setOn();
                           // * Light blue
                           this->m_leds->setColor(30, 30, 200);
                           this->m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::AutoIntake:
                           this->m_leds->setOn();
                           // * Orange
                           this->m_leds->setColor(200, 70, 0);
                           this->m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::TravelMode:
                           this->m_leds->setOn();
                           // * Green
                           this->m_leds->setColor(10, 230, 10);
                           this->m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                       case MovementType::PlaceHigh:
                           this->m_leds->setOn();
                           // * Dark red
                           this->m_leds->setColor(142, 17, 52);
                           this->m_leds->setPattern(
                               LEDControllerSubsystem::PatternType::Blink);
                           break;
                   }
               },
               {this})
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
        // Start intaking
        .AndThen(IntakeIn(m_intake).ToPtr())
        // Move forward slowly until we reach the cube
        .RaceWith(m_drive->GetArcadeDriveCommand(0.1, 0))
        .Until([this]() { return this->m_lidar->GetDistance() <= 400; })
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
        .Until([this]() { return !this->m_wrist->GetIsMovingToPosition(); })
        // Move extension to 0
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        .Until([this]() { return !this->m_extension->GetIsMovingToPosition(); })
        // Move arm to 0
        .AndThen(SetPose({.axis = m_rotateArm, .position = 0}))
        .Until([this]() { return !this->m_rotateArm->GetIsMovingToPosition(); })
        .AndThen(SetMovementLED(MovementType::None));
}

frc2::CommandPtr CompleteArmSubsystem::AutoPlaceHigh() {
    // TODO
}

frc2::CommandPtr CompleteArmSubsystem::SetPose(ArmAxisPose pose) {
    return frc2::InstantCommand(
               [pose]() { pose.axis->MoveToPosition(pose.position); })
        .ToPtr();

    // ? Only allow arm to rotate downwards completely if extension is fully
    // ? retracted and wrist is not too low
}

bool CompleteArmSubsystem::GetIsMoving(ISingleAxisSubsystem* axis) const {
    return axis->GetIsMovingToPosition();
}

void CompleteArmSubsystem::Periodic() {}