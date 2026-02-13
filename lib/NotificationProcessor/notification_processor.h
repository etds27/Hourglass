#pragma once

#include "executable_task.h"


struct NotificationProcessor: ExecutableTask {
    /// @brief Update the notification processor's internal state
    /// @return true if the task is complete and should be removed, false otherwise
    bool update(DeviceContext* context, DeviceRuntime* runtime);
};