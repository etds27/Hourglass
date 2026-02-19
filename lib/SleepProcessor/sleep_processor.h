#pragma once

#include "executable_task.h"

class SleepProcessor : public ExecutableTask {
    public:
    SleepProcessor();

    void start(DeviceContext* context, DeviceRuntime* runtime) override;

    /// @brief Update the task's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime) override;

};