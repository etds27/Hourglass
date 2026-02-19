#pragma once

#include "hg_notification.h"
#include "executable_task.h"
#include <functional>

class HGNotificationHandler: ExecutableTask {
    public:
    /// @brief Handle a notification sent by the HourglassNotificationManager
    /// @param notification The notification to handle
    virtual void handleNotification(HourglassNotification notification) = 0;

    virtual bool update(DeviceContext* context, DeviceRuntime* runtime) = 0;
};