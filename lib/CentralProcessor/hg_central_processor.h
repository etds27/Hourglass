#pragma once

#include "executable_task.h"
#include "device_context.h"
#include "device_runtime.h"


class CentralProcessor : public ExecutableTask {
    public:
    CentralProcessor();

    void start(DeviceContext* context, DeviceRuntime* runtime) override;


    /// @brief Update the task's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;
};