#include "light_interface.h"
#include "logger.h"

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

LightInterface::LightInterface(const uint8_t ledCount, const uint8_t diPin)
{
  logger.info("Initializing Light Interface");
  m_ledCount = ledCount;
  m_diPin = diPin;
  m_lastUpdate = millis();
  logger.info("Initialized Light Interface");
}

LightInterface::~LightInterface() {}


void LightInterface::setDisplayMode(DeviceState state)
{
  setBrightness(DEFAULT_BRIGHTNESS);
  HGDisplayInterface::setDisplayMode(state);
}

/*
  // 2 equally spaced lights
  for (int i = 0; i < m_ledCount; i++) {
    if (i == currentSecond % m_ledCount) {
      m_ring.setPixelColor(i, m_ring.Color(0, 0, 255));
      m_ring.setPixelColor(((int)i + m_ledCount / 2) % m_ledCount, m_ring.Color(0, 255, 0));
    }
  }
  m_ring.show();
  */

/* Alternate back and forth
  for (int i = 0; i < m_ledCount; i++) {
    if ((currentSecond % 2 == 0 && i % 2 == 0) || (currentSecond % 2 == 1 && i % 2 == 1)) {
      m_ring.setPixelColor(i, m_ring.Color(0, 0, 255));
    } else {
      m_ring.setPixelColor(i, m_ring.Color(0, 255, 255));
    }
  }
  m_ring.show();
  */

void LightInterface::updateLightModeActiveTurn()
{
  if (m_timerData.isTurnTimeEnforced)
  {
    updateLightModeActiveTurnTimer();
  }
  else
  {
    updateLightModeActiveTurnNoTimer();
  }
}

void LightInterface::updateLightModeActiveTurnNoTimer()
{
  // This lighting effect will be n (divides m_ledCount) equally spaced LEDs
  // These LEDs will grow to fill until reaching the next LED
  // Then it will shrink from the starting edge
  // This will give an inchworm type effect
  uint32_t colorBuffer[16] = {};

  // Get the current time segment since the beginning of the color mode change
  unsigned long adjustedTime = (int)(millis() - m_startTime) / (NO_TIMER_SPEED);

  // Length of an individual segment
  uint8_t segmentLength = m_ledCount / NO_TIMER_SEGMENTS;

  // The full cycle will run for 2x the number of LEDs per segment
  uint8_t cycleLength = 2 * segmentLength;

  // Find current stage
  uint8_t currentSegment = adjustedTime % cycleLength;

  // Current state of animation
  bool growing = currentSegment < cycleLength / 2;

  // Current value within the growing or shrinking cycles
  uint8_t subcycle = currentSegment % (cycleLength / 2);

  /*
  logger.debug("Adjusted Time: " + String(adjustedTime));
  logger.debug("Segment Length: " + String(segmentLength));
  logger.debug("Cycle length: " + String(cycleLength));
  logger.debug("Current Segment: " + String(currentSegment));
  logger.debug("Growing: " + String(growing));
  logger.debug("Subcycle: " + String(subcycle));
  */

  if (growing)
  {
    for (int i = 0; i < subcycle; i++)
    {
      colorBuffer[i] = NO_TIMER_COLOR;
    }
  }
  else
  {
    for (int i = subcycle; i < segmentLength; i++)
    {
      colorBuffer[i] = NO_TIMER_COLOR;
    }
  }

  uint32_t fullColorBuffer[16] = {};

  // Because each segment looks the same, we can just take one segment and duplicate it to fill the LED array
  extendBuffer(colorBuffer, fullColorBuffer, segmentLength);
  if (NO_TIMER_APPLY_OFFSET)
  {
    // Offset is the number of full cycles that have already played
    uint8_t offset = adjustedTime / cycleLength % m_ledCount;
    offsetBuffer(fullColorBuffer, offset);
  }
  displayBuffer(fullColorBuffer);
}

void LightInterface::updateLightModeActiveTurnTimer()
{
  uint32_t colorBuffer[16];
  uint32_t color1 = (m_colorBlindMode) ? TIMER_COLOR_ALT_1 : TIMER_COLOR_1;
  uint32_t color2 = (m_colorBlindMode) ? TIMER_COLOR_ALT_2 : TIMER_COLOR_2;
  uint32_t color3 = (m_colorBlindMode) ? TIMER_COLOR_ALT_3 : TIMER_COLOR_3;
  uint32_t color;

  double pct = (double)m_timerData.elapsedTime / m_timerData.totalTime;
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));
  int filled = (int)(pct * m_ledCount);
  int deltaTime = ((int)(millis() - m_startTime)) / 200 - 60;

  solidBuffer(colorBuffer, BLACK);

  if (pct < 0.75)
  {
    color = color1;
  }
  else if (pct < 0.9)
  {
    color = color2;
  }
  else if (pct < 1.0)
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
  displayBuffer(colorBuffer);
}

