#pragma once
#include <stdint.h>
#include "constants.h"
#include "colors.h"
#include "device_state.h"
#include "hg_display_interface.h"

class LightInterface : public HGDisplayInterface
{
protected:
  uint8_t m_ledCount;
  uint8_t m_diPin;

  void updateLightModeActiveTurn();
  void updateLightModeActiveTurnTimer();
  void updateLightModeActiveTurnNoTimer();
  void updateLightModeSkipped();
  void updateLightModeTurnSequence();
  void updateLightModeAwaitGameStart();
  void updateLightModeAwaitConnection();
  void updateGamePaused();

  // Take a buffer of size (2, 4, 8) and populate a full 16 item buffer using the condensed buffer
  // The fill option will fill in the space between the
  void expandBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size, bool fill = true);

  // Replicate the smaller buffer to fit into the full buffer
  void extendBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size);

  // Rotate the full sized color buffer by some offset
  void offsetBuffer(uint32_t *buffer, uint8_t offset);

  // Reverse the full sized color buffer
  void reverseBuffer(uint32_t *buffer, uint8_t offset);

  /// @brief Overlays the overlay buffer on top of the base buffer
  /// Blank/Black values in the overlay buffer will not overwrite the base buffer
  /// The base buffer will be updated with the new result
  /// @param baseBuffer Buffer to modify in place with overlayed content
  /// @param overlayBuffer Buffer to overlay on the base buffer
  /// @param inverse If set, the overlay only blank leds for negative light designs
  void overlayBuffer(uint32_t *baseBuffer, const uint32_t *overlayBuffer, uint8_t bufferSize, bool inverse = false);

  /// @brief Set the provided buffer to a single solid color
  /// @param buffer Buffer to modify in-place
  /// @param bufferSize Size of the provided buffer
  /// @param color Color to set the buffer to
  void solidBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color);

  /// @brief Displays the full sized buffer to the light interface
  /// @param buffer 
  void displayBuffer(const uint32_t *buffer);

  /// @brief Set a buffer to a specific color for all non zero buffer values
  /// @param buffer Buffer to update colors of in place
  /// @param bufferSize Size of the provided buffer
  /// @param color Color to change buffer to
  void colorBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color);

  // Linearly interpolate the colors provided by their RGB channels
  uint32_t interpolateColors(uint32_t color1, uint32_t color2, double pct);
  uint32_t interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct);

public:
  LightInterface(const uint8_t ledCount = 16, const uint8_t diPin = 0);
  virtual ~LightInterface();

  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void setPixelColor(uint8_t i, uint32_t color) = 0;

  // Non enforced turn timer
  unsigned long m_lastColorChange;
  uint32_t m_targetColor;
  uint32_t m_previousColor;

  void setDisplayMode(DeviceState state);
};
