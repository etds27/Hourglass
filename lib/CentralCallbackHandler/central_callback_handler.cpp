#include "central_callback_handler.h"

#include "motor_notification.h"

namespace
{
    const LogString loggerTag = "CentralCallbackHandler";
}


CentralCallbackHandler::CentralCallbackHandler()
{
}

char *CentralCallbackHandler::readDeviceName(char *out)
{
    char arduinoID[MAX_NAME_LENGTH] = {};
#ifdef SIMULATOR
    out = new char[10]{'S', 'I', 'M', 'U', 'L', 'A', 'T', 'O', 'R', '\0'};
#else
    DeviceConfigurator::readName(arduinoID, MAX_NAME_LENGTH);
#endif

    strncpy(out, arduinoID, MAX_NAME_LENGTH);
    out[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    return out;
}

#ifndef SIMULATOR

void CentralCallbackHandler::writeDeviceName(const char *deviceName, uint8_t length)
{
    DeviceConfigurator::writeName(deviceName);
    logger.info(loggerTag, ": Device name changed to: ", deviceName);
    m_context->centralInterface->sendDeviceName(deviceName);
}

void CentralCallbackHandler::writeDeviceColorConfig(ColorConfig config, DeviceState::State configState)
{
    DeviceConfigurator::writeColorConfig(config, static_cast<uint16_t>(configState));
    // m_interface->sendDeviceColorConfig(config);
    DeviceConfigurator::printColorConfig(config, static_cast<uint16_t>(configState));
}
#endif

void CentralCallbackHandler::onDeviceNameChanged(char *name)
{
    // No-op
    // Device name writes are handled immediately in onDeviceNameWriteChanged
}

void CentralCallbackHandler::onDeviceNameWriteChanged(bool write)
{
    if (write)
    {
        logger.info(loggerTag, ": Device name write enabled");
        m_context->centralInterface->getDeviceName(m_runtime->deviceName, MAX_NAME_LENGTH);
        logger.info(loggerTag, ": onDeviceNameWriteChanged received. Device name changed to: ", m_runtime->deviceName);
        writeDeviceName(m_runtime->deviceName, strlen(m_runtime->deviceName));
    }
    else
    {
        logger.info(loggerTag, ": Device name write not enabled");
    }
}

void CentralCallbackHandler::onDeviceColorConfigChanged(ColorConfig config)
{
    DeviceConfigurator::printColorConfig(config, 0);
    // writeDeviceColorConfig(config);
    m_context->displayManager->updateColorConfig(config);
}

void CentralCallbackHandler::onDeviceColorConfigWriteChanged(bool write)
{
    if (write)
    {
        logger.info(loggerTag, ": Device color config write enabled");
        DeviceState::State configState = m_context->centralInterface->getDeviceColorConfigState();
        ColorConfig config = m_context->centralInterface->readColorConfig();
        // DeviceConfigurator::printColorConfig(config, configState);
        writeDeviceColorConfig(config, configState);
    }
    else
    {
        logger.info(loggerTag, ": Device color config write not enabled");
    }
}

void CentralCallbackHandler::onDeviceColorConfigStateChanged(DeviceState::State state)
{
    logger.info(loggerTag, ": onDeviceColorConfigStateChanged received. New state: ", static_cast<int>(state));
    // Update the app with the new configuration state
    ColorConfig config = DeviceConfigurator::readColorConfig(static_cast<uint16_t>(state));
    logger.info(loggerTag, ": Sending color config for state ", static_cast<int>(state), " to central");
    DeviceConfigurator::printColorConfig(config);
    m_context->centralInterface->sendDeviceColorConfig(config);
}

void CentralCallbackHandler::onDeviceLEDOffsetChanged(uint8_t offset)
{
    logger.info(loggerTag, ": onDeviceLEDOffsetChanged received. New offset: ", offset);
    m_context->displayManager->updateLEDOffset(offset);
}

void CentralCallbackHandler::onDeviceLEDOffsetWriteChanged(bool write)
{
    logger.info(loggerTag, ": onDeviceLEDOffsetWriteChanged received. New value: ", write);
    if (write)
    {
        uint8_t offset = m_context->centralInterface->readDeviceLEDOffset();
        DeviceConfigurator::writeLEDOffset(offset);
        logger.info(loggerTag, ": LED offset changed to: ", offset);
    }
    else
    {
        logger.info(loggerTag, ": LED offset write not enabled");
    }
}

void CentralCallbackHandler::onDeviceLEDCountChanged(uint8_t count)
{
    logger.info(loggerTag, ": onDeviceLEDCountChanged received. New count: ", count);
    m_context->displayManager->updateLEDCount(count);
}

void CentralCallbackHandler::onDeviceLEDCountWriteChanged(bool write)
{
    logger.info(loggerTag, ": onDeviceLEDCountWriteChanged received. New value: ", write);
    if (write)
    {
        uint8_t count = m_context->centralInterface->readDeviceLEDCount();
        DeviceConfigurator::writeLEDCount(count);
        logger.info(loggerTag, ": LED count changed to: ", count);
    }
    else
    {
        logger.info(loggerTag, ": LED count write not enabled");
    }
}

void hexDump(const void* data, size_t size)
{
    const uint8_t* bytes = (const uint8_t*)data;

    for (size_t i = 0; i < size; i++)
    {
        if (bytes[i] < 16)
            Serial.print('0');

        Serial.print(bytes[i], HEX);
        Serial.print(' ');

        if ((i + 1) % 16 == 0)
            Serial.println();
    }
    Serial.println();
}

void CentralCallbackHandler::onDeviceMotorNotificationReceived(NotificationEvent event)
{
    logger.info(loggerTag, ": onDeviceMotorNotificationReceived received. Event type: ", static_cast<int>(event.data));
    logger.info(loggerTag, ": onDeviceMotorNotificationReceived sent at " , event.timestamp);
    hexDump(&event, sizeof(event));
    MotorNotification* notification = new MotorNotification(MOTOR_PIN, event.data);
    m_runtime->pendingTasks.enqueue(notification);
}


void CentralCallbackHandler::registerCallbacks(DeviceContext *context, DeviceRuntime *runtime)
{
    m_context = context;
    m_runtime = runtime;
    m_context->centralInterface->registerDeviceNameChangedCallback([this](char *name)
                                                                   { this->onDeviceNameChanged(name); });

    m_context->centralInterface->registerDeviceColorConfigChangedCallback([this](ColorConfig config)
                                                                          { this->onDeviceColorConfigChanged(config); });
    m_context->centralInterface->registerDeviceNameWriteCallback([this](bool write)
                                                                 { this->onDeviceNameWriteChanged(write); });
    m_context->centralInterface->registerDeviceColorConfigStateChangeCallback([this](DeviceState::State state)
                                                                              { this->onDeviceColorConfigStateChanged(state); });
    m_context->centralInterface->registerDeviceColorConfigWriteCallback([this](bool write)
                                                                        { this->onDeviceColorConfigWriteChanged(write); });
    m_context->centralInterface->registerDeviceLEDOffsetChangedCallback([this](uint8_t offset)
                                                                        { this->onDeviceLEDOffsetChanged(offset); });
    m_context->centralInterface->registerDeviceLEDOffsetWriteCallback([this](bool write)
                                                                      { this->onDeviceLEDOffsetWriteChanged(write); });
    m_context->centralInterface->registerDeviceLEDCountChangedCallback([this](uint8_t count)
                                                                       { this->onDeviceLEDCountChanged(count); });
    m_context->centralInterface->registerDeviceLEDCountWriteCallback([this](bool write)
                                                                     { this->onDeviceLEDCountWriteChanged(write); });
    m_context->centralInterface->registerDeviceMotorNotificationCallback([this](NotificationEvent event)
                                                                          { this->onDeviceMotorNotificationReceived(event); });
}