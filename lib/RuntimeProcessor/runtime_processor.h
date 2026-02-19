#pragma once

#include "executable_task.h"

class RuntimeProcessor : public ExecutableTask {
    public:
    RuntimeProcessor();

    /// @brief Update the task's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;

};