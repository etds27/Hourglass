#include "motor_notification_handler.h"

MotorNotificationHandler::MotorNotificationHandler(uint8_t motorPin, uint16_t duration, uint8_t intensity)
{
    m_motorPin = motorPin;
    m_vibrationDuration = duration;
    m_vibrationIntensity = intensity;

    pinMode(m_motorPin, OUTPUT);
}

void MotorNotificationHandler::handleNotification(HourglassNotification notification)
{
    if (notification == HG_NOTIFICATION_TURN_STARTED)
    {
        analogWrite(m_motorPin, m_vibrationIntensity);
        m_vibrationEndTime = millis() + m_vibrationDuration;
    }
}

bool MotorNotificationHandler::update(DeviceContext* context, DeviceRuntime* runtime)
{
    if (m_vibrationEndTime != 0 && millis() >= m_vibrationEndTime)
    {
        analogWrite(m_motorPin, 0);
        m_vibrationEndTime = 0;
        return true; // Task is complete
    }
    return false;
}
