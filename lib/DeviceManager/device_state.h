#pragma once
#include <array>
#include <algorithm>

namespace DeviceState
{
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

  extern const std::array<State, 2> ACTIVE_TURN_STATES;

  extern const std::array<State, 5> ACTIVE_GAME_STATES;

  /// @brief Collection of states where the Skip action is allowed
  extern const std::array<State, 4> SKIP_ELIGIBLE_STATES;

  bool isStateActiveTurn(State state);

  bool isStateActiveGame(State state);

  bool isStateSkipEligible(State state);

}