#pragma once
#include "colors.h"

#define ENABLE_DEBUG 0


// Display Type Enumeration
// Adafruit: 0
// FastLED:  1
// LCD:      2
#define DISPLAY_TYPE 2

#define LED_ADAFRUIT 0

#define RING_LED_COUNT 16
#define RING_DI_PIN 5
#define BUTTON_INPUT_PIN 4
#define BUTTON_GPIO_PIN GPIO_NUM_4

#define CONNECTION_TIMEOUT 120000


/// Amount of time to wait after the BLE device has connected to allow Central device to discover all characteristics
#define EXPECTED_CHARACTERISTIC_DISCOVERY 1500

#define MIN_TURN_LENGTH 1000

// Lighting constants

// All Hourglass devices have the ring LED installed in the same orientation. 
// This orientation does not have the first LED at the top of the ring
// This offset translates the display buffer to be displayed at the top of the ring.
#define TOP_RING_OFFSET 14
#define DEFAULT_BRIGHTNESS 25
#define DISPLAY_REFRESH_RATE 5  // ms

#define TIMER_COLOR_1 GREEN
#define TIMER_COLOR_ALT_1 0x33BBEE
#define TIMER_COLOR_2 YELLOW
#define TIMER_COLOR_ALT_2 0xEE7733
#define TIMER_COLOR_3 RED
#define TIMER_COLOR_ALT_3 0xCC3311

#define NO_TIMER_SPEED 200
// Must divide evenly into the total LED count
#define NO_TIMER_SEGMENTS 4

#define NO_TIMER_APPLY_OFFSET 1
#define NO_TIMER_COLOR WHITE

#define PAUSED_NEW_COLOR_PERIOD 3000
#define PAUSED_BLANK_SPEED 200

#define SKIPPED_COLOR GRAY
#define SKIPPED_MIN_BRIGHTNESS 0
#define SKIPPED_MAX_BRIGHTNESS 20
#define SKIPPED_PULSE_DURATION 4000 // milliseconds

#define EXPAND_TURN_SEQUENCE_BUFFER 1
#define UNIFORM_SEQUENCES_REQUIRED 0
#define MY_PLAYER_COLOR BLUE
#define MY_PLAYER_COLOR_ALT 0x004488
#define OTHER_PLAYER_COLOR PINK
#define OTHER_PLAYER_COLOR_ALT 0xDDAA33
#define CURRENT_PLAYER_COLOR GREEN
#define CURRENT_PLAYER_COLOR_ALT 0xBB5566
#define CURRENT_PLAYER_PULSE_DURATION 4000 // milliseconds
#define CURRENT_PLAYER_MIN_BRIGHTNESS 5
#define CURRENT_PLAYER_MAX_BRIGHTNESS 20

#define AWAIT_GAME_START_SPEED 200
#define AWAIT_GAME_COLOR1 GREEN
#define AWAIT_GAME_COLOR2 RED
#define AWAIT_GAME_COLOR3 BLUE
#define AWAIT_GAME_COLOR4 YELLOW
#define AWAIT_GAME_COLOR5 CYAN
#define AWAIT_GAME_COLOR6 PURPLE
#define AWAIT_GAME_COLOR7 ORANGE
#define AWAIT_GAME_COLOR8 WHITE
#define AWAIT_GAME_COLOR9 NAVY
#define AWAIT_GAME_COLOR10 MINT
#define AWAIT_GAME_COLOR11 PINK
#define AWAIT_GAME_COLOR12 MAROON
#define AWAIT_GAME_COLOR13 OLIVE
#define AWAIT_GAME_COLOR14 TEAL
#define AWAIT_GAME_COLOR15 BROWN
#define AWAIT_GAME_COLOR16 LILAC
#define AWAIT_GAME_COLOR_ALT1 0xEE3377
#define AWAIT_GAME_COLOR_ALT2 0x0077BB
#define AWAIT_GAME_COLOR_ALT3 0xFF0000
#define AWAIT_GAME_COLOR_ALT4 0xFF8000
#define AWAIT_GAME_COLOR_ALT5 0xCC3311
#define AWAIT_GAME_COLOR_ALT6 0x009988
#define AWAIT_GAME_COLOR_ALT7 0xEE7733
#define AWAIT_GAME_COLOR_ALT8 0xBBBBBB
#define AWAIT_GAME_COLOR_ALT9 AWAIT_GAME_COLOR_ALT1
#define AWAIT_GAME_COLOR_ALT10 AWAIT_GAME_COLOR_ALT2
#define AWAIT_GAME_COLOR_ALT11 AWAIT_GAME_COLOR_ALT3
#define AWAIT_GAME_COLOR_ALT12 AWAIT_GAME_COLOR_ALT4
#define AWAIT_GAME_COLOR_ALT13 AWAIT_GAME_COLOR_ALT5
#define AWAIT_GAME_COLOR_ALT14 AWAIT_GAME_COLOR_ALT6
#define AWAIT_GAME_COLOR_ALT15 AWAIT_GAME_COLOR_ALT7
#define AWAIT_GAME_COLOR_ALT16 AWAIT_GAME_COLOR_ALT8

#define AWAIT_CONNECTION_SPEED 200
#define AWAIT_CONNECTION_COLOR BLUE
#define AWAIT_CONNECTION_COLOR_ALT CYAN
