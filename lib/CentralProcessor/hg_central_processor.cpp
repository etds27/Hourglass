#include "hg_central_processor.h"

#include "device_config.h"
#include "device_context.h"
#include "device_runtime.h"

#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

CentralProcessor::CentralProcessor()
{
}

void CentralProcessor::start(DeviceContext *context, DeviceRuntime *runtime)
{
    context->centralInterface->setService();
    runtime->deviceState = DeviceState::State::AwaitingConnection;

    context->centralInterface->sendDeviceName(runtime->deviceName);
    context->centralInterface->sendDeviceLEDCount(DeviceConfigurator::readLEDCount());
    context->centralInterface->sendDeviceLEDOffset(DeviceConfigurator::readLEDOffset());

    ColorConfig deviceConfig = DeviceConfigurator::readColorConfig(static_cast<uint16_t>(DeviceState::State::DeviceColorMode));
    DeviceConfigurator::printColorConfig(deviceConfig, static_cast<uint16_t>(DeviceState::State::DeviceColorMode));

    context->centralInterface->sendDeviceColorConfig(deviceConfig);
}

bool CentralProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{
    // Process central interface updates
    context->centralInterface->poll();
    runtime->deviceState = context->centralInterface->getCommandedDeviceState();
    runtime->configState = context->centralInterface->getDeviceColorConfigState();
    runtime->isActiveTurn = context->centralInterface->isTurn();

    runtime->isConnected = context->centralInterface->isConnected();

    if (runtime->isConnected)
    {
        runtime->lastConnection = millis();
    }
    else
    {
        runtime->lastDisconnection = millis();
    }

    return true;
}
