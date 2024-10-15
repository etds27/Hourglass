#include <Arduino.h>
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include <EEPROM.h>
#include "device_state.h"
#include "fast_led_light.h"
/*
RingLight* m_ring;

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT); // use the LED as an output
  pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("ButtonLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);

  // add the service
  BLE.addService(ledService);

  ledCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");

  m_ring = new RingLight(RING_LED_COUNT, RING_DI_PIN, NEO_GRB + NEO_KHZ800);
  m_ring->setLightMode(DeviceState::AwaitingGameStart);
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();
  m_ring->update();

  if (!BLE.connected()) { return; }

  // read the current button pin state
  char buttonValue = digitalRead(buttonPin);

  // has the value changed since the last read
  bool buttonChanged = (buttonCharacteristic.value() != buttonValue);

  if (buttonChanged) {
    // button state changed, update characteristics
    ledCharacteristic.writeValue(buttonValue);
    buttonCharacteristic.writeValue(buttonValue);
  }

  if (ledCharacteristic.written() || buttonChanged) {
    // update LED, either central has written to characteristic or button state has changed
    if (ledCharacteristic.value()) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }
}
*/

// SevenSegmentDisplay* sevenSegment;
// DeviceManager* deviceManager;
// RingLight* m_ring;
// FastLEDLight* fastLEDLight;
DeviceManager *deviceManager;
// ButtonInputMonitor* buttonInputMonitor;

void setup()
{
  loggerLevel = LoggerLevel::DEBUG;
  Serial.begin(9600);
  while (!Serial)
    ;
  delay(2000);
  logger.info("Start of program");
  // Start the BLE peripheral

  
  EEPROM.begin(8);
  deviceManager = new DeviceManager();
  // deviceManager->writeDeviceName("FISCHER2", 8);
  // logger.info(String(deviceManager->getDeviceName()));
  deviceManager->start();
  
  
  // fastLEDLight = new FastLEDLight(16, 3);
  // fastLEDLight->setLightMode(DeviceState::Paused);
  //struct GameStartData data = { .totalPlayers = 4 };
  //fastLEDLight->updateAwaitingGameStartData(data);
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
  // fastLEDLight->update();
  // m_ring->updateTurnSequenceData(data);
  // m_ring->update();
  deviceManager->update();
  // buttonInputMonitor->getAction();
  // logger.info(String(digitalRead(BUTTON_INPUT_PIN)));
  // delay(1000);
}
