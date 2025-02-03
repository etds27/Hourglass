#pragma once
#include <array>
#include <algorithm>

namespace DeviceState
{
  
  /// @brief The current state of the device
  ///
  /// This enum represents all possible states the the peripheral device can be in
  /// Of all the states, only the AwaitingConneciton and Off states can be entered from the peripheral only
  /// All other states are commanded from the Central Device
  /// These states also encode the current game type that is happening
  /// 
  enum class State
  {
    Off = 0,
    AwaitingConnection = 1,
    AwaitingGameStart = 2,
    ActiveTurnEnforced = 3,
    ActiveTurnNotEnforced = 4,
    AwaitingTurn = 5,
    Skipped = 6,
    Paused = 7,
    Debug = 8,
    BuzzerAwaitingBuzz = 9,
    BuzzerAwaitingBuzzTimed = 10,
    BuzzerAwaitingTurnEnd = 11,
    BuzzerResults = 12,
    BuzzerWinnerPeriod = 13,
    BuzzerWinnerPeriodTimed = 14
  };

  /// @brief Collection of states where the button press event should be sent to the central device
  extern const std::array<State, 4> ACTIVE_TURN_STATES;

  /// @brief Collection of states where the game has started
  extern const std::array<State, 10> ACTIVE_GAME_STATES;

  /// @brief Collection of states where the Skip action is allowed
  extern const std::array<State, 4> SKIP_ELIGIBLE_STATES;

  /// @brief Array of all possible states the device can be in when in Sequential Turn Order mode
  extern const std::array<State, 7> SEQUENTIAL_TURN_ORDER_MODE_STATES;

  /// @brief Array of all possible states the device can be in when in Buzzer mode
  extern const std::array<State, 9> BUZZER_MODE_STATES;





  bool isStateActiveTurn(State state);

  bool isStateActiveGame(State state);

  bool isStateSkipEligible(State state);

}