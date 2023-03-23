#include "subsystems/CompleteArmSubsystem.h"

frc2::CommandPtr CompleteArmSubsystem::Stop() {
    return frc2::InstantCommand(
               [this]() {
                   m_rotateArm->StopMovement();
                   m_wrist->StopMovement();
                   m_extension->StopMovement();
               },
               {this, m_rotateArm, m_wrist, m_extension})
        .ToPtr();
}

frc2::CommandPtr CompleteArmSubsystem::Home() {
    return SetPose({.axis = m_rotateArm, .position = 100})
        .AndThen(m_wrist->GetHomeCommand())
        .AlongWith(m_extension->GetHomeCommand())
        .AndThen(m_rotateArm->GetHomeCommand());
}

frc2::CommandPtr CompleteArmSubsystem::SetPose(WholeArmPose pose) {
    // ! MUST extend 2 inches out BEFORE moving upwards or else it gets stuck
    if (!pose.reverseDirection) {
        return SetPose({.axis = m_extension, .position = 2})
            .AndThen(SetPose({.axis = m_rotateArm, .position = pose.arm}))
            .AndThen(frc2::InstantCommand([this]() {
                         m_rotateArm->RunMotorExternal(-.07);
                     }).ToPtr())
            .AndThen(SetPose({.axis = m_extension, .position = pose.extension}))
            .AndThen(SetPose({.axis = m_wrist, .position = pose.wrist}));
    }

    return SetPose({.axis = m_wrist, .position = pose.wrist})
        .AndThen(SetPose({.axis = m_extension, .position = pose.extension}))
        .AndThen(SetPose({.axis = m_rotateArm, .position = pose.arm}))
        .AndThen(frc2::InstantCommand([this]() {
                     m_rotateArm->RunMotorExternal(0);
                 }).ToPtr());
}

// frc2::CommandPtr CompleteArmSubsystem::AutoIntake() {
//     return TravelMode()
//         .AndThen(m_leds->SetMovementLED(MovementType::AutoIntake))
//         // TODO: Change angle based on game piece
//         .AndThen(SetPose({.axis = m_wrist, .position = 90}))

//         // Move forward slowly until we reach the cube
//         .AndThen(autos::StraightBack(m_drive, -20, 0.1)
//                      // TODO: Change distance based on game piece
//                      // TODO: Make distance(s) a constant
//                      .Until([this]() {
//                          return (m_lidar->GetDistance() <= 600 &&
//                                  m_lidar->IsValid());
//                      })
//                      .RaceWith(IntakeIn(m_intake).ToPtr()))

//         // Intake a little bit more
//         .AndThen(SpinIntakeTimer(m_intake, 750_ms, true).ToPtr())
//         .AndThen(TravelMode())
//         .AndThen(m_leds->SetMovementLED(MovementType::None));
// }

frc2::CommandPtr CompleteArmSubsystem::TravelMode() {
    return SetPose({.axis = m_wrist, .position = 0})
        // Move extension to 0
        .AndThen(SetPose({.axis = m_extension, .position = 0}))
        // Move arm to 0
        .AndThen(SetPose({.axis = m_rotateArm,
                          .position = ArmConstants::kRotationHomeDegree}));
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