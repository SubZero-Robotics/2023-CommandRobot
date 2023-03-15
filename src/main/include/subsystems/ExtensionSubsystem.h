#pragma once

#include "Constants.h"
#include "rev/CANSparkMax.h"
#include "subsystems/BaseSingleAxisSubsystem.h"
#include "utils/Logging.h"

class ExtensionSubsystem
    : public BaseSingleAxisSubsystem<rev::CANSparkMax,
                                     rev::SparkMaxRelativeEncoder, units::meter,
                                     units::meter_t> {
   public:
    ExtensionSubsystem()
        : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, &min,
                                  nullptr, "EXTEND ARM") {}

    void ResetEncoder() override { m_encoder.SetPosition(0); }

    units::meter_t GetCurrentPosition() override {
        auto position = m_encoder.GetPosition() * ArmConstants::kInPerRotation;

        Logging::logToSmartDashboard("ExtensionPositon",
                                     std::to_string(position.value()),
                                     Logging::Level::INFO);

        return position;
    }

   private:
    rev::CANSparkMax m_extensionMotor{CANSparkMaxConstants::kExtensionMotorID,
                                      rev::CANSparkMax::MotorType::kBrushless};

    rev::SparkMaxRelativeEncoder m_encoder = m_extensionMotor.GetEncoder(
        rev::SparkMaxRelativeEncoder::Type::kHallSensor,
        ArmConstants::kTicksPerMotorRotation);

    SingleAxisConfig m_config = {
        BaseSingleAxisSubsystem::AxisType::Linear,
        frc::ProfiledPIDController<units::meter>(
            1.3, 0.0, 0.7,
            frc::TrapezoidProfile<units::meter>::Constraints(1.75_mps,
                                                             0.75_mps_sq)),
        0_in,
        ArmConstants::kMaxArmDistance,
        ArmConstants::kInPerRotation,
        BaseSingleAxisSubsystem::ConfigConstants::MOTOR_DIRECTION_NORMAL,
        ArmConstants::kExtenderLimitSwitchPort,
        BaseSingleAxisSubsystem::UNUSED_DIO_PORT,
        ArmConstants::kExtenderHomingSpeed};

    frc::DigitalInput min{ArmConstants::kExtenderLimitSwitchPort};
};