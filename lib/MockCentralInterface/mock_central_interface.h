#pragma once

#include "hg_central_interface.h"
#include "device_state.h"
#include "device_config.h"
#include <cstring>

class MockCentralInterface : public HGCentralInterface
{
public:
    MockCentralInterface();
    ~MockCentralInterface() override;

    // HGCentralInterface implementation
    void poll() override;
    bool isConnected() override;
    bool isTurn() override;
    void endTurn() override;
    void toggleSkippedState() override;
    int getTimer() override;
    void readData() override;
    int getCurrentPlayer() override;
    int getTotalPlayers() override;
    ColorConfig readColorConfig() override;
    int8_t readDeviceLEDOffset() override;
    uint8_t readDeviceLEDCount() override;

    void getDeviceName(char *out, uint8_t length) override;
    DeviceState::State getDeviceColorConfigState() override;
    int getMyPlayer() override;
    bool isGameActive() override;
    bool isGamePaused() override;
    DeviceState::State getCommandedDeviceState() override;
    uint16_t getSkippedPlayers() override;
    bool isTurnTimerEnforced() override;
    void setService() override;
    void sendDeviceName(const char *name) override;
    void sendDeviceColorConfig(ColorConfig config) override;

    // --- Mock control methods ---

    void setConnected(bool connected);
    void setCommandedDeviceState(DeviceState::State state);
    void setTimer(int timer);
    void setElapsedTime(int elapsedTime);
    void setCurrentPlayer(int player);
    void setTotalPlayers(int total);
    void setMyPlayer(int player);
    void setSkippedPlayers(uint16_t skipped);

    // Methods to trigger callbacks
    void triggerDeviceNameChange(char *name);
    void triggerDeviceNameWrite(bool write);
    void triggerDeviceColorConfigChange(ColorConfig config);
    void triggerDeviceColorConfigStateChange(DeviceState::State state);
    void triggerDeviceColorConfigWrite(bool write);

    // Methods to inspect mock state
    bool wasEndTurnCalled() const;
    bool wasToggleSkipCalled() const;
    const char *getSentDeviceName() const;
    const ColorConfig &getSentColorConfig() const;

private:
    bool m_connected = false;
    DeviceState::State m_commandedDeviceState = DeviceState::State::Off;
    int m_timer = 0;
    int m_currentPlayer = 0;
    int m_totalPlayers = 0;
    int m_myPlayer = 0;
    uint16_t m_skippedPlayers = 0;
    ColorConfig m_colorConfig;
    char m_deviceName[MAX_NAME_LENGTH] = "MockDevice";
    DeviceState::State m_configState = DeviceState::State::Off;
    uint8_t m_ledOffset = 0;
    uint8_t m_ledCount = 16;

    // State for inspection
    bool m_endTurnCalled = false;
    bool m_toggleSkipCalled = false;
    char m_sentDeviceName[MAX_NAME_LENGTH] = "";
    ColorConfig m_sentColorConfig;
};