#ifndef MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H
#define MULTI_AXIS_ORCHESTRATOR_SUBSYSTEM_H

#include <frc2/command/Command.h>
#include <frc2/command/SequentialCommandGroup.h>
#include <frc2/command/SubsystemBase.h>

#include <vector>

#include "subsystems/BaseSingleAxisSubsystem.h"
#include "utils/Logging.h"

class MultiAxisOrhestratorSubsystem : public frc2::SubsystemBase {
   public:
    struct MultiAxisPose {
        ISingleAxisSubsystem *axis;
        double position;
    };

    /**
     * @param axes List of axes to control. Order when homing depends on order
     * within list (Axis 1 -> Axis 2 -> ... -> Axis n)
     */
    MultiAxisOrhestratorSubsystem(std::vector<ISingleAxisSubsystem *> &&axes)
        : m_axes(axes) {}

    /**
     * @brief Stop movement on all axes
     *
     */
    void Stop();

    /**
     * @brief Homes all axes in order
     *
     */
    void Home() {
        // * I tried to get too fancy here :(
        if (!m_axes.empty()) {
            auto commandChain = m_axes.front()->GetHomeCommand();
            for (auto it = m_axes.begin() + 1; it != m_axes.end(); it++) {
                auto axis = *it;
                commandChain = commandChain.AndThen(axis->GetHomeCommand());
            }

            commandChain.Schedule();
        }
    }

    void SetPoses(std::vector<MultiAxisPose> poses);

    /**
     * @brief Let R := this method's return value and S := list of axes. R_i =
     * S_i->GetIsMovingToPosition()
     *
     * @return std::vector<bool> List of movement states
     */
    std::vector<bool> GetIsMoving() const;

    void Periodic() override;

   private:
    std::vector<ISingleAxisSubsystem *> m_axes;
};

#endif