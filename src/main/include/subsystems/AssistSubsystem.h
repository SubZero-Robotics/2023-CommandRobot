// haha mario oddessy assist mode

#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <networktables/NetworkTableInstance.h>

#include <cmath>
#include <vector>

#include "commands/IntakeInCommand.h"
#include "commands/SpinIntakeTimer.h"
#include "subsystems/CompleteArmSubsystem.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/IntakeSubsystem.h"
#include "utils/DetectionParser.h"
#include "utils/Logging.h"

class AssistSubsystem : public frc2::SubsystemBase {
   public:
    struct ArmPose {
        double rotationDegree;
        double armLengthIn;
    };

    AssistSubsystem(CompleteArmSubsystem* arm, LEDControllerSubsystem* leds,
                    IntakeSubsystem* intake, DriveSubsystem* drive);

    frc2::CommandPtr GetAutoPlaceCommand(PlacementLocation location);

    frc2::CommandPtr AutoIntake();

    frc::Pose2d GetPosition();

    std::vector<DetectionParser::DetectedObject> GetObjects();

    static ArmPose GetArmPoseFromDistance(double distanceIn,
                                          double poleHeightIn) {
        return {atan(poleHeightIn / distanceIn),
                hypot(distanceIn, poleHeightIn)};
    }

   private:
    frc2::CommandPtr GetPlacementHigh(PieceType piece);
    frc2::CommandPtr GetPlacementMiddle(PieceType piece);
    frc2::CommandPtr GetPlacementLow(PieceType piece);

    CompleteArmSubsystem* m_arm;
    LEDControllerSubsystem* m_leds;
    IntakeSubsystem* m_intake;
    DriveSubsystem* m_drive;
};