// Light mode for skipped should be a pulsing gray that changes based on brightness values
void LightInterface::updateLightModeSkipped()
{
  int deltaTime = (int)(millis() - m_startTime);

  // Percentage through pulse sequence
  double pct = (deltaTime % SKIPPED_PULSE_DURATION) / (double)SKIPPED_PULSE_DURATION;
  pct = abs(pct - 0.5);
  uint8_t brightness = 2 * pct * (SKIPPED_MAX_BRIGHTNESS - SKIPPED_MIN_BRIGHTNESS) + SKIPPED_MIN_BRIGHTNESS;
  uint32_t colorBuffer[16];

  setBrightness(brightness);

  for (int i = 0; i < m_ledCount; i++)
  {
    colorBuffer[i] = SKIPPED_COLOR;
  }

  displayBuffer(colorBuffer);
}

void LightInterface::updateLightModeTurnSequence()
{
  // TODO: Add support for displaying skipped players in turn sequence mode
  //    Skipped players will be shown with their lights being pulsed and slightly off color
  //    Requires support in interface protocol to provide a skipped mask
  //    Skipped player lights will be calculated and then dimmed with a dynamic local brightness setting that is calculated based on time
  //    A local brightness function for color will need to be implemented to acheive localized dimming

  // logger.debug("Total Players:  " + String(m_turnSequenceData.totalPlayers));
  // logger.debug("My Player:      " + String(m_turnSequenceData.myPlayerIndex));
  // logger.debug("Current Player: " + String(m_turnSequenceData.currentPlayerIndex));
  // logger.debug("");
  uint32_t colorBuffer[16];
  uint32_t myPlayerColor = (m_colorBlindMode) ? MY_PLAYER_COLOR_ALT : MY_PLAYER_COLOR;
  uint32_t currentPlayerColor = (m_colorBlindMode) ? CURRENT_PLAYER_COLOR_ALT : CURRENT_PLAYER_COLOR;
  uint32_t otherPlayerColor = (m_colorBlindMode) ? OTHER_PLAYER_COLOR_ALT : OTHER_PLAYER_COLOR;

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
        colorBuffer[i] = otherPlayerColor;
      }
    }
    else
    {
      colorBuffer[i] = BLACK;
    }

    uint32_t modifiedColorBuffer[16];

    if ((m_turnSequenceData.totalPlayers == 2 || m_turnSequenceData.totalPlayers == 4 || m_turnSequenceData.totalPlayers == 8) && EXPAND_TURN_SEQUENCE_BUFFER)
    {
      expandBuffer(colorBuffer, modifiedColorBuffer, m_turnSequenceData.totalPlayers);
      displayBuffer(modifiedColorBuffer);
    }
    else
    {
      displayBuffer(colorBuffer);
    }
  }
}

void LightInterface::updateLightModeAwaitGameStart()
{
  uint32_t colorBuffer[16];
  const uint32_t *colors = (m_colorBlindMode) ? AWAIT_GAME_COLORS_ALT : AWAIT_GAME_COLORS;

  int segments = max(m_gameStartData.totalPlayers, 1);

  expandBuffer(colors, colorBuffer, segments);

  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_GAME_START_SPEED;
  uint8_t offset = currentSecond % m_ledCount;
  offsetBuffer(colorBuffer, offset);
  displayBuffer(colorBuffer);
}

void LightInterface::updateLightModeAwaitConnection()
{
  uint32_t colorBuffer[16] = {};

  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_CONNECTION_SPEED;
  uint32_t color = (m_colorBlindMode) ? AWAIT_CONNECTION_COLOR_ALT : AWAIT_CONNECTION_COLOR;

  int timeSlice = currentSecond % (m_ledCount * 2);
  int waxing = timeSlice / m_ledCount == 0;
  for (int i = 0; i < m_ledCount; i++)
  {
    if (waxing)
    {
      if (i <= timeSlice % m_ledCount)
      {
        colorBuffer[i] = color;
      }
    }
    else
    {
      if (i > timeSlice % m_ledCount)
      {
        colorBuffer[i] = color;
      }
    }
  }
  displayBuffer(colorBuffer);
}

