#include "light_interface.h"
#include "logger.h"
#include <cmath>
#include <algorithm>
#include <cstring>
#include "color_converter.h"

#ifdef SIMULATOR
#include <random>
#include "simulator_tools.h"
#endif

namespace
{
    const LogString loggerTag = "LightInterface";
}

// const int RING_REFRESH_RATE = 1;

const uint32_t AWAIT_GAME_COLORS[16] = {
    AWAIT_GAME_COLOR1,
    AWAIT_GAME_COLOR2,
    AWAIT_GAME_COLOR3,
    AWAIT_GAME_COLOR4,
    AWAIT_GAME_COLOR5,
    AWAIT_GAME_COLOR6,
    AWAIT_GAME_COLOR7,
    AWAIT_GAME_COLOR8,
    AWAIT_GAME_COLOR9,
    AWAIT_GAME_COLOR10,
    AWAIT_GAME_COLOR11,
    AWAIT_GAME_COLOR12,
    AWAIT_GAME_COLOR13,
    AWAIT_GAME_COLOR14,
    AWAIT_GAME_COLOR15,
    AWAIT_GAME_COLOR16};

const uint32_t AWAIT_GAME_COLORS_ALT[16] = {
    AWAIT_GAME_COLOR_ALT1,
    AWAIT_GAME_COLOR_ALT2,
    AWAIT_GAME_COLOR_ALT3,
    AWAIT_GAME_COLOR_ALT4,
    AWAIT_GAME_COLOR_ALT5,
    AWAIT_GAME_COLOR_ALT6,
    AWAIT_GAME_COLOR_ALT7,
    AWAIT_GAME_COLOR_ALT8,
    AWAIT_GAME_COLOR_ALT9,
    AWAIT_GAME_COLOR_ALT10,
    AWAIT_GAME_COLOR_ALT11,
    AWAIT_GAME_COLOR_ALT12,
    AWAIT_GAME_COLOR_ALT13,
    AWAIT_GAME_COLOR_ALT14,
    AWAIT_GAME_COLOR_ALT15,
    AWAIT_GAME_COLOR_ALT16};

/*
The number of colors used in each state:
This will match what is set in the app so that when we are in the color
editor, we display the correct number of colors for each state that can be modified
  {
    Off = 0,
    AwaitingConnection = 1,
    AwaitingGameStart = 0,
    ActiveTurnEnforced = 0,
    ActiveTurnNotEnforced = 3,
    AwaitingTurn = 3,
    Skipped = 1,
    Paused = 0,
    Debug = 0,
    BuzzerAwaitingBuzz = 4,
    BuzzerAwaitingBuzzTimed = 0,
    BuzzerAwaitingTurnEnd = 0,
    BuzzerResults = 1,
    BuzzerWinnerPeriod = 1,
    BuzzerWinnerPeriodTimed = 0,
    BuzzerAlreadyAnswered = 0,
    BuzzerAwaitingBuzzerEnabled = 0,
    BuzzerAwaitingTurnStart = 4,
    Unknown = 0,
  };
*/


LightInterface::LightInterface(const uint8_t ledCount, const uint8_t diPin)
{
  logger.info(loggerTag, "Initializing Light Interface");
  m_ledCount = ledCount;
  m_diPin = diPin;
  m_lastUpdate = millis();
  logger.info(loggerTag, "Initialized Light Interface");
}

LightInterface::~LightInterface() {}

void LightInterface::transformBufferColor(uint32_t *buffer, uint8_t bufferSize, ColorTransform::ColorTransform *transform)
{
  for (int i = 0; i < bufferSize; i++)
  {
    buffer[i] = transform->applyTransform(buffer[i]);
  }
}

void LightInterface::brightnessGradientBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color, uint32_t colorMinBrightness)
{
  for (int i = 0; i < bufferSize; i++)
  {
    uint8_t dimmingFactor = ((bufferSize - i) / (double)bufferSize * (255 - colorMinBrightness)) + colorMinBrightness;
    ColorTransform::DimColor *dimColor = new ColorTransform::DimColor(dimmingFactor);
    uint32_t transformedColor = dimColor->applyTransform(color);
    buffer[i] = transformedColor;
    delete dimColor;
  }
}

void LightInterface::setDisplayMode(DeviceState::State state)
{
  setBrightness(DEFAULT_BRIGHTNESS);
  HGDisplayInterface::setDisplayMode(state);
}

