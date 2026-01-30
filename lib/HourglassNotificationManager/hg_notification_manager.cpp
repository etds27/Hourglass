
#include "hg_notification_manager.h"

void HourglassNotificationManager::sendNotification(HourglassNotification notification)
{
    for (auto &client : notificationClients)
    {
        client->handleNotification(notification);
    }
}

void HourglassNotificationManager::registerNotificationHandler(HGNotificationHandler* handler)
{
    notificationClients.push_back(handler);
}