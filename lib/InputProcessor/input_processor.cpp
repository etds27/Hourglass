#include "input_processor.h"
#include "input_monitor.h"
#include "device_context.h"

InputProcessor::InputProcessor()
{
}

bool InputProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{
    DeviceState::State deviceState = runtime->deviceState;
    ButtonInputType buttonAction = context->buttonInputMonitor->getAction();

    if (buttonAction == ButtonInputType::LongPress)
    {
        runtime->absoluteOrientation = !runtime->absoluteOrientation;
    }

    if (buttonAction == ButtonInputType::TripleButtonPress)
    {
        runtime->colorBlindMode = !runtime->colorBlindMode;
    }

    if (buttonAction == ButtonInputType::DoubleButtonPress &&
        DeviceState::isStateSkipEligible(deviceState))
    {
        context->centralInterface->toggleSkippedState();
    }

    if (runtime->isActiveTurn && buttonAction == ButtonInputType::ButtonPress)
    {
        context->centralInterface->endTurn();
    }

    // This is an indefinite task, continue to return true
    return true;
}
