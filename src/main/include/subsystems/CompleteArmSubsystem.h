#ifndef MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H
#define MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H

#include <frc2/command/Command.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SubsystemBase.h>

#include <vector>

#include "subsystems/BaseSingleAxisSubsystem.h"

class CompleteArmSubsystem : public frc2::SubsystemBase {
   public:
    struct ArmAxisPose {
        ISingleAxisSubsystem* axis;
        double position;
    };

    struct WholeArmPose {
        ArmAxisPose arm;
        ArmAxisPose wrist;
        ArmAxisPose extension;
    };

    /**
     * @param axes List of axes to control. Order when homing depends on order
     * within list (Axis 1 -> Axis 2 -> ... -> Axis n)
     */
    CompleteArmSubsystem(ISingleAxisSubsystem* rotateArm,
                         ISingleAxisSubsystem* wrist,
                         ISingleAxisSubsystem* extension)
        : m_rotateArm(rotateArm), m_wrist(wrist), m_extension(extension) {}

    /**
     * @brief Stop movement on all axes
     *
     */
    void Stop();

    /**
     * @brief Homes all axes in order
     *
     */
    frc2::CommandPtr Home() {
        // ? Not sure if this is right...
        return m_rotateArm->GetHomeCommand()
            .AndThen(m_wrist->GetHomeCommand())
            .AndThen(m_extension->GetHomeCommand());
    }

    void SetPoses(WholeArmPose pose);

    /**
     * @brief True if axis is moving
     *
     * @return bool
     */
    bool GetIsMoving(ISingleAxisSubsystem* axis) const;

    void Periodic() override;

   private:
    ISingleAxisSubsystem* m_rotateArm;
    ISingleAxisSubsystem* m_wrist;
    ISingleAxisSubsystem* m_extension;
};

#endif