// Update the paused game screen
void LightInterface::updateGamePaused()
{
  unsigned long currentTime = millis();
  if (currentTime - m_lastColorChange > PAUSED_NEW_COLOR_PERIOD) {
    m_lastColorChange = currentTime;
    m_previousColor = m_targetColor;
    uint32_t r = random(0xFF) << 16;
    uint32_t g = random(0xFF) << 8;
    uint32_t b = random(0xFF);
    m_targetColor = r | g | b;
  }

  uint32_t colorBuffer[16];

  double pct = (double) (currentTime - m_lastColorChange) / PAUSED_NEW_COLOR_PERIOD;
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));


  uint32_t color = interpolateColors(m_previousColor, m_targetColor, pct);

  unsigned long currentSecond = (int)(millis() - m_startTime) / PAUSED_BLANK_SPEED;
  uint8_t offset = currentSecond % m_ledCount;
  uint32_t blankBuffer[16] = {};

  for (int i = 0; i < m_ledCount; i++) {
    if (!(i == offset || i == m_ledCount - 1 - offset)) {
      blankBuffer[i] = WHITE;
    }
  }

  /*
  logger.info("PCT: " + String(pct));
  logger.info("Prev: " + String(m_previousColor));
  logger.info("Target: " + String(m_targetColor));
  logger.info("Color: " + String(color));
  logger.info("");
  */

  solidBuffer(colorBuffer, color);
  overlayBuffer(colorBuffer, blankBuffer, true);

  displayBuffer(colorBuffer);
}



void LightInterface::extendBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size)
{
  if (m_ledCount % size)
  {
    logger.error("LightInterface::extendBuffer: Smaller buffer size must divide evenly into LED count");
    return;
  }

  uint8_t repeat = m_ledCount / size;

  uint8_t currentIndex = 0;
  for (int i = 0; i < repeat; i++)
  {
    for (int j = 0; j < size; j++)
    {
      fullBuffer[currentIndex] = smallBuffer[j];
      currentIndex += 1;
    }
  }
}

void LightInterface::expandBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size, bool fill)
{
  int lengthSegment = m_ledCount / size;
  int remainder = m_ledCount % size;

  int currentIndex = 0;
  for (int currentSegment = 0; currentSegment < size; currentSegment++)
  {
    fullBuffer[currentIndex] = smallBuffer[currentSegment];
    for (int j = 0; j < lengthSegment; j++)
    {
      if (fill)
      {
        fullBuffer[currentIndex] = smallBuffer[currentSegment];
      }
      currentIndex += 1;
    }
    if (currentSegment < remainder && fill)
    {
      fullBuffer[currentIndex] = smallBuffer[currentSegment];
      currentIndex += 1;
    }
  }
}

void LightInterface::offsetBuffer(uint32_t *buffer, uint8_t offset)
{
  uint32_t *originalBuffer = new uint32_t[m_ledCount];
  memcpy(originalBuffer, buffer, sizeof(uint32_t) * m_ledCount);
  for (int i = 0; i < m_ledCount; i++)
  {
    uint8_t newIndex = (i + offset) % m_ledCount;
    buffer[i] = originalBuffer[newIndex];
  }
  delete originalBuffer;
}

void LightInterface::reverseBuffer(uint32_t *buffer, uint8_t offset)
{
  uint32_t *originalBuffer = new uint32_t[m_ledCount];
  memcpy(originalBuffer, buffer, sizeof(uint32_t) * m_ledCount);
  for (int i = 0; i < m_ledCount; i++)
  {
    uint8_t newIndex = m_ledCount - i - 1;
    buffer[i] = originalBuffer[newIndex];
  }
  delete originalBuffer;
}

void LightInterface::overlayBuffer(uint32_t *baseBuffer, const uint32_t *overlayBuffer, bool inverse) {
  for (int i = 0; i < m_ledCount; i++) {
    if (overlayBuffer[i] && !inverse || !overlayBuffer[i] && inverse) {
      baseBuffer[i] = overlayBuffer[i];
    }
  }
}

void LightInterface::displayBuffer(const uint32_t *buffer)
{
  for (int i = 0; i < m_ledCount; i++)
  {
    setPixelColor(i, buffer[i]);
  }
}

void LightInterface::solidBuffer(uint32_t *buffer, uint32_t color) {
  for (int i = 0; i < m_ledCount; i++) {
    buffer[i] = color;
  }
}

uint32_t LightInterface::interpolateColors(uint32_t color1, uint32_t color2, double pct) {
  uint32_t colorR1 = color1 & 0xFF0000;
  uint32_t colorG1 = color1 & 0x00FF00;
  uint32_t colorB1 = color1 & 0x0000FF;
  uint32_t colorR2 = color2 & 0xFF0000;
  uint32_t colorG2 = color2 & 0x00FF00;
  uint32_t colorB2 = color2 & 0x0000FF;
  return interpolateColors(colorR1, colorG1, colorB1, colorR2, colorG2, colorB2, pct);
}

uint32_t LightInterface::interpolateColors(uint32_t colorR1, uint32_t colorG1, uint32_t colorB1, uint32_t colorR2, uint32_t colorG2, uint32_t colorB2, double pct) {
  uint32_t colorR = ((int) colorR2 - (int) colorR1) * pct + colorR1; 
  uint32_t colorG = ((int) colorG2 - (int) colorG1) * pct + colorG1;
  uint32_t colorB = ((int) colorB2 - (int) colorB1) * pct + colorB1;
  return colorR & 0xFF0000 | colorG & 0x00FF00 | colorB & 0x0000FF;
}
