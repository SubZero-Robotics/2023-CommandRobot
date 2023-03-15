// haha mario oddessy assist mode

#pragma once

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/SubsystemBase.h>
#include <networktables/NetworkTableInstance.h>

#include <cmath>
#include <vector>

#include "utils/DetectionParser.h"
#include "utils/Logging.h"

class AssistSubsystem : public frc2::SubsystemBase {
   public:
    struct ArmPose {
        double rotationDegree;
        double armLengthIn;
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

    static ArmPose GetArmPoseFromDistance(double distanceIn,
                                          double poleHeightIn) {
        return {atan(poleHeightIn / distanceIn),
                hypot(distanceIn, poleHeightIn)};
    }

   private:
    // Components (e.g. motor controllers and sensors) should generally be
    // declared private and exposed only through public methods.
};