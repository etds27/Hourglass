#pragma once 

#include "hg_display_manager.h"
#include "input_monitor.h"
#include "hg_central_interface.h"


// Forward declaration of all components to avoid circular dependencies
struct HGCentralInterface;
struct ButtonInputMonitor;
struct HourglassDisplayManager;
struct SleepProcessor;
struct DisplayProcessor;
struct CentralProcessor;
struct RuntimeProcessor;
struct InputProcessor;


struct DeviceContext {
    HGCentralInterface* centralInterface;
    ButtonInputMonitor* buttonInputMonitor;
    HourglassDisplayManager* displayManager;
    SleepProcessor* sleepProcessor;
    DisplayProcessor* displayProcessor;
    CentralProcessor* centralProcessor;
    RuntimeProcessor* runtimeProcessor;
    InputProcessor* inputProcessor;
};