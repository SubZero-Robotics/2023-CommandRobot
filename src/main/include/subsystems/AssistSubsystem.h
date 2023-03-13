// haha mario oddessy assist mode

#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <networktables/NetworkTableInstance.h>

#include <vector>
#include <units/math.h>
#include <units/angle.h>
#include <units/length.h>

#include "utils/DetectionParser.h"

class AssistSubsystem : public frc2::SubsystemBase {
   public:
    struct ArmPose {
        units::degree_t rotationDegree;
        units::inch_t armLengthIn;
    };

    AssistSubsystem();

    /**
     * Will be called periodically whenever the CommandScheduler runs.
     */
    void Periodic() override;

    frc::Pose2d GetPosition();

    std::vector<DetectionParser::DetectedObject> GetObjects();

    /**
     * Will be called periodically whenever the CommandScheduler runs during
     * simulation.
     */
    void SimulationPeriodic() override;

    static ArmPose GetArmPoseFromDistance(units::inch_t distance,
                                          units::inch_t poleHeight) {
        return {units::math::atan(poleHeight / distance),
                units::math::hypot(distance, poleHeight)};
    }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.
};