#pragma once

#include <frc/Joystick.h>
#include <frc/geometry/Pose2d.h>
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>

#include "subsystems/DriveSubsystem.h"

class DriveResetOdometry
    : public frc2::CommandHelper<frc2::CommandBase,
                                 DriveResetOdometry> {  // this line builds fine
   public:
    explicit DriveResetOdometry(DriveSubsystem* driveSubsystem,
                                frc::Joystick* controller)
        : m_driveSubsystem(driveSubsystem), m_controller(controller) {
        AddRequirements({m_driveSubsystem});
    }

    void Initialize() override {
        frc::Pose2d currentRobotPose;  // is also zeroed by default
        m_driveSubsystem->ResetOdometry(currentRobotPose);
    }

    void Execute() override {}

    void End(bool interrupted) override {}

    bool IsFinished() override { return false; }

   private:
    DriveSubsystem* m_driveSubsystem;
    frc::Joystick* m_controller;
};