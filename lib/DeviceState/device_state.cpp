#include "device_state.h"

namespace DeviceState
{
    const std::array<State, 4> ACTIVE_TURN_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed
    };

    const std::array<State, 10> ACTIVE_GAME_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped,
        State::Paused,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,
        State::BuzzerAwaitingTurnEnd,
        State::BuzzerWinnerPeriod,
        State::BuzzerWinnerPeriodTimed};

    const std::array<State, 4> SKIP_ELIGIBLE_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped};

    const std::array<State, 7> SEQUENTIAL_TURN_ORDER_MODE_STATES = {
        State::AwaitingConnection,
        State::AwaitingGameStart,
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped,
        State::Paused};

    const std::array<State, 9> BUZZER_MODE_STATES = {
        State::AwaitingConnection,
        State::AwaitingGameStart,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,
        State::BuzzerWinnerPeriod,
        State::BuzzerWinnerPeriodTimed,
        State::BuzzerAwaitingTurnEnd,
        State::Skipped,
        State::Paused};

    // Off = 0,
    // AwaitingConnection = 1,
    // AwaitingGameStart = 2,
    // ActiveTurnEnforced = 3,
    // ActiveTurnNotEnforced = 4,
    // AwaitingTurn = 5,
    // Skipped = 6,
    // Paused = 7,
    // Debug = 8

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
