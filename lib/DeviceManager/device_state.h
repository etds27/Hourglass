#pragma once
#include <array>
#include <algorithm>

namespace DeviceState
{
  enum class State
  {
    Off = 0,
    AwaitingConnecion = 1,
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

  bool isStateActiveTurn(State state);

  bool isStateActiveGame(State state);
}