#include "device_state.h"

namespace DeviceState
{

    const uint8_t NUMBER_OF_STATES = 19;


    const std::array<State, 9> ACTIVE_TURN_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,
        State::BuzzerWinnerPeriod,
        State::BuzzerWinnerPeriodTimed,
        State::BuzzerAwaitingTurnEnd,
        State::BuzzerAwaitingBuzzerEnabled,
        State::BuzzerResults,
    };

    const std::array<State, 12> ACTIVE_GAME_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped,
        State::Paused,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,
        State::BuzzerAwaitingTurnEnd,
        State::BuzzerWinnerPeriod,
        State::BuzzerWinnerPeriodTimed,
        State::BuzzerAwaitingTurnStart, 
        State::BuzzerAwaitingBuzzerEnabled};

    const std::array<State, 11> SKIP_ELIGIBLE_STATES = {
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped,
        State::BuzzerAwaitingTurnEnd,
        State::BuzzerResults,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,        
        State::BuzzerAwaitingTurnStart, 
        State::BuzzerAwaitingBuzzerEnabled,};

    const std::array<State, 7> SEQUENTIAL_TURN_ORDER_MODE_STATES = {
        State::AwaitingConnection,
        State::AwaitingGameStart,
        State::ActiveTurnEnforced,
        State::ActiveTurnNotEnforced,
        State::AwaitingTurn,
        State::Skipped,
        State::Paused};

    const std::array<State, 12> BUZZER_MODE_STATES = {
        State::AwaitingConnection,
        State::AwaitingGameStart,
        State::BuzzerAwaitingBuzz,
        State::BuzzerAwaitingBuzzTimed,
        State::BuzzerWinnerPeriod,
        State::BuzzerWinnerPeriodTimed,
        State::BuzzerAwaitingTurnEnd,
        State::BuzzerAwaitingBuzzerEnabled,
        State::BuzzerResults,
        State::BuzzerAwaitingTurnStart, 
        State::Skipped,
        State::Paused};


    const std::array<State, 3> TIME_DATA_REQUIRED = {
        State::ActiveTurnEnforced,
        State::BuzzerWinnerPeriodTimed,
        State::BuzzerAwaitingBuzzTimed};

    const std::array<State, 1> GAME_START_DATA_REQUIRED = {
        State::AwaitingGameStart};

    const std::array<State, 1> TURN_SEQUENCE_DATA_REQUIRED = {
        State::AwaitingTurn};

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

    bool deviceStateRequiresTimeData(State state)
    {
        return std::find(TIME_DATA_REQUIRED.begin(), TIME_DATA_REQUIRED.end(), state) != TIME_DATA_REQUIRED.end();
    }

    bool deviceStateRequiresGameStartData(State state)
    {
        return std::find(GAME_START_DATA_REQUIRED.begin(), GAME_START_DATA_REQUIRED.end(), state) != GAME_START_DATA_REQUIRED.end();
    }

    bool deviceStateRequiresTurnSequenceData(State state)
    {
        return std::find(TURN_SEQUENCE_DATA_REQUIRED.begin(), TURN_SEQUENCE_DATA_REQUIRED.end(), state) != TURN_SEQUENCE_DATA_REQUIRED.end();
    }
    
    bool deviceStateRequiresAuxiliaryData(State state)
    {
        return deviceStateRequiresGameStartData(state) || deviceStateRequiresTurnSequenceData(state) || deviceStateRequiresTimeData(state);
    }
}
