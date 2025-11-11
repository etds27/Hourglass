#pragma once
#include <stdint.h>
#include "constants.h"
#include "colors.h"
#include "device_state.h"
#include "hg_display_interface.h"
#include "easing_function.h"
#include "color_converter.h"

// All required data for any display interface to show the Awaiting Game Start state
struct GameDebugData
{
  uint32_t *buffer;
};

class LightInterface : public HGDisplayInterface
{
protected:
  uint8_t m_ringOffset = TOP_RING_OFFSET;
  uint8_t m_ledCount;
  uint8_t m_diPin;

  virtual uint8_t getRingOffset() const;

  void updateLightModeActiveTurnTimer() override;
  void updateLightModeActiveTurnNoTimer() override;
  void updateLightModeSkipped() override;
  void updateLightModeTurnSequence() override;
  void updateLightModeAwaitGameStart() override;
  void updateLightModeAwaitConnection() override;
  void updateGamePaused() override;
  void updateGameDebug() override;

  void updateLightModeBuzzerAwaitingBuzz() override;
  void updateLightModeBuzzerAwaitingBuzzTimed() override;
  void updateLightModeWinnerPeriod() override;
  void updateLightModeWinnerPeriodTimed() override;
  void updateLightModeBuzzerResults() override;
  void updateLightModeAwaitTurnStart() override;
  void updateDeviceColorMode() override;



  // MARK: Canned display effects
  void displayMarqueeBuzzer(uint32_t color);
  void displayCounterClockwiseTimer(uint32_t elapsedTime, uint32_t totalTimerDuration);
  void displaySymmetricFixedColorTimer(uint32_t elapsedTime, uint32_t totalTimerDuration);


  /// @brief Displays the full sized buffer to the light interface
  /// @param buffer
  void displayBuffer(const uint32_t *buffer, const bool clockwise = true);

  /// @brief Obtain the current segment of the display cycle adjusted by the easing function
  /// @param cycleLength Length of a single cycle in milliseconds
  /// @param cycleStartTime Start time of a cycle in milliseconds (this can be the display mode change time)
  /// @param totalSteps Total number of steps in a cycle
  /// @param easingFunction Easing Function to adjust the cycle completion percentage to
  /// @return The current step in the cycle adjusted for the Easing Function
  uint8_t getAdjustedCycleSegment(unsigned long cycleDuration, unsigned long cycleStartTime, uint8_t totalCycleSteps, EasingFunction::EasingFunction *easingFunction);

  /// @brief Linearly interpolate the colors provided by their RGB channels
  /// @param color1 Color to use for interpolation
  /// @param color2 Color to use for interpolation
  /// @param pct Percent color shifting between colors provided
  /// @return Updated color
  uint32_t interpolateColors(uint32_t color1, uint32_t color2, double pct);

  /// @brief Linearly interpolate the colors provided by their RGB channels
  /// @param colorR1 Color red channel
  /// @param colorG1 Color green channel
  /// @param colorB1 Color blue channel
  /// @param colorR2 Color red channel
  /// @param colorG2 Color green channel
  /// @param colorB2 Color blue channel
  /// @param pct Percent color shifting between colors provided
  /// @return Updated color
  uint32_t interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct);

  /// @brief Interpolate the colors provided by their RGB channels using the provided easing function
  /// @param color1
  /// @param color2
  /// @param pct Percent color shifting between colors provided
  /// @param easingFunction Function to adjust percentage to expected curve
  /// @return Updated color
  uint32_t interpolateColors(uint32_t color1, uint32_t color2, double pct, EasingFunction::EasingFunction *easingFunction);

  /// @brief Interpolate the colors provided by their RGB channels using the provided easing function
  /// @param colorR1 Color red channel
  /// @param colorG1 Color green channel
  /// @param colorB1 Color blue channel
  /// @param colorR2 Color red channel
  /// @param colorG2 Color green channel
  /// @param colorB2 Color blue channel
  /// @param pct Percent color shifting between colors provided
  /// @param easingFunction Function to adjust percentage to expected curve
  /// @return Updated color
  uint32_t interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct, EasingFunction::EasingFunction *easingFunction);

  struct GameDebugData m_gameDebugData;

public:
  LightInterface(const uint8_t ledCount = 16, const uint8_t diPin = 0);
  virtual ~LightInterface();

  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void setPixelColor(uint8_t i, uint32_t color) = 0;

  // Rotate the full sized color buffer by some offset
  void offsetBuffer(uint32_t *buffer, int8_t offset, uint8_t size = RING_LED_COUNT);

  // Replicate the smaller buffer to fit into the full buffer
  void extendBuffer(const uint32_t *smallBuffer, uint32_t *fullBufferr, uint8_t smallBufferSize, uint8_t fullBufferSize = RING_LED_COUNT);

  void copyBuffer(const uint32_t *sourceBuffer, uint32_t *targetBuffer, uint8_t size);

  /// @brief Overlays the overlay buffer on top of the base buffer
  /// Blank/Black values in the overlay buffer will not overwrite the base buffer
  /// The base buffer will be updated with the new result
  /// @param baseBuffer Buffer to modify in place with overlayed content
  /// @param overlayBuffer Buffer to overlay on the base buffer
  /// @param baseBufferSize Size of the base buffer to apply the overlay to
  /// @param overlayBufferSize Size of the buffer to overlay
  /// @param overlayBufferOffset Starting position of the base buffer to start the overlay at
  /// @param inverse If set, only overrlay blank leds for negative light designs
  void overlayBuffer(uint32_t *baseBuffer, const uint32_t *overlayBuffer, uint8_t baseBufferSize = RING_LED_COUNT, uint8_t overlayBufferSize = RING_LED_COUNT, uint8_t overlayBufferOffset = 0, bool inverse = false);

  /// @brief Set a buffer to a specific color for all non zero buffer values
  /// @param buffer Buffer to update colors of in place
  /// @param bufferSize Size of the provided buffer
  /// @param color Color to change buffer to
  void colorBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color);

  // Take a buffer of size (2, 4, 8) and populate a full 16 item buffer using the condensed buffer
  // The fill option will fill in the space between the LEDs
  void expandBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t smallBufferSize, uint8_t fullBufferSize = RING_LED_COUNT, bool fill = true);

  // Reverse the full sized color buffer
  void reverseBuffer(uint32_t *buffer, uint8_t size);

  /// @brief Set the provided buffer to a single solid color
  /// @param buffer Buffer to modify in-place
  /// @param bufferSize Size of the provided buffer
  /// @param color Color to set the buffer to
  void solidBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color);

  void printBuffer(uint32_t *buffer, int8_t size = RING_LED_COUNT);

  // Non enforced turn timer
  unsigned long m_lastColorChange;
  uint32_t m_targetColor;
  uint32_t m_previousColor;

  void transformBufferColor(uint32_t *buffer, uint8_t bufferSize, ColorTransform::ColorTransform *transform);

  /// @brief Create a fading tail gradient buffer for a specified color
  /// @param buffer Buffer to update to
  /// @param bufferSize Size of buffer/tail to make
  /// @param color Brightest color of the tail
  /// @param colorMinBrightness The brightness value of the last value in the tail
  void brightnessGradientBuffer(
      uint32_t *buffer,
      uint8_t bufferSize,
      uint32_t tailColor,
      uint32_t tailMinBrightness = 0.0);

  void setDisplayMode(DeviceState::State state);

  void updateGameDebugData(GameDebugData data);


};
