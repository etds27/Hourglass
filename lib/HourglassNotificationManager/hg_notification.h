#pragma once

#include <stdint.h>

enum class HourglassNotification: uint32_t {
    HG_NOTIFICATION_TURN_STARTED = 0,
    HG_NOTIFICATION_USER_POKE = 1,
    HG_NOTIFICATION_DEVICE_CONNECTED = 2
};