#include <EEPROM.h>
#include "constants.h"
#include "seven_segment.h"
#include "logger.h"
#include "device_manager.h"
#include "ring_light.h"
#include "device_state.h"



// SevenSegmentDisplay* sevenSegment;
DeviceManager* deviceManager;
RingLight* m_ring;

void setup() {
  loggerLevel = LoggerLevel::INFO;
  Serial.begin(9600);
  delay(2000);
  Serial.println("TEST");

  logger.info("Start of program");
  BLE.begin();

  deviceManager = new DeviceManager();
  deviceManager->start();
  logger.info("Finished device setup");
  // sevenSegment = new SevenSegmentDisplay(12, 11, 10, 9, 8, 7, 6, 5);
  // buttonInputMonitor = new ButtonInputMonitor(4);
  // digitalWrite(7, LOW);
  // sevenSegment->setAll();
  // sevenSegment->set(6);

  m_ring = new RingLight(RING_LED_COUNT, RING_DI_PIN, NEO_GRB + NEO_KHZ800);
  m_ring->setLightMode(DeviceState::AwaitingConnecion);
  //delay(2000);
  // sevenSegment->set(0);

}


void loop() {
  //struct TurnSequenceData data = { .totalPlayers = 10, .myPlayerIndex = 7, .currentPlayerIndex = (millis() / 1000) % 10 };
  //m_ring->updateTurnSequenceData(data);
  m_ring->update();
  // deviceManager->update();
}
