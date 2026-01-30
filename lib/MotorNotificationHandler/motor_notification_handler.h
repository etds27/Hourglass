#pragma once

#include "hg_notification.h"
#include "hg_notification_manager.h"
#include "Arduino.h"

class MotorNotificationHandler : public HGNotificationHandler {
    public:
    MotorNotificationHandler(uint8_t motorPin, uint16_t duration, uint8_t intensity);

    /// @brief Handle a notification sent by the HourglassNotificationManager
    /// @param notification The notification to handle
    void handleNotification(HourglassNotification notification) override;

    void update() override;

    private:
    uint16_t m_vibrationDuration; // Duration of the vibration in milliseconds
    uint8_t m_vibrationIntensity; // Intensity of the vibration (0-255)
    uint8_t m_motorPin;          // Pin connected to the motor driver

    unsigned long m_vibrationEndTime = 0; // Time when the current vibration should end
};