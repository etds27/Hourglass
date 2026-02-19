#pragma once

#include "device_state.h"
#include "hg_notification.h"
#include "executable_task.h"


/// Information is provided from the central device in two way
/// 1. The static state information which is data that is set once, then persists until changed regardless of whether it was read or not
/// 2. Event driven information which is data that is sent as events occur. Each event is queued and consumed in order by a task processor

/// For the static state information, this is used for things like commanded device state where we have to know about every rapid transition 
/// for that value but instead only care about what the central is currently telling us.

/// In contrast, event driven information is used for things like notifications where we need to process every notification sent by the central device
/// and we cannot miss any notifications even if they arrive in rapid succession.

/// Each notification event is comprised of the notification (of whatever data type) itself and a timestamp of when it was received.
/// This timestamp ensures that a unique central/BLE update is processed and prevents issues where multiple identical notifications are sent in rapid succession but not processed

/// The BLE Notification Struct will be added to the event queue
/// A separate notification task processor will consume the events in order
/// For each event struct, it will create a new notification task and add it to the runtimes new task queue to be processed and displayed by the display processor
/// The the DeviceManager will pick up the new tasks from the queue

template <typename T, size_t Size>
class DeviceRuntimeQueue {
    public: 
        bool enqueue(T item) {
            size_t nextHead = (head + 1) % Size;
            if (nextHead == tail) return false; // Queue is full
            items[head] = item;
            head = nextHead;
            return true;
        }

        bool dequeue(T& item) {
            if (head == tail) return false; // Queue is empty
            item = items[tail];
            tail = (tail + 1) % Size;
            return true;
        }

        bool empty() const {
            return head == tail;
        }

        bool full() const {
            return (head + 1) % Size == tail;
        }

        size_t size() const {
            return (head - tail + Size) % Size;
        }


    private:
        T items[Size];
        size_t head = 0;
        size_t tail = 0;
};

/// @brief Event structure for receiving notification data from central
template<typename T>
struct HourglassEvent {
    uint64_t timestamp;
    T data;
};

using NotificationEvent = HourglassEvent<HourglassNotification>;

struct DeviceRuntimeEvents {
    DeviceRuntimeQueue<NotificationEvent, 16> notificationQueue;
};

/// @brief Manages the overall state and logic of the Hourglass device.
struct DeviceRuntime {
    DeviceState::State deviceState = DeviceState::State::Off;
    DeviceState::State configState = DeviceState::State::Off;
    bool isActiveTurn = false;
    bool isConnected = false;

    char *deviceName;

    /// @brief Flag indicating if color-blind friendly colors should be used.
    bool colorBlindMode = false;

    /// @brief Flag indicating if the turn sequence should be oriented by the turn order, or by the device's player
    /// If true (absolute), the 1st player is at the top.
    /// If false (player-relative), this device's player is at the top.
    bool absoluteOrientation = true;

    bool forceDisplayUpdate = false;

    unsigned long lastTurnStart = 0;
    unsigned long lastUpdate = 0;
    unsigned long lastReadOut = 0;
    unsigned long lastConnection = 0;
    unsigned long lastDisconnection = 0;

    DeviceRuntimeEvents events;

    /// @brief The DeviceRuntime struct holds all of the new tasks that have been generated from the current update loop to be added to the DeviceManager's main task list at the end of the current update loop.
    /// This allows us to avoid modifying the main task list while it is being processed.
    /// This also allows us to add new tasks to the DeviceManager's queue through the runtime without needing to have direct access to the DeviceManager itself
    DeviceRuntimeQueue<ExecutableTask*, 16> pendingTasks;

    explicit DeviceRuntime(const char* name);

    void setDeviceState(DeviceState::State newState);
    void setConfigState(DeviceState::State newState);
    
};