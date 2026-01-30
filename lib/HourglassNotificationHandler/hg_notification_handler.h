#pragma once

#include "hg_notification.h"
#include <functional>

class HGNotificationHandler {
    public:
    /// @brief Handle a notification sent by the HourglassNotificationManager
    /// @param notification The notification to handle
    virtual void handleNotification(HourglassNotification notification) = 0;

    virtual void update() = 0;
};