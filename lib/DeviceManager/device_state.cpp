#include "device_state.h"

namespace DeviceState {
  const std::array<State, 2> ACTIVE_TURN_STATES = {
      State::ActiveTurnEnforced,
      State::ActiveTurnNotEnforced,
  };

  const std::array<State, 5> ACTIVE_GAME_STATES = {
      State::ActiveTurnEnforced,
      State::ActiveTurnNotEnforced,
      State::AwaitingTurn,
      State::Skipped,
      State::Paused};

bool isStateActiveTurn(State state)
{
    return std::find(ACTIVE_TURN_STATES.begin(), ACTIVE_TURN_STATES.end(), state) != ACTIVE_TURN_STATES.end();
}

bool isStateActiveGame(State state)
{
    return std::find(ACTIVE_GAME_STATES.begin(), ACTIVE_GAME_STATES.end(), state) != ACTIVE_GAME_STATES.end();
}
}