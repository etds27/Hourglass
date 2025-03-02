#pragma once
#include "hg_display_interface.h"
#include <TFT_eSPI.h>
#include <vector>

class LCDTimer : public HGDisplayInterface
{
public:
    LCDTimer(TFT_eSPI *tft);
    ~LCDTimer();
    // MARK: Light Modes
    void updateLightModeActiveTurnTimer();
    void updateLightModeActiveTurnNoTimer();
    void updateLightModeSkipped();
    void updateLightModeTurnSequence();
    void updateLightModeAwaitGameStart();
    void updateLightModeAwaitConnection();
    void updateGameDebug();
    // Light ring mode for when the game is paused by the companion app
    void updateGamePaused();
    void clear() override;


protected:
    bool m_clearBeforeUpdate = false;
    bool getClearBeforeUpdate() const override;

private:
    uint32_t m_refreshRate = 1000;
    void begin();
    void show();
    void setTextSize();

    TFT_eSPI *m_tft;
};