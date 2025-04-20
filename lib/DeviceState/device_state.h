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
    BuzzerWinnerPeriodTimed = 14,
    BuzzerAlreadyAnswered = 15,
    BuzzerAwaitingBuzzerEnabled = 16,
    BuzzerAwaitingTurnStart = 17,

    Unknown = 100,
  };

  /// @brief Collection of states where the button press event should be sent to the central device
  extern const std::array<State, 9> ACTIVE_TURN_STATES;

  /// @brief Collection of states where the game has started
  extern const std::array<State, 12> ACTIVE_GAME_STATES;

  /// @brief Collection of states where the Skip action is allowed
  extern const std::array<State, 11> SKIP_ELIGIBLE_STATES;

  /// @brief Array of all possible states the device can be in when in Sequential Turn Order mode
  extern const std::array<State, 7> SEQUENTIAL_TURN_ORDER_MODE_STATES;

  /// @brief Array of all possible states the device can be in when in Buzzer mode
  extern const std::array<State, 12> BUZZER_MODE_STATES;

  /// @brief Array of all possible states that require time data to be displayed
  extern const std::array<State, 3> TIME_DATA_REQUIRED_STATE;

  /// @brief Array of all possible states that require game start data to be displayed
  extern const std::array<State, 1> GAME_START_DATA_REQUIRED_STATE;

  /// @brief Array of all possible states that require turn sequence data to be displayed
  extern const std::array<State, 1> TURN_SEQUENCE_DATA_REQUIRED_STATE;




  /// @brief Determines if the device is in a state where the turn is active
  /// @param state 
  /// @return 
  bool isStateActiveTurn(State state);

  /// @brief Determines if the device is in a state where the game has started
  bool isStateActiveGame(State state);

  /// @brief Determines if the device is in a state where the device is eligible to be skipped
  bool isStateSkipEligible(State state);

  /// @brief Determines if the device is in a state where the timer data is needed
  bool deviceStateRequiresTimeData(State state);

  /// @brief Determines if the device is in a state where game start data is needed
  bool deviceStateRequiresGameStartData(State state);

  /// @brief Determines if the device is in a state where the turn sequence data is needed
  bool deviceStateRequiresTurnSequenceData(State state);

}