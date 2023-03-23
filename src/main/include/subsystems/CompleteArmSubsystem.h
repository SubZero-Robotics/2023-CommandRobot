#ifndef MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H
#define MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H

#include <frc2/command/Command.h>
#include <frc2/command/FunctionalCommand.h>
#include <frc2/command/InstantCommand.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SubsystemBase.h>

#include <vector>

#include "Constants.h"
#include "commands/Autos.h"
#include "subsystems/BaseSingleAxisSubsystem.h"
#include "utils/Logging.h"

class CompleteArmSubsystem : public frc2::SubsystemBase {
   public:
    /**
     * @param axes List of axes to control. Order when homing depends on order
     * within list (Axis 1 -> Axis 2 -> ... -> Axis n)
     */
    CompleteArmSubsystem(ISingleAxisSubsystem* rotateArm,
                         ISingleAxisSubsystem* wrist,
                         ISingleAxisSubsystem* extension)
        : m_rotateArm(rotateArm),
          m_wrist(wrist),
          m_extension(extension) {}

    /**
     * @brief Stop movement on all axes
     *
     */
    frc2::CommandPtr Stop();

    /**
     * @brief Homes all axes in order
     *
     */
    frc2::CommandPtr Home();

    // frc2::CommandPtr AutoIntake();

    frc2::CommandPtr TravelMode();

    frc2::CommandPtr SetPose(WholeArmPose pose);

    frc2::CommandPtr SetPose(ArmAxisPose pose);

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