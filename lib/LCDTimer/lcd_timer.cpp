#include "lcd_timer.h"
#include "logger.h"
LCDTimer::LCDTimer(TFT_eSPI *tft)
{
    m_tft = tft;
}

LCDTimer::~LCDTimer()
{
}

void LCDTimer::updateLightModeActiveTurnTimer()
{
    m_tft->setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
                                               // so the anti-aliasing works

    
    setTextSize();

    uint32_t remainingTime = m_timerData.totalTime - m_timerData.elapsedTime; 

    // The left value of the displayed time
    uint32_t major = 0;
    // The right value of the displayed time
    uint32_t minor = 0;
    // Unit of the left value
    char unit = 's';
    char separator = ':';

    uint32_t ms = remainingTime % 1000 / 10;
    remainingTime = remainingTime / 1000;
    uint32_t s = remainingTime % 60;
    remainingTime = remainingTime / 60;
    uint32_t m = remainingTime % 60;
    remainingTime = remainingTime / 60;
    uint32_t h = remainingTime / 24;

    if (h > 0) {
        major = h;
        minor = m;
        unit = 'h';
    } else if (m > 0) {
        major = m;
        minor = s;
        unit = 'm';
    } else {
        major = s;
        minor = ms;
        unit = 's';
        separator = '.';
    }

    char displayBuffer[8];
    sprintf(displayBuffer, "%02d%c%02d %c\0", major, separator, minor, unit);

    uint16_t textWidth = m_tft->textWidth(displayBuffer);
    uint16_t fontHeight = m_tft->fontHeight();
    uint16_t cursorX = m_tft->width() / 2 - textWidth / 2;
    uint16_t cursorY = m_tft->height() / 2 - fontHeight / 2;
    m_tft->setCursor(cursorX, cursorY); // Set cursor at top left of screen

    m_tft->print(displayBuffer);
}

void LCDTimer::updateLightModeActiveTurnNoTimer()
{
    // No display in mode
}

void LCDTimer::updateLightModeSkipped()
{
    // No display in mode
}

void LCDTimer::updateLightModeTurnSequence()
{
    // No display in mode
}

void LCDTimer::updateLightModeAwaitGameStart()
{
    // No display in mode
}

void LCDTimer::updateLightModeAwaitConnection()
{
    // No display in mode
}

void LCDTimer::updateGameDebug()
{
    // No display in mode
}

void LCDTimer::updateGamePaused()
{
    // No display in mode
}

bool LCDTimer::getClearBeforeUpdate() const
{
    return m_clearBeforeUpdate;
}

void LCDTimer::begin()
{
}

void LCDTimer::clear()
{
    setTextSize();
    // Get the width and height of text that is of the same size as what can be rendered
    uint16_t boxWidth = m_tft->textWidth("       ") + 4;  // +4 to cover just more than text size;
    uint16_t boxHeight = m_tft->fontHeight() + 4;  // +4 to cover just more than text size;
    uint16_t boxX = m_tft->width() / 2 - boxWidth / 2;
    uint16_t boxY = m_tft->height() / 2 - boxHeight / 2;
    m_tft->fillRect(boxX, boxY, boxWidth, boxHeight, TFT_BACKGROUND_COLOR);
}

void LCDTimer::show()
{
}

void LCDTimer::setTextSize()
{
    m_tft->setTextSize(3);  // Set text size (1 = small, 2 = medium, etc.)
}
