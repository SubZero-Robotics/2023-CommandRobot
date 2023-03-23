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
#include "commands/IntakeInCommand.h"
#include "commands/IntakeOutCommand.h"
#include "commands/SpinIntakeTimer.h"
#include "subsystems/BaseSingleAxisSubsystem.h"
#include "subsystems/DriveSubsystem.h"
#include "subsystems/IntakeSubsystem.h"
#include "subsystems/LEDControllerSubsystem.h"
#include "subsystems/VL53L1XSubsystem.h"
#include "commands/BrakeSetCommand.h"
#include "commands/BrakeStopCommand.h"
#include "utils/Logging.h"

class CompleteArmSubsystem : public frc2::SubsystemBase {
   public:
    enum class MovementType {
        None,
        HomeAll,
        AutoIntake,
        TravelMode,
        PlaceHighCone,
        PlaceMediumCone,
        PlaceLowCone,
        PlaceHighCube,
        PlaceMediumCube,
        PlaceLowCube,
        BrakeModeOn,
        BrakeModeOff
    };

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
                         ISingleAxisSubsystem* extension,
                         IntakeSubsystem* intake, DriveSubsystem* drive,
                         LEDControllerSubsystem* leds, VL53L1XController* lidar)
        : m_rotateArm(rotateArm),
          m_wrist(wrist),
          m_extension(extension),
          m_intake(intake),
          m_drive(drive),
          m_leds(leds),
          m_lidar(lidar) {}

    /**
     * @brief Stop movement on all axes
     *
     */
    frc2::CommandPtr Stop();

    /**
     * @brief  Set the LEDs to show the correct
     *
     * @param type
     * @return frc2::CommandPtr
     */
    frc2::CommandPtr SetMovementLED(MovementType type);

    /**
     * @brief Homes all axes in order
     *
     */
    frc2::CommandPtr Home();

    frc2::CommandPtr AutoIntake();

    frc2::CommandPtr TravelMode();

    frc2::CommandPtr AutoPlaceHighCone();

    frc2::CommandPtr AutoPlaceMediumCone();

    frc2::CommandPtr AutoPlaceLowCone();

    frc2::CommandPtr AutoPlaceHighCube();

    frc2::CommandPtr AutoPlaceMediumCube();

    frc2::CommandPtr AutoPlaceLowCube();

    frc2::CommandPtr AutoPlaceHigh();

    frc2::CommandPtr AutoPlaceMedium();

    frc2::CommandPtr AutoPlaceLow();

    frc2::CommandPtr BrakeModeOn();

    frc2::CommandPtr BrakeModeOff();

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
    IntakeSubsystem* m_intake;
    DriveSubsystem* m_drive;
    LEDControllerSubsystem* m_leds;
    VL53L1XController* m_lidar;
};

#endif