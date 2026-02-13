#include "notification_processor.h"

#include "logger.h"
#include "device_context.h"
#include "device_runtime.h"

#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

bool NotificationProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{
    NotificationEvent event;
    while(runtime->events.notificationQueue.dequeue(event)){
        
    }
    return true;
}
