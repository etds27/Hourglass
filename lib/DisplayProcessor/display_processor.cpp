#include "display_processor.h"
#include "logger.h"

namespace
{
  const LogString loggerTag = "DeviceManager";
}

DisplayProcessor::DisplayProcessor() {}

bool DisplayProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{
    DeviceState::State state = runtime->deviceState;
    HGCentralInterface *centralInterface = context->centralInterface;
    HourglassDisplayManager *displayManager = context->displayManager;
    DeviceState::State displayState;

    displayManager->setAbsoluteOrientation(runtime->absoluteOrientation);
    displayManager->setColorBlindMode(runtime->colorBlindMode);

    // If in configuration mode, display the configured state instead if it doesnt require auxiliary data
    if (state == DeviceState::State::ConfigurationMode)
    {
        DeviceState::State configState = centralInterface->getDeviceColorConfigState();

        if (DeviceState::deviceStateRequiresAuxiliaryData(configState))
        {
            logger.info(loggerTag, ": updateConfigDisplay: New config state requires auxiliary data: ", static_cast<int>(configState));
            displayState = DeviceState::State::Off;
        }
    }
    else
    {
        // Normal mode, just use the current state
        displayState = state;
    }

    // Update the display configuration for the AwaitingGameStart state
    if (displayState == DeviceState::State::AwaitingGameStart)
    {
        GameStartData data{.totalPlayers = centralInterface->getTotalPlayers()};
        displayManager->updateAwaitingGameStartData(data);
    }

    // Update turn sequence data for states that require it
    if (DeviceState::deviceStateRequiresTurnSequenceData(displayState))
    {
        TurnSequenceData data{
            .totalPlayers = centralInterface->getTotalPlayers(),
            .myPlayerIndex = centralInterface->getMyPlayer(),
            .currentPlayerIndex = centralInterface->getCurrentPlayer(),
            .skippedPlayers = centralInterface->getSkippedPlayers()};
        displayManager->updateTurnSequenceData(data);
    }

    // Update timer data for states that require it
    if (DeviceState::deviceStateRequiresTimeData(displayState))
    {
        TimerData data{.totalTime = centralInterface->getTimer(),
                       .elapsedTime = centralInterface->getElapsedTime(),
                       .isTurnTimeEnforced = centralInterface->isTurnTimerEnforced()};

        displayManager->updateTimerData(data);
    }

    displayManager->setDisplayMode(displayState);

    displayManager->update(runtime->forceDisplayUpdate);
    return true;
}
