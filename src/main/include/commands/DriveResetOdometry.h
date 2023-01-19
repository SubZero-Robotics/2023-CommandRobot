#pragma once

#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc/Joystick.h>

#include "subsystems/drivetrain.h"

class DriveResetOdometry : public frc2::CommandHelper<frc2::CommandBase, DriveResetOdometry> {   // this line builds fine
    public: 
     explicit DriveResetOdometry(drivetrain* subsystem, frc::Joystick* controller);

     void Initialize() override;
     
     void Execute() override;
     
     void End(bool interrupted) override;
     
     bool IsFinished() override;

    private: 
     drivetrain* m_drive;
     frc::Joystick* m_controller;

};