void LightInterface::updateLightModeActiveTurnNoTimer()
{
  // Uses 3 colors to create a moving inchworm effect
  // The colors will be set in the color config for the state
  // color[0] is the main color
  // color[1] is the color of the first segment
  // color[2] is the color of the second segment

  // This lighting effect will be n (divides m_ledCount) equally spaced LEDs
  // These LEDs will grow to fill until reaching the next LED
  // Then it will shrink from the starting edge
  // This will give an inchworm type effect
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  uint32_t *fullColorBuffer = new uint32_t[m_ledCount]{};

  unsigned long timeSinceModeStart = (millis() - m_startTime);

  // Get the current time segment since the beginning of the color mode change
  unsigned long adjustedTime = (int)timeSinceModeStart / (NO_TIMER_SPEED);

  // Length of an individual segment
  uint8_t segmentLength = m_ledCount / NO_TIMER_SEGMENTS;

  // The full cycle will run for 2x the number of LEDs per segment
  // Subtract two steps to skip the start and end blank screens
  uint8_t totalCycleSteps = 2 * segmentLength;

  // Total cycle duration
  unsigned long cycleDuration = totalCycleSteps * NO_TIMER_SPEED;

  // Elapsed cycle time
  unsigned long cycleElapsedTime = timeSinceModeStart % cycleDuration;

  // Current cycle
  uint8_t currentCycle = adjustedTime / totalCycleSteps;

  // Find current stage
  EasingFunction::EasingFunction *function = new EasingFunction::Cubic(EasingMode::EaseInAndOut);
  uint8_t currentSegment = getAdjustedCycleSegment(cycleDuration, m_startTime, totalCycleSteps - 1, function);
  currentSegment++;

  delete function;

  // Current state of animation
  bool growing = currentSegment < totalCycleSteps / 2;

  // Current value within the growing or shrinking cycles
  uint8_t subcycle = currentSegment % (totalCycleSteps / 2);

#if ENABLE_DEBUG
  logger.debug(loggerTag, "Adjusted Time: ", adjustedTime);
  logger.debug(loggerTag, "Segment Length: ", segmentLength);
  logger.debug(loggerTag, "Cycle length: ", totalCycleSteps);
  logger.debug(loggerTag, "Current Segment: ", currentSegment);
  logger.debug(loggerTag, "Growing: ", growing);
  logger.debug(loggerTag, "Subcycle: ", subcycle);
#endif

  if (growing)
  {
    for (int i = 0; i < subcycle; i++)
    {
      colorBuffer[i] = m_colorConfig.colors[0];
    }
    /*
    // This prevents the ring from being 100% off at the start of the sequence
    if (subcycle == 0) { 
      // logger.info("Extending first light");
      colorBuffer[0] = NO_TIMER_COLOR;
    }
    */
  }
  else
  {
    for (int i = subcycle; i < segmentLength; i++)
    {
      colorBuffer[i] =  m_colorConfig.colors[0];
    }
    /*
    // This prevents the ring from being 100% off at the end of the sequence
    if (subcycle == segmentLength) { 
      // logger.info("Extending last light");
      colorBuffer[subcycle - 1] = NO_TIMER_COLOR;
    }
    */
  }

  // Because each segment looks the same, we can just take one segment and duplicate it to fill the LED array
  extendBuffer(colorBuffer, fullColorBuffer, segmentLength);

  // Color the first segment green and the opposite segment blue
  this->colorBuffer(fullColorBuffer, segmentLength,  m_colorConfig.colors[1]);
  this->colorBuffer(fullColorBuffer + (segmentLength * NO_TIMER_SEGMENTS / 2), segmentLength,  m_colorConfig.colors[2]);
  if (NO_TIMER_APPLY_OFFSET)
  {
    // Offset is the number of full cycles that have already played
    uint8_t offset = -segmentLength * (currentCycle % NO_TIMER_SEGMENTS);
    offsetBuffer(fullColorBuffer, offset, m_ledCount);
  }

  for (int i = 0; i < m_ledCount; i++)
  {
    if (fullColorBuffer[i] == 0)
    {
      fullColorBuffer[i] = m_colorConfig.colors[3];
    }
  }

  displayBuffer(fullColorBuffer);

  delete[] colorBuffer;
  delete[] fullColorBuffer;
}

void LightInterface::updateLightModeActiveTurnTimer()
{
  displayCounterClockwiseTimer(m_timerData.elapsedTime, m_timerData.totalTime);
}

// Light mode for skipped should be a pulsing gray that changes based on brightness values
void LightInterface::updateLightModeSkipped()
{
  // This lighting mode uses one color from the color config for the state
  // color[0] is used for the pulsing color

  int deltaTime = (int)(millis() - m_startTime);

  // Percentage through pulse sequence
  double pct = (deltaTime % SKIPPED_PULSE_DURATION) / (double)SKIPPED_PULSE_DURATION;
  pct = fabs(pct - 0.5);
  uint8_t brightness = 2 * pct * (SKIPPED_MAX_BRIGHTNESS - SKIPPED_MIN_BRIGHTNESS) + SKIPPED_MIN_BRIGHTNESS;
  uint32_t *colorBuffer = new uint32_t[m_ledCount];

  setBrightness(brightness);

  for (int i = 0; i < m_ledCount; i++)
  {
    colorBuffer[i] = m_colorConfig.colors[0];
  }

  // logger.info(loggerTag, "Skipped Brightness: ", brightness, " color: ", colorBuffer[0]);

  displayBuffer(colorBuffer);
  delete[] colorBuffer;
}

void LightInterface::updateLightModeTurnSequence()
{
  // TODO: Add support for displaying skipped players in turn sequence mode
  //    Skipped players will be shown with their lights being pulsed and slightly off color
  //    Requires support in interface protocol to provide a skipped mask
  //    Skipped player lights will be calculated and then dimmed with a dynamic local brightness setting that is calculated based on time
  //    A local brightness function for color will need to be implemented to acheive localized dimming

  // This lighting mode uses 3 colors from the color config for the state
  // color[0] is the color for the current player
  // color[1] is the color for the user's player
  // color[2] is the color for all other players
  // color[3] is unused

  // logger.debug(loggerTag, "Total Players:  ", m_turnSequenceData.totalPlayers);
  // logger.debug(loggerTag, "My Player:      ", m_turnSequenceData.myPlayerIndex);
  // logger.debug(loggerTag, "Current Player: ", m_turnSequenceData.currentPlayerIndex);
  // logger.debug(loggerTag, "");
  uint32_t *colorBuffer = new uint32_t[m_ledCount];
  uint32_t *modifiedColorBuffer = new uint32_t[m_ledCount];

  uint32_t myPlayerColor = m_colorConfig.colors[0];
  uint32_t currentPlayerColor = m_colorConfig.colors[1];
  uint32_t otherPlayerColor = m_colorConfig.colors[2];

  int deltaTime = (int)(millis() - m_startTime);
  double pct = (deltaTime % SKIPPED_PULSE_DURATION) / (double)SKIPPED_PULSE_DURATION;

  // Convert total percentage into percentage from midpoint
  pct = fabs(pct - 0.5) * 2;

  EasingFunction::EasingFunction *easingFunction = new EasingFunction::Sine(EasingMode::EaseIn);

  // Fix the percentage to some range between 0 and 100. i.e 30 - 100
  pct = pct * 0.5 + 0.4;

  // Ease that percentage using the provided curve
  pct = easingFunction->ease(pct);

  delete easingFunction;

  // Get that percentage as a absolute value from the 50% mark

  // Covert the new percentage to a brightness value
  uint8_t brightness = pct * 255;

  ColorTransform::ColorTransform *transform = new ColorTransform::DimColor(brightness);
  uint32_t skippedOtherPlayerColor = transform->applyTransform(otherPlayerColor);
  delete transform;

  for (int i = 0; i < m_ledCount; i++)
  {
    if (i < m_turnSequenceData.totalPlayers)
    {
      if (m_turnSequenceData.currentPlayerIndex == i)
      {
        colorBuffer[i] = currentPlayerColor;
      }
      else if (m_turnSequenceData.myPlayerIndex == i)
      {
        colorBuffer[i] = myPlayerColor;
      }
      else
      {
        // Check if player is skipped
        if ((m_turnSequenceData.skippedPlayers >> i) & 1)
        {
          colorBuffer[i] = skippedOtherPlayerColor;
        }
        else
        {
          colorBuffer[i] = otherPlayerColor;
        }
      }
    }
    else
    {
      colorBuffer[i] = BLACK;
    }
  }

  if ((m_ledCount % m_turnSequenceData.totalPlayers == 0 && UNIFORM_SEQUENCES_REQUIRED && EXPAND_TURN_SEQUENCE_BUFFER) || (EXPAND_TURN_SEQUENCE_BUFFER && !UNIFORM_SEQUENCES_REQUIRED))
  {
    if (!m_absoluteOrientation)
    {
      // If the orientation should be absolute, offset the buffer to put the device's player in the first index
      offsetBuffer(colorBuffer, -m_turnSequenceData.myPlayerIndex, m_turnSequenceData.totalPlayers);
    }
    expandBuffer(colorBuffer, modifiedColorBuffer, m_turnSequenceData.totalPlayers, m_ledCount);
    displayBuffer(modifiedColorBuffer);
  }
  else
  {
    displayBuffer(colorBuffer);
  }

  delete[] colorBuffer;
  delete[] modifiedColorBuffer;
}

void LightInterface::updateLightModeAwaitGameStart()
{
  uint32_t *colorBuffer = new uint32_t[m_ledCount];
  const uint32_t *colors = (m_colorBlindMode) ? AWAIT_GAME_COLORS_ALT : AWAIT_GAME_COLORS;
  int segments = std::max(m_gameStartData.totalPlayers, 1);

  expandBuffer(colors, colorBuffer, segments, m_ledCount);

  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_GAME_START_SPEED;
  uint8_t offset = currentSecond % m_ledCount;
  offsetBuffer(colorBuffer, offset, m_ledCount);
  displayBuffer(colorBuffer);
  delete[] colorBuffer;
}

void LightInterface::updateLightModeAwaitConnection()
{
  // This lighting mode uses one color from the color config for the state
  // color[0] is used for the cycling color
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};

  // unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_CONNECTION_SPEED;
  uint32_t color = m_colorConfig.colors[0];

  unsigned long timeSinceModeStart = (millis() - m_startTime);

  // Length of an individual segment
  uint8_t segmentLength = m_ledCount;

  // The full cycle will run for 2x the number of LEDs per segment
  uint8_t totalCycleSteps = 2 * segmentLength;

  // Total cycle duration
  unsigned long cycleDuration = totalCycleSteps * AWAIT_CONNECTION_SPEED;

  // Elapsed cycle time
  unsigned long cycleElapsedTime = timeSinceModeStart % cycleDuration;

  // Cycle completion percent
  double cycleCompletionPercent = (double)cycleElapsedTime / (double)cycleDuration;

  // Waxing / Waning
  bool waxing = cycleCompletionPercent < 0.5;

  // Find current stage
  EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseInAndOut);
  uint8_t currentSingleCycleStep = getAdjustedCycleSegment(cycleDuration / 2.0, m_startTime, totalCycleSteps / 2.0, function);
  delete function;

  for (int i = 0; i < m_ledCount; i++)
  {
    if (waxing)
    {
      if (i <= currentSingleCycleStep % m_ledCount)
      {
        colorBuffer[i] = color;
      }
    }
    else
    {
      if (i > currentSingleCycleStep % m_ledCount)
      {
        colorBuffer[i] = color;
      }
    }
  }

  displayBuffer(colorBuffer);
  delete[] colorBuffer;
}

// Update the paused game screen
void LightInterface::updateGamePaused()
{
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  uint32_t *blankBuffer = new uint32_t[m_ledCount]{};

  unsigned long currentTime = millis();
  if (currentTime - m_lastColorChange > PAUSED_NEW_COLOR_PERIOD)
  {
    m_lastColorChange = currentTime;
    m_previousColor = m_targetColor;

#ifdef SIMULATOR
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 0xFF);

    uint32_t r = dis(gen) << 16;
    uint32_t g = dis(gen) << 8;
    uint32_t b = dis(gen);
#else
    uint32_t r = random(0xFF) << 16;
    uint32_t g = random(0xFF) << 8;
    uint32_t b = random(0xFF);
#endif
    m_targetColor = r | g | b;
  }

  double pct = (double)(currentTime - m_lastColorChange) / PAUSED_NEW_COLOR_PERIOD;
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));

  EasingFunction::EasingFunction *easingFunction = new EasingFunction::Circular(EasingMode::EaseInAndOut);
  uint32_t color = interpolateColors(m_previousColor, m_targetColor, pct, easingFunction);
  delete easingFunction;

  unsigned long currentSecond = (int)(millis() - m_startTime) / PAUSED_BLANK_SPEED;
  uint8_t offset = currentSecond % m_ledCount;

  for (int i = 0; i < m_ledCount; i++)
  {
    if (!(i == offset || i == m_ledCount - 1 - offset))
    {
      blankBuffer[i] = WHITE;
    }
  }

  /*
  logger.info(loggerTag, "PCT: ", pct);
  logger.info(loggerTag, "Prev: ", m_previousColor);
  logger.info(loggerTag, "Target: ", m_targetColor);
  logger.info(loggerTag, "Color: ", color);
  logger.info(loggerTag, "");
  */

  solidBuffer(colorBuffer, m_ledCount, color);
  overlayBuffer(colorBuffer, blankBuffer, m_ledCount, m_ledCount, 0, true);

  displayBuffer(colorBuffer);

  delete[] blankBuffer;
  delete[] colorBuffer;
}

void LightInterface::updateGameDebug()
{
  uint32_t *buffer = new uint32_t[m_ledCount]{};
  // copyBuffer(m_gameDebugData.buffer, buffer, m_ledCount);

  for (int i = 0; i < m_ledCount; i++)
  {
    if (i % 2 == 0)
      buffer[i] = m_accentColor;
    else
      buffer[i] = m_primaryColor;
  }

  displayBuffer(buffer);

  delete[] buffer;
}

void LightInterface::updateLightModeBuzzerAwaitingBuzz()
{
  // 4 colors are used for this lighting mode
  // color[0] is the first color in the cycle
  // color[1] is the second color in the cycle
  // color[2] is the third color in the cycle
  // color[3] is the fourth color in the cycle

  // TODO: Remove before merge
  updateLightModeActiveTurnNoTimer();
  return;

  unsigned long start = millis();

  uint8_t halfBufferSize = m_ledCount / 2;

  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  uint32_t *tailBuffer1 = new uint32_t[halfBufferSize]{};
  uint32_t *tailBuffer2 = new uint32_t[halfBufferSize]{};

  uint8_t numColors = 4;
  uint32_t timeSinceModeStart = millis() - m_startTime;
  uint32_t colorCycleDuration = BUZZER_AWAITING_TURN_CYCLE_DURATION / numColors;

  uint32_t currentCycleTime = timeSinceModeStart % colorCycleDuration;
  uint32_t totalCycleTime = timeSinceModeStart % BUZZER_AWAITING_TURN_CYCLE_DURATION;

  uint8_t currentColorCycle = totalCycleTime / colorCycleDuration;

  uint8_t offset = (currentCycleTime / (double)colorCycleDuration) * m_ledCount;

  uint32_t color;

  // logger.info(loggerTag, int(offset));

  switch (currentColorCycle)
  {
  case 0:
    color = m_colorConfig.colors[0];
    break;
  case 1:
    color = m_colorConfig.colors[1];
    break;
  case 2:
    color = m_colorConfig.colors[2];
    break;
  case 3:
    color = m_colorConfig.colors[3];
    break;
  default:
    break;
  }

  brightnessGradientBuffer(tailBuffer1, halfBufferSize / 2, color);
  reverseBuffer(tailBuffer1, halfBufferSize);

  copyBuffer(tailBuffer1, tailBuffer2, halfBufferSize);

  ColorTransform::ColorTransform *transform = new ColorTransform::ShiftLeft();
  transformBufferColor(tailBuffer2, halfBufferSize, transform);
  overlayBuffer(colorBuffer, tailBuffer1, m_ledCount, halfBufferSize);
  overlayBuffer(colorBuffer, tailBuffer2, m_ledCount, halfBufferSize, halfBufferSize);
  offsetBuffer(colorBuffer, offset, m_ledCount);
  displayBuffer(colorBuffer);

  delete transform;
  delete[] colorBuffer;
  delete[] tailBuffer1;
  delete[] tailBuffer2;
}

void LightInterface::updateLightModeBuzzerAwaitingBuzzTimed()
{
  displayCounterClockwiseTimer(m_timerData.elapsedTime, m_timerData.totalTime);
}

void LightInterface::updateLightModeWinnerPeriod()
{
  // Uses a marquee effect with the winner color
  // color[0] is the winner color
  displayDualMarqueeBuzzer(m_colorConfig.colors[0], m_colorConfig.colors[1]);
}

void LightInterface::updateLightModeWinnerPeriodTimed()
{
  displaySymmetricFixedColorTimer(m_timerData.elapsedTime, m_timerData.totalTime);
}

void LightInterface::updateLightModeBuzzerResults()
{
  // Uses a marquee effect with the loser color
  // color[0] is the loser color
  displayDualMarqueeBuzzer(m_colorConfig.colors[0], m_colorConfig.colors[1]);
}

void LightInterface::updateLightModeAwaitTurnStart()
{
  // This lighting mode uses 4 colors from the color config for the state
  // color[0] is the first color in the cycle
  // color[1] is the second color in the cycle
  // color[2] is the third color in the cycle
  // color[3] is the fourth color in the cycle

  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};

  unsigned long timeSinceModeStart = (millis() - m_startTime);

  // Get length of the cycle
  uint8_t cycleLength = m_ledCount;

  // Get full cycle duration
  unsigned long totalCycleDuration = cycleLength * BUZZER_AWAITING_TURN_START_SPEED * 2;

  // Current cycle elapsed time
  unsigned long cycleElapsedTime = timeSinceModeStart % totalCycleDuration;

  // Cycle percentage completion
  double pct = (double)cycleElapsedTime / (double)totalCycleDuration;

  double doublePct = pct * 2.0;
  doublePct = doublePct - (int)doublePct;

  double triplePct = pct * 3.0;
  triplePct = triplePct - (int)triplePct;

  double quadPct = pct * 4.0;
  quadPct = quadPct - (int)quadPct;

  EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseInAndOut);
  pct = function->ease(pct);
  doublePct = function->ease(doublePct);
  triplePct = function->ease(triplePct);
  quadPct = function->ease(quadPct);
  delete function;

  uint8_t index1 = min((int)(m_ledCount * pct), m_ledCount - 1);
  uint8_t index2 = min((int)(m_ledCount * doublePct), m_ledCount - 1);
  uint8_t index3 = min((int)(m_ledCount * triplePct), m_ledCount - 1);
  uint8_t index4 = min((int)(m_ledCount * quadPct), m_ledCount - 1);

  colorBuffer[index1] = m_colorConfig.colors[0];
  colorBuffer[index2] = m_colorConfig.colors[1];
  colorBuffer[index3] = m_colorConfig.colors[2];
  colorBuffer[index4] = m_colorConfig.colors[3];

  displayBuffer(colorBuffer, false);
  delete[] colorBuffer;
}

void LightInterface::updateDeviceColorMode()
{
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  uint32_t primaryColor = m_colorConfig.colors[0];
  uint32_t accentColor = m_colorConfig.colors[1];
  uint32_t *smallBuffer = new uint32_t[2]{primaryColor, accentColor};

  expandBuffer(smallBuffer, colorBuffer, 2, m_ledCount);
  displayBuffer(colorBuffer);
  delete[] colorBuffer;
  delete[] smallBuffer;
}

void LightInterface::updateLightModeDeviceLEDOffsetMode()
{
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  colorBuffer[0] = BLUE;
  colorBuffer[m_ledCount - 1] = RED;

  displayBuffer(colorBuffer, false);
  delete[] colorBuffer;
}

void LightInterface::displayMarqueeBuzzer(uint32_t color)
{
  ColorTransform::ColorTransform *transform = new ColorTransform::DimColor(150);
  uint32_t dimColor = transform->applyTransform(color);
  delete transform;
  displayDualMarqueeBuzzer(color, dimColor);
}

void LightInterface::displayDualMarqueeBuzzer(uint32_t color1, uint32_t color2)
{
  unsigned long timeSinceModeStart = millis() - m_startTime;
  uint32_t doubleCycleTime = timeSinceModeStart % BUZZER_ACTIVE_TURN_MARQUEE_DURATION;
  uint8_t offset = 0;

  if (doubleCycleTime > BUZZER_ACTIVE_TURN_MARQUEE_DURATION / 2)
  {
    offset = 1;
  }

  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};

  for (int i = 0; i < m_ledCount; i++)
  {
    colorBuffer[i] = i % 2 ? color1 : color2;
  }

  offsetBuffer(colorBuffer, offset, m_ledCount);
  displayBuffer(colorBuffer);
  delete[] colorBuffer;
}

void LightInterface::displayCounterClockwiseTimer(uint32_t elapsedTime, uint32_t totalTimerDuration)
{
  uint32_t *colorBuffer = new uint32_t[m_ledCount];
  uint32_t color1 = (m_colorBlindMode) ? TIMER_COLOR_ALT_1 : TIMER_COLOR_1;
  uint32_t color2 = (m_colorBlindMode) ? TIMER_COLOR_ALT_2 : TIMER_COLOR_2;
  uint32_t color3 = (m_colorBlindMode) ? TIMER_COLOR_ALT_3 : TIMER_COLOR_3;
  uint32_t color;

  double pct = (double)elapsedTime / totalTimerDuration;
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));
  int filled = std::min((int)(pct * m_ledCount) + 1, (int)m_ledCount);
  int deltaTime = ((int)(millis() - m_startTime)) / 200 - 60;

  solidBuffer(colorBuffer, m_ledCount, BLACK);

  if (pct < 0.5)
  {
    color = color1;
  }
  else if (pct < 0.75)
  {
    color = color2;
  }
  else if (pct <= 1.0)
  {
    color = color3;
  }
  else
  {
    if (deltaTime % 2 == 0)
    {
      color = color3;
    }
  }

  for (int i = 0; i < filled; i++)
  {
    colorBuffer[i] = color;
  }

  displayBuffer(colorBuffer, false);
  delete[] colorBuffer;
}

void LightInterface::displaySymmetricFixedColorTimer(uint32_t elapsedTime, uint32_t totalTimerDuration)
{

  uint8_t halfBufferSize = m_ledCount / 2;

  double pct = (double)elapsedTime / totalTimerDuration;
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));
  uint8_t filled = std::min((int)((pct * halfBufferSize) + 1), (int)halfBufferSize);

  // Create a buffer
  uint32_t *colorBuffer = new uint32_t[m_ledCount]{};
  uint32_t *basicColorBuffer;
  if (m_colorBlindMode)
  {
    basicColorBuffer = new uint32_t[3]{
        BUZZER_AWAITING_BUZZ_COLOR_ALT_1, BUZZER_AWAITING_BUZZ_COLOR_ALT_2, BUZZER_AWAITING_BUZZ_COLOR_ALT_3};
  }
  else
  {
    basicColorBuffer = new uint32_t[3]{
        BUZZER_AWAITING_BUZZ_COLOR_1, BUZZER_AWAITING_BUZZ_COLOR_2, BUZZER_AWAITING_BUZZ_COLOR_3};
  }

  uint32_t *halfBuffer = new uint32_t[halfBufferSize];
  expandBuffer(basicColorBuffer, halfBuffer, 3, halfBufferSize);
  uint32_t *halfBuffer2 = new uint32_t[halfBufferSize];

  uint32_t *blankBuffer = new uint32_t[halfBufferSize];

  for (int i = 0; i < halfBufferSize; i++)
  {
    blankBuffer[i] = i < filled ? 1 : 0;
  }

  overlayBuffer(halfBuffer, blankBuffer, halfBufferSize, halfBufferSize, 0, true);
  copyBuffer(halfBuffer, halfBuffer2, halfBufferSize);
  reverseBuffer(halfBuffer2, halfBufferSize);
  overlayBuffer(colorBuffer, halfBuffer, m_ledCount, halfBufferSize);
  overlayBuffer(colorBuffer, halfBuffer2, m_ledCount, halfBufferSize, halfBufferSize);

  displayBuffer(colorBuffer);
  delete[] colorBuffer;
  delete[] halfBuffer;
  delete[] halfBuffer2;
  delete[] basicColorBuffer;
  delete[] blankBuffer;
}

void LightInterface::updateGameDebugData(GameDebugData data)
{
  m_gameDebugData = data;
}

void LightInterface::extendBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t smallBufferSize, uint8_t fullBufferSize)
{
  if (fullBufferSize % smallBufferSize)
  {
    logger.error(loggerTag, "LightInterface::extendBuffer: Smaller buffer size must divide evenly into Full Buffer Size");
    return;
  }

  uint8_t repeat = fullBufferSize / smallBufferSize;

  uint8_t currentIndex = 0;
  for (int i = 0; i < repeat; i++)
  {
    for (int j = 0; j < smallBufferSize; j++)
    {
      fullBuffer[currentIndex] = smallBuffer[j];
      currentIndex += 1;
    }
  }
}

void LightInterface::copyBuffer(const uint32_t *sourceBuffer, uint32_t *targetBuffer, uint8_t size)
{
  for (int i = 0; i < size; i++)
  {
    targetBuffer[i] = sourceBuffer[i];
  }
}

void LightInterface::expandBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t smallBufferSize, uint8_t fullBufferSize, bool fill)
{
  int lengthSegment = fullBufferSize / smallBufferSize;
  int remainder = fullBufferSize % smallBufferSize;

  int currentIndex = 0;
  for (int currentSegment = 0; currentSegment < smallBufferSize; currentSegment++)
  {
    int adjustedSegmentLength = lengthSegment;
    if (currentSegment < remainder)
    {
      adjustedSegmentLength += 1;
    }

    for (int subIndex = 0; subIndex < adjustedSegmentLength; subIndex++)
    {
      if (subIndex == 0 || fill)
      {
        fullBuffer[currentIndex] = smallBuffer[currentSegment];
      }
      currentIndex += 1;
    }
  }
}

void LightInterface::offsetBuffer(uint32_t *buffer, int8_t offset, uint8_t size)
{
  // Correct negative offsets
  while (offset < 0)
  {
    offset += size;
  }
  uint32_t *originalBuffer = new uint32_t[size];
  memcpy(originalBuffer, buffer, sizeof(uint32_t) * size);
  for (int i = 0; i < size; i++)
  {
    uint8_t newIndex = (i + offset) % size;
    buffer[newIndex] = originalBuffer[i];
  }
  delete originalBuffer;
}

void LightInterface::reverseBuffer(uint32_t *buffer, uint8_t size)
{
  uint32_t *originalBuffer = new uint32_t[size];
  memcpy(originalBuffer, buffer, sizeof(uint32_t) * size);
  for (int i = 0; i < size; i++)
  {
    uint8_t newIndex = size - i - 1;
    buffer[i] = originalBuffer[newIndex];
  }
  delete originalBuffer;
}

void LightInterface::printBuffer(uint32_t *buffer, int8_t size)
{
  char bufferString[256];
  for (int i = 0; i < size; i++)
  {
    sprintf(bufferString, "printBuffer: Buffer[%d/%d]: %d", i, size, buffer[i]);
    logger.info(loggerTag, bufferString);
  }
}

void LightInterface::displayBuffer(const uint32_t *buffer, const bool clockwise)
{
  for (int i = 0; i < m_ledCount; i++)
  {
    if (clockwise)
    {
      setPixelColor(((m_ledCount - i - 1) + m_ledOffset) % m_ledCount, buffer[i]);
    }
    else
    {
      setPixelColor((i + m_ledOffset) % m_ledCount, buffer[i]);
    }
  }
}

void LightInterface::solidBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color)
{
  for (int i = 0; i < bufferSize; i++)
  {
    buffer[i] = color;
  }
}

void LightInterface::overlayBuffer(uint32_t *baseBuffer, const uint32_t *overlayBuffer, uint8_t baseBufferSize, uint8_t overlayBufferSize, uint8_t overlayBufferOffset, bool inverse)
{
  // Correct negative offsets
  while (overlayBufferOffset < 0)
  {
    overlayBufferOffset += overlayBufferSize;
  }
  for (int i = 0; i < overlayBufferSize; i++)
  {
    if (overlayBuffer[i] && !inverse || !overlayBuffer[i] && inverse)
    {
      baseBuffer[(i + overlayBufferOffset) % baseBufferSize] = overlayBuffer[i];
    }
  }
}

void LightInterface::colorBuffer(uint32_t *buffer, uint8_t bufferSize, uint32_t color)
{
  for (int i = 0; i < bufferSize; i++)
  {
    if (buffer[i] > 0)
    {
      buffer[i] = color;
    }
  }
}

uint8_t LightInterface::getAdjustedCycleSegment(unsigned long cycleDuration, unsigned long cycleStartTime, uint8_t totalCycleSteps, EasingFunction::EasingFunction *easingFunction)
{
  // Adjusted current cycle
  unsigned long cycleElapsedTime = (millis() - cycleStartTime) % cycleDuration;
  double pctOfCycle = (double)cycleElapsedTime / cycleDuration;
  double newPctOfCycle = easingFunction->ease(pctOfCycle);
  return (int)(newPctOfCycle * totalCycleSteps);
}

uint32_t LightInterface::interpolateColors(uint32_t color1, uint32_t color2, double pct)
{
  uint32_t colorR1 = color1 & 0xFF0000;
  uint32_t colorG1 = color1 & 0x00FF00;
  uint32_t colorB1 = color1 & 0x0000FF;
  uint32_t colorR2 = color2 & 0xFF0000;
  uint32_t colorG2 = color2 & 0x00FF00;
  uint32_t colorB2 = color2 & 0x0000FF;
  return interpolateColors(colorR1, colorG1, colorB1, colorR2, colorG2, colorB2, pct);
}

uint32_t LightInterface::interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct)
{
  EasingFunction::EasingFunction *easingFunction = new EasingFunction::Linear(EasingMode::EaseIn);
  uint32_t color = interpolateColors(colorR1, colorG1, colorB1, colorR2, colorG2, colorB2, pct, easingFunction);
  delete easingFunction;
  return color;
}

uint32_t LightInterface::interpolateColors(uint32_t color1, uint32_t color2, double pct, EasingFunction::EasingFunction *easingFunction)
{
  uint32_t colorR1 = color1 & 0xFF0000;
  uint32_t colorG1 = color1 & 0x00FF00;
  uint32_t colorB1 = color1 & 0x0000FF;
  uint32_t colorR2 = color2 & 0xFF0000;
  uint32_t colorG2 = color2 & 0x00FF00;
  uint32_t colorB2 = color2 & 0x0000FF;
  return interpolateColors(colorR1, colorG1, colorB1, colorR2, colorG2, colorB2, pct);
}

uint32_t LightInterface::interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct, EasingFunction::EasingFunction *easingFunction)
{
  double adjustedPct = easingFunction->ease(pct);
  uint32_t colorR = ((int)colorR2 - (int)colorR1) * adjustedPct + colorR1;
  uint32_t colorG = ((int)colorG2 - (int)colorG1) * adjustedPct + colorG1;
  uint32_t colorB = ((int)colorB2 - (int)colorB1) * adjustedPct + colorB1;
  return colorR & 0xFF0000 | colorG & 0x00FF00 | colorB & 0x0000FF;
}
