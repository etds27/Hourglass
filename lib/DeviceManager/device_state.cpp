#include "device_state.h"

namespace DeviceState
{
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

    const std::array<State, 4> SKIP_ELIGIBLE_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped};

    bool isStateActiveTurn(State state)
    {
        return std::find(ACTIVE_TURN_STATES.begin(), ACTIVE_TURN_STATES.end(), state) != ACTIVE_TURN_STATES.end();
    }

    bool isStateActiveGame(State state)
    {
        return std::find(ACTIVE_GAME_STATES.begin(), ACTIVE_GAME_STATES.end(), state) != ACTIVE_GAME_STATES.end();
    }

    bool isStateSkipEligible(State state)
    {
        return std::find(SKIP_ELIGIBLE_STATES.begin(), SKIP_ELIGIBLE_STATES.end(), state) != SKIP_ELIGIBLE_STATES.end();
    }
}
