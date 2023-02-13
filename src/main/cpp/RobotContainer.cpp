// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/button/Trigger.h>

#include "commands/autos/StraightBack.h"
#include "commands/ExampleCommand.h"
#include "commands/DefaultDrive.h"

RobotContainer::RobotContainer()
{
  // Initialize all of your commands and subsystems here

  m_chooser.SetDefaultOption("Straight Back", &m_straightbackAuto);

  // Configure the button bindings
  ConfigureBindings();

  // Default drive command.  This will be run in teleop and when no other command is running.
  m_drive.SetDefaultCommand(DefaultDrive(
      &m_drive,
      [this]
      { return Xbox.GetLeftY(); },
      [this]
      { return Xbox.GetLeftX(); }));
}

void RobotContainer::ConfigureBindings()
{
  // Configure your trigger bindings here
  // This is a fun change from 2022. Button bindings as we knew them are gone.
  // We now have triggers.
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
{
  return NULL;
}
