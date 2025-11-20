#include "mock_central_interface.h"
#include "logger.h"

namespace
{
    const LogString loggerTag = "MockCentralInterface";
}

MockCentralInterface::MockCentralInterface()
{
    m_lastElapsedTimePoll = 0;
}

void MockCentralInterface::poll() {}

MockCentralInterface::~MockCentralInterface() = default;

bool MockCentralInterface::isConnected() { return m_connected; }

bool MockCentralInterface::isTurn()
{
    return DeviceState::isStateActiveTurn(m_commandedDeviceState);
}

void MockCentralInterface::endTurn()
{
    m_endTurnCalled = true;
}

void MockCentralInterface::toggleSkippedState()
{
    m_toggleSkipCalled = true;
}

int MockCentralInterface::getTimer() { return m_timer; }

void MockCentralInterface::readData()
{
    // No-op for mock
}

int MockCentralInterface::getCurrentPlayer() { return m_currentPlayer; }

int MockCentralInterface::getTotalPlayers() { return m_totalPlayers; }

ColorConfig MockCentralInterface::readColorConfig() { return m_colorConfig; }

uint8_t MockCentralInterface::readDeviceLEDOffset()
{
    return m_ledOffset;
}

uint8_t MockCentralInterface::readDeviceLEDCount()
{
    return m_ledCount;
}

void MockCentralInterface::getDeviceName(char *out, uint8_t length)
{
    strncpy(out, m_deviceName, length);
    out[length - 1] = '\0';
}

DeviceState::State MockCentralInterface::getDeviceColorConfigState() { return m_configState; }

int MockCentralInterface::getMyPlayer() { return m_myPlayer; }

bool MockCentralInterface::isGameActive()
{
    return DeviceState::isStateActiveGame(m_commandedDeviceState);
}

bool MockCentralInterface::isGamePaused()
{
    return m_commandedDeviceState == DeviceState::State::Paused;
}

DeviceState::State MockCentralInterface::getCommandedDeviceState() { return m_commandedDeviceState; }

uint16_t MockCentralInterface::getSkippedPlayers() { return m_skippedPlayers; }

bool MockCentralInterface::isTurnTimerEnforced()
{
    return m_commandedDeviceState == DeviceState::State::ActiveTurnEnforced;
}

void MockCentralInterface::setService()
{
    // No-op for mock
}

void MockCentralInterface::sendDeviceName(const char *name)
{
    strncpy(m_sentDeviceName, name, MAX_NAME_LENGTH);
    m_sentDeviceName[MAX_NAME_LENGTH - 1] = '\0';
}

void MockCentralInterface::sendDeviceColorConfig(ColorConfig config)
{
    m_sentColorConfig = config;
}

// --- Mock control methods ---

void MockCentralInterface::setConnected(bool connected) { m_connected = connected; }

void MockCentralInterface::setCommandedDeviceState(DeviceState::State state) { m_commandedDeviceState = state; }

void MockCentralInterface::setTimer(int timer) { m_timer = timer; }

void MockCentralInterface::setElapsedTime(int elapsedTime)
{
    m_lastElapsedTime = elapsedTime;
    m_lastElapsedTimePoll = millis();
}

void MockCentralInterface::setCurrentPlayer(int player) { m_currentPlayer = player; }

void MockCentralInterface::setTotalPlayers(int total) { m_totalPlayers = total; }

void MockCentralInterface::setMyPlayer(int player) { m_myPlayer = player; }

void MockCentralInterface::setSkippedPlayers(uint16_t skipped) { m_skippedPlayers = skipped; }

void MockCentralInterface::triggerDeviceNameChange(char *name)
{
    logger.info(loggerTag, ": triggerDeviceNameChange called with name: ", name);
    if (m_deviceNameChangeCallback)
    {
        logger.info(loggerTag, ": Invoking device name change callback");
        m_deviceNameChangeCallback(name);
    }
}

void MockCentralInterface::triggerDeviceNameWrite(bool write)
{
    if (m_deviceNameWriteChangeCallback)
        m_deviceNameWriteChangeCallback(write);
}

void MockCentralInterface::triggerDeviceColorConfigChange(ColorConfig config)
{
    m_colorConfig = config;
    if (m_deviceColorConfigChangeCallback)
        m_deviceColorConfigChangeCallback(config);
}

void MockCentralInterface::triggerDeviceColorConfigStateChange(DeviceState::State state)
{
    m_configState = state;
    if (m_deviceColorConfigStateChangeCallback)
        m_deviceColorConfigStateChangeCallback(state);
}

void MockCentralInterface::triggerDeviceColorConfigWrite(bool write)
{
    if (m_deviceColorConfigWriteChangeCallback)
        m_deviceColorConfigWriteChangeCallback(write);
}

// --- Methods to inspect mock state ---

bool MockCentralInterface::wasEndTurnCalled() const { return m_endTurnCalled; }
bool MockCentralInterface::wasToggleSkipCalled() const { return m_toggleSkipCalled; }
const char *MockCentralInterface::getSentDeviceName() const { return m_sentDeviceName; }
const ColorConfig &MockCentralInterface::getSentColorConfig() const { return m_sentColorConfig; }