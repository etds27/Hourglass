#include <EEPROM.h>
#include "device_manager.h"
#include "logger.h"

DeviceManager::DeviceManager() {
  m_deviceName = getDeviceName();
  m_interface = new BLEInterface(m_deviceName);
}

char* getDeviceName() {
  return m_deviceName;
}

char* readDeviceName() {
  int i;
  // Read the arduinos ID
  char arduinoID[8];
  unsigned char value;
  for (i = 0; i < 8; i++) {
    value = EEPROM.read(i);
    arduinoID[i] = value;
  }
  arduinoID[i] = '\0';
  logger.info("Device name: " + String(arduinoID));
  return arduinoID;
}

void writeDeviceName(const char* deviceName) {
  // Write unique ID to EEPROM
  int i;
  char arduinoData[9] = deviceName;
  logger.info("Writing Device Name: " + String(arduinoData));
  for (i = 0; i < 8; i++) {
    EEPROM.write(i, arduinoData[i]);
  }
  EEPROM.write(i, '\0');
  logger.info("Wrote device name to EEPROM: " +  String(deviceName));
  m_deviceName = deviceName;
}

bool DeviceManager::getTurn() { 
  return m_isTurn; 

}
void DeviceManager::updateTurn(bool turn) {
  if (turn != getTurn()) {
    if (turn) {
      // The current turn state was changed to true
    } else {
      // The current turn state was changed to false
    }
  }
}

void DeviceManager::handleButtonInput(ButtonInputType inputType) {
  switch (inputType) {
    case ButtonInputType::ButtonPress: 
      handleSingleButtonPress();
      break;
    case ButtonInputType::None:
      break;
  }
}

void DeviceManager::handleSingleButtonPress() {
  if (getTurn()) {
    endTurn();
  }
}

void DeviceManager::endTurn() {

}
