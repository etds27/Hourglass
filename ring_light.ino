#include <EEPROM.h>
#include "ring_light.h"
#include "seven_segment.h"
#include "input_monitor.h"
#include "logger.h"
#include "ble_interface.h"

const int RING_LED_COUNT = 16;
const int RING_DI_PIN = 3;
RingLight *ring;
SevenSegmentDisplay *sevenSegment;
ButtonInputMonitor *buttonInputMonitor;
BLEInterface *bleInterface;

bool isTurn = false;


void setup() {
  loggerLevel = LoggerLevel::INFO;
  Serial.begin(9600);
  delay(1000);
  BLE.begin();
  logger.info("Start of program");

  sevenSegment = new SevenSegmentDisplay(12, 11, 10, 9, 8, 7, 6, 5);
  buttonInputMonitor = new ButtonInputMonitor(4);
  // digitalWrite(7, LOW);
  // sevenSegment->setAll();
  // sevenSegment->set(6);

  ring = new RingLight(RING_LED_COUNT, RING_DI_PIN, NEO_GRB + NEO_KHZ800);
  ring->setBrightness(5);
  ring->setLightMode(DeviceState::Off);
  sevenSegment->set(0);
  bleInterface->setService(0);
  logger.info("Finished device setup");
}


void loop() {
  if (millis() % 2000 == 0 && bleInterface->isConnected()) {
    bleInterface->readData();
  }

  if (buttonInputMonitor->getAction() == ButtonInputType::ButtonPress && isTurn) {
    // If a button was pressed and it is the person's turn
    bleInterface->endTurn();
  }

  if (bleInterface->isTurn() != isTurn && !isTurn) {
    // If the turn just started
    logger.info("Turn Started");
    int timer = bleInterface->getTimer();
    struct TimerData data = { .totalTime = timer, .remainingTme = 1 };
    ring->updateTimerData(data);
    ring->setLightMode(DeviceState::Timer);
    sevenSegment->set(1);
    isTurn = true;
  } else if (bleInterface->isTurn() != isTurn && isTurn) {
    // If the turn just ended
    ring->setLightMode(DeviceState::Off);
    sevenSegment->set(0);

    isTurn = false;
  }

  /*
  if (bleInterface->isConnected()) {
    sevenSegment->set(1);
    while(bleInterface->isConnected()) {
      bleInterface->readData();
      delay(2000);
    };
    sevenSegment->set(0);
  }
  */
  /*
  ButtonInputType inputType = buttonInputMonitor->getAction();
  if (inputType == ButtonInputType::ButtonPress) {
    sevenSegment->set(1);
  }
    if (inputType == ButtonInputType::DoubleButtonPress) {
    sevenSegment->set(2);
  }
    if (inputType == ButtonInputType::TripleButtonPress) {
    sevenSegment->set(3);
  }
    if (inputType == ButtonInputType::LongPress) {
    sevenSegment->set(4);
  }
  /*
  while(true) {
    for (int i = 0; i < 10; i++) {
      sevenSegment->set(i);
      delay(500);
    }
  }
  */
  ring->update();
}
