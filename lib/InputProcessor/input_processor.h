#pragma once

#include "executable_task.h"
#include "device_runtime.h"
#include "device_context.h"

struct InputProcessor : ExecutableTask {
    InputProcessor();
    /// @brief Update the input processor's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;
};