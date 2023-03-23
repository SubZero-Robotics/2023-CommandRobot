#include "subsystems/WristSubsystem.h"

WristSubsystem::WristSubsystem()
    : BaseSingleAxisSubsystem(m_config, m_wristMotor, m_encoder, &min,
                                nullptr, "WRIST", "\033[92;40;4m") {
    _config = m_config;
    _controller = m_config.pid;
    _controller.SetTolerance(10, 10);
    m_encoder.SetPositionConversionFactor(1);
}

void WristSubsystem::ResetEncoder() {
    if (_log)
        Logging::logToStdOut(_prefix, "RESET POSITION",
                                Logging::Level::INFO);
    // m_encoder.SetZeroOffset(0);
}

double WristSubsystem::GetCurrentPosition() {
    auto position = m_encoder.GetPosition() * _config.distancePerRevolution;

    if (position >= 350) position = 0;

    Logging::logToSmartDashboard("WristPosition",
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

void WristSubsystem::RunMotorExternal(double speed) {
    // TODO: constant
    if (abs(speed) <= 0.05) {
        if (_isMovingToPosition)
            return;  // Don't set the motor and overwrite a potential
                        // automated movement

        if (_config.type == AxisType::Rotational)
            RunMotorSpeed(-0.005);
        else
            _motor.Set(0);
        return;
    }

    // Overwrite current automated position with joystick input
    if (_isMovingToPosition) {
        StopMovement();
    }

    RunMotorSpeed(speed);
}

void WristSubsystem::UpdateMovement() {
    if (_isMovingToPosition) {
        if (_log)
            Logging::logToStdOut(
                _prefix,
                "Target Position: " + std::to_string(_targetPosition) +
                    std::string(_config.type == AxisType::Linear ? " in"
                                                                    : " deg"),
                Logging::Level::INFO, _ansiPrefixModifiers);

        // TODO: extract multipliers to constants and pass through the config
        auto res =
            _controller.Calculate(GetCurrentPosition(), _targetPosition);
        auto clampedRes = std::clamp(res, -1.0, 1.0) * 0.66;
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