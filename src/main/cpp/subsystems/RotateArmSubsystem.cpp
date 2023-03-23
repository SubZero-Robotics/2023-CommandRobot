#include "subsystems/RotateArmSubsystem.h"

RotateArmSubsystem::RotateArmSubsystem()
    : BaseSingleAxisSubsystem(m_config, m_leadRotationMotor, m_enc, &min,
                                &max, "ARM", "\033[95;40;4m") {
    m_followRotationMotor.Follow(m_leadRotationMotor);
    // https://docs.wpilib.org/en/stable/docs/software/advanced-controls/introduction/tuning-vertical-arm.html
    _config = m_config;
    _controller = m_config.pid;
    _controller.SetTolerance(10, 15);
    m_enc.SetPositionConversionFactor(1);
}

void RotateArmSubsystem::ResetEncoder() {
    if (_log)
        Logging::logToStdOut(_prefix, "RESET POSITION",
                                Logging::Level::INFO);
    // m_enc.SetZeroOffset(0);
}

double RotateArmSubsystem::GetCurrentPosition() {
    // "Home" is at 60 deg relative to ground
    auto position = m_enc.GetPosition() * _config.distancePerRevolution +
                    _config.minDistance;

    if (position >= 350) position = 0;

    Logging::logToSmartDashboard("ArmPosition",
                                    std::to_string(position) + " deg",
                                    Logging::Level::INFO);

    if (_log)
        Logging::logToStdOut(_prefix,
                                std::to_string(position) + "/" +
                                    std::to_string(_config.maxDistance) +
                                    " deg",
                                Logging::Level::INFO);

    return position;
}

void RotateArmSubsystem::UpdateMovement() {
    if (_isMovingToPosition) {
        if (_log)
            Logging::logToStdOut(
                _prefix,
                "Target Position: " + std::to_string(_targetPosition) +
                    std::string(_config.type == AxisType::Linear ? " in"
                                                                    : " deg"),
                Logging::Level::INFO, _ansiPrefixModifiers);

        // TODO: extract multipliers to constants and pass through the
        // config
        auto res =
            _controller.Calculate(GetCurrentPosition(), _targetPosition) *
            -1;
        auto clampedRes = std::clamp(res, -1.0, 1.0) * 0.5;
        if (_log)
            Logging::logToStdOut(
                _prefix, "Clamped Res: " + std::to_string(clampedRes),
                Logging::Level::INFO, _ansiPrefixModifiers);
        Logging::logToSmartDashboard(_prefix + " TargetPos",
                                        std::to_string(_targetPosition),
                                        Logging::Level::INFO);

        if (_controller.AtSetpoint()) {
            Logging::logToStdOut(_prefix, "REACHED GOAL",
                                    Logging::Level::INFO,
                                    _ansiPrefixModifiers);
            StopMovement();
            return;
        }

        RunMotorSpeed(clampedRes);
    }
}
