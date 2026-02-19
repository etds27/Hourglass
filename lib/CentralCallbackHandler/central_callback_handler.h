
#pragma once

#include "device_context.h"
#include "device_runtime.h"

class CentralCallbackHandler
{
public:
    CentralCallbackHandler();

    void registerCallbacks(DeviceContext *context, DeviceRuntime *runtime);

private:
    DeviceContext *m_context;
    DeviceRuntime *m_runtime;

    char *readDeviceName(char *out);

    void writeDeviceName(const char *deviceName, uint8_t length);

    void writeDeviceColorConfig(ColorConfig config, DeviceState::State configState);

    void onDeviceNameChanged(char *name);

    void onDeviceNameWriteChanged(bool write);

    void onDeviceColorConfigChanged(ColorConfig config);

    void onDeviceColorConfigWriteChanged(bool write);

    void onDeviceColorConfigStateChanged(DeviceState::State state);
    void onDeviceLEDOffsetChanged(uint8_t offset);
    void onDeviceLEDOffsetWriteChanged(bool write);
    void onDeviceLEDCountChanged(uint8_t count);
    void onDeviceLEDCountWriteChanged(bool write);
    void onDeviceMotorNotificationReceived(NotificationEvent event);
};