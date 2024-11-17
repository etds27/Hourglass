#include <Arduino.h>
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include <EEPROM.h>
#include "device_state.h"
#include "fast_led_light.h"

// SevenSegmentDisplay* sevenSegment;
// DeviceManager* deviceManager;
// RingLight* m_ring;
FastLEDLight* fastLEDLight;
DeviceManager *deviceManager;
// ButtonInputMonitor* buttonInputMonitor;

void setup()
{
  loggerLevel = LoggerLevel::DEBUG;
  Serial.begin(9600);
  // while (!Serial)
  //   ;
  delay(2000);
  logger.info("Start of program");
  // Start the BLE peripheral

  EEPROM.begin(8);
  deviceManager = new DeviceManager();
  // deviceManager->writeDeviceName("FISCHER7", 8);
  logger.info(String(deviceManager->getDeviceName()));
  deviceManager->start();

  // exit(0);

  // fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  // struct TimerData data {.totalTime = 1, .elapsedTime = 1, .isTurnTimeEnforced = false};

  // fastLEDLight->updateTimerData(data);
  // fastLEDLight->setDisplayMode(DeviceState::ActiveTurn);

  // struct GameStartData data = { .totalPlayers = 4 };
  // fastLEDLight->updateAwaitingGameStartData(data);
  // fastLEDLight->setLightMode(DeviceState::Paused);

  // struct TurnSequenceData data = {.totalPlayers = 8, .myPlayerIndex = 1, .currentPlayerIndex = 6};
  // fastLEDLight->updateTurnSequenceData(data);
  // fastLEDLight->setLightMode(DeviceState::AwaitingTurn);

  /*
  //
  logger.info("Finished device setup");
  // sevenSegment = new SevenSegmentDisplay(12, 11, 10, 9, 8, 7, 6, 5);
  // digitalWrite(7, LOW);
  // sevenSegment->setAll();
  // sevenSegment->set(6);

  // m_ring = new RingLight(RING_LED_COUNT, RING_DI_PIN, NEO_GRB + NEO_KHZ800);
  // m_ring->setLightMode(DeviceState::AwaitingConnecion);
  // delay(2000);
  // sevenSegment->set(0);
  */
  // buttonInputMonitor = new ButtonInputMonitor(4);
  // pinMode(BUTTON_INPUT_PIN, INPUT);
}

// unsigned long start = millis();

void loop()
{

  // int diff = (millis() - start) / 5000;
  // struct GameStartData data = { .totalPlayers = diff + 1 };
  // fastLEDLight->updateAwaitingGameStartData(data);
  // logger.info("Updating " + String(freeMemory()));
  // struct TurnSequenceData data = { .totalPlayers = 10, .myPlayerIndex = 7, .currentPlayerIndex = (millis() / 1000) % 10 };
  // logger.info("TEST");
  // fastLEDLight->update();
  // m_ring->updateTurnSequenceData(data);
  // m_ring->update();
  deviceManager->update();
  // buttonInputMonitor->getAction();
  // logger.info(String(digitalRead(BUTTON_INPUT_PIN)));
  // delay(1000);
}
