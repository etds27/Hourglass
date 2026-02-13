#include "motor_notification.h"
#include "hg_notification.h"

MotorNotification::MotorNotification(uint8_t motorPin, HourglassNotification notification)
{
    m_motorPin = motorPin;
    m_notification = notification;

    if (notification == HourglassNotification::HG_NOTIFICATION_TURN_STARTED)
    {
        m_vibrationDuration = 1000; // Vibrate for 1 second
        m_vibrationIntensity = 255; // Maximum intensity
    }
    else if (notification == HourglassNotification::HG_NOTIFICATION_USER_POKE)
    {
        m_vibrationDuration = 500; // Vibrate for 0.5 seconds
        m_vibrationIntensity = 128; // Medium intensity
    }
    else if (notification == HourglassNotification::HG_NOTIFICATION_DEVICE_CONNECTED)
    {
        m_vibrationDuration = 250; // Vibrate for 0.25
        m_vibrationIntensity = 255;
    }



    pinMode(m_motorPin, OUTPUT);
}

void MotorNotification::start(DeviceContext* context, DeviceRuntime* runtime)
{
    analogWrite(m_motorPin, m_vibrationIntensity);
    m_vibrationEndTime = millis() + m_vibrationDuration;
}

void MotorNotification::cleanup(DeviceContext *context, DeviceRuntime *runtime)
{
    analogWrite(m_motorPin, 0);
}

bool MotorNotification::update(DeviceContext* context, DeviceRuntime* runtime)
{
    if (m_vibrationEndTime != 0 && millis() >= m_vibrationEndTime)
    {
        analogWrite(m_motorPin, 0);
        m_vibrationEndTime = 0;
        return false; // Task is complete
    }
    return true;
}
