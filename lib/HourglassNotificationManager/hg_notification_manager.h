#pragma once

#include <vector>
#include <functional>
#include "hg_notification.h"
#include "hg_notification_handler.h"

class HourglassNotificationManager {
    public:
    /// @brief Send a notification to all registered notification handlers
    /// @param notification The notification to send
    void sendNotification(HourglassNotification notification);

    /// @brief Register a notification handler
    /// @param handler The handler function to register
    void registerNotificationHandler(HGNotificationHandler* handler);

    private:
        void applyToAllInterfaces(std::function<void(HGNotificationHandler*)> func);
        std::vector<HGNotificationHandler*> notificationClients;
};