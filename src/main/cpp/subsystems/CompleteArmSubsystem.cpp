#include "subsystems/CompleteArmSubsystem.h"

void CompleteArmSubsystem::Stop() {
    // TODO
}

void CompleteArmSubsystem::SetPoses(WholeArmPose pose) {
    // TODO

    // Only allow arm to rotate downwards completely if extension is fully
    // retracted and wrist is not too low
}

bool CompleteArmSubsystem::GetIsMoving(ISingleAxisSubsystem* axis) const {
    // TODO
    return false;
}

void CompleteArmSubsystem::Periodic() {
    // TODO
}