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
    Debug = 8
  };

  /// @brief Array of all possible states the device can be in when in Sequential Turn Order mode
  extern const std::array<State, 7> SEQUENTIAL_TURN_ORDER_MODE_STATES;

  /// @brief Array of all possible states the device can be in when in Buzzer mode
  extern const std::array<State, 6> BUZZER_MODE_STATES;

  extern const std::array<State, 2> ACTIVE_TURN_STATES;

  extern const std::array<State, 5> ACTIVE_GAME_STATES;

  /// @brief Collection of states where the Skip action is allowed
  extern const std::array<State, 4> SKIP_ELIGIBLE_STATES;

  bool isStateActiveTurn(State state);

  bool isStateActiveGame(State state);

  bool isStateSkipEligible(State state);

}