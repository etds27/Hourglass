#pragma once

#include "executable_task.h"
#include "device_context.h"
#include "device_runtime.h"

class DisplayProcessor : public ExecutableTask {
    public:
    DisplayProcessor();

    /// @brief Update the task's internal state
    /// @return false if the task is complete and should be removed, true otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;

};