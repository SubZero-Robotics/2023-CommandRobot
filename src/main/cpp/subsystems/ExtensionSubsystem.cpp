#include "subsystems/ExtensionSubsystem.h"

ExtensionSubsystem::ExtensionSubsystem()
    : BaseSingleAxisSubsystem(m_config, m_extensionMotor, m_encoder, &min,
                              nullptr, "EXTEND") {
    m_extensionMotor.SetInverted(true);
    _config = m_config;
    _controller = m_config.pid;
    _controller.SetTolerance(.5, 1);
    _config.distancePerRevolution = ArmConstants::kInPerRotation;
    m_encoder.SetPositionConversionFactor(1);
}

void ExtensionSubsystem::ResetEncoder() {
    if (_log)
        Logging::logToStdOut(_prefix, "RESET POSITION", Logging::Level::INFO);
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
                                 std::to_string(_config.maxDistance) + " in",
                             Logging::Level::INFO);

    return position;
}
