#pragma once

#include "hg_notification.h"
#include "hg_notification_manager.h"
#include "executable_task.h"
#include "Arduino.h"

class MotorNotification : public ExecutableTask{
    public:
    MotorNotification(uint8_t motorPin, HourglassNotification notification);

    void start(DeviceContext* context, DeviceRuntime* runtime) override;
    void cleanup(DeviceContext* context, DeviceRuntime* runtime) override;
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;

    private:
    uint16_t m_vibrationDuration; // Duration of the vibration in milliseconds
    uint8_t m_vibrationIntensity; // Intensity of the vibration (0-255)
    uint8_t m_motorPin;          // Pin connected to the motor driver
    HourglassNotification m_notification; // The notification that triggered this motor notification

    unsigned long m_vibrationEndTime = 0; // Time when the current vibration should end
    unsigned long m_vibrationStartTime = 0; // Time when the current vibration started

};