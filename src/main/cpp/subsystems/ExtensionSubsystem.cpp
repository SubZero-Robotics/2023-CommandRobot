#include "subsystems/ExtensionSubsystem.h"

ExtensionSubsystem::ExtensionSubsystem()
    : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, &min,
                                nullptr, "EXTEND", "\033[96;40;4m") {
    m_extensionMotor.SetInverted(true);
    _config = m_config;
    _controller = m_config.pid;
    _controller.SetTolerance(.5, 1);
    _config.distancePerRevolution = ArmConstants::kInPerRotation;
    m_encoder.SetPositionConversionFactor(1);
}

void ExtensionSubsystem::ResetEncoder() {
    if (_log)
        Logging::logToStdOut(_prefix, "RESET POSITION",
                                Logging::Level::INFO);
    m_encoder.SetPosition(0);
}

double ExtensionSubsystem::GetCurrentPosition() {
    auto position = m_encoder.GetPosition() * _config.distancePerRevolution;

    Logging::logToSmartDashboard("ExtensionPosition",
                                    std::to_string(position) + " in",
                                    Logging::Level::INFO);

    if (_log)
        Logging::logToStdOut(_prefix,
                                std::to_string(position) + "/" +
                                    std::to_string(_config.maxDistance) +
                                    " in",
                                Logging::Level::INFO);

    return position;
}

void ExtensionSubsystem::UpdateMovement() {
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
            6.0;
        auto clampedRes = std::clamp(res, -1.0, 1.0) * -1.0;
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
