#include "runtime_processor.h"
#include "device_context.h"
#include "device_runtime.h"

#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

RuntimeProcessor::RuntimeProcessor()
{
}

bool RuntimeProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{
    runtime->lastUpdate = millis();
    return true;
}
