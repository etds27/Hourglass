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
    if (notification == HG_NOTIFICATION_USER_POKE)
    {
        analogWrite(m_motorPin, m_vibrationIntensity);
        m_vibrationEndTime = millis() + m_vibrationDuration;
    }
}

void MotorNotificationHandler::update()
{
    if (m_vibrationEndTime != 0 && millis() >= m_vibrationEndTime)
    {
        analogWrite(m_motorPin, 0);
        m_vibrationEndTime = 0;
    }
}
