#include "BGDisplayFaceTextBase.h"
#include "BGDisplayManager.h"
#include "globals.h"

void BGDisplayFaceTextBase::showReadingBase(const GlucoseReading reading, int16_t x, int16_t y, bool centered) const {
    String readingToDisplay = "";
    if (SettingsManager.settings.bgUnit == MGDL) {
        readingToDisplay += String(reading.sgv);
    } else {
        char buffer[10];
        sprintf(buffer, "%.1f", round((float)reading.sgv / 1.8) / 10);
        readingToDisplay += String(buffer);
    }

    switch (BGDisplayManager.getGlucoseIntervals().getBGLevel(reading.sgv)) {
        case URGENT_LOW:
        case URGENT_HIGH:
            DisplayManager.setTextColor(COLOR_RED);
            break;
        case WARNING_LOW:
        case WARNING_HIGH:
            DisplayManager.setTextColor(COLOR_YELLOW);
            break;
        case NORMAL:
            DisplayManager.setTextColor(COLOR_GREEN);
            break;
        default:
            DisplayManager.setTextColor(COLOR_GRAY);
            break;
    }

    DisplayManager.printText(x, y, readingToDisplay.c_str(), centered, 2);
}

#pragma region Show arrow

// Glucose trends
const uint8_t symbol_doubleUp[] PROGMEM = {
    0x50, 0xF8, 0x50, 0x50, 0x50,
};
const uint8_t symbol_singleUp[] PROGMEM = {
    0x20, 0x70, 0xA8, 0x20, 0x20,
};
const uint8_t symbol_fortyFiveUp[] PROGMEM = {
    0x38, 0x18, 0x28, 0x40, 0x80,
};
const uint8_t symbol_flat[] PROGMEM = {
    0x20, 0x10, 0xF8, 0x10, 0x20,
};
const uint8_t symbol_fortyFiveDown[] PROGMEM = {
    0x80, 0x40, 0x28, 0x18, 0x38,
};
const uint8_t symbol_singleDown[] PROGMEM = {
    0x20, 0x20, 0xA8, 0x70, 0x20,
};
const uint8_t symbol_doubleDown[] PROGMEM = {
    0x50, 0x50, 0x50, 0xF8, 0x50,
};

const uint8_t *glucoseTrendSymbols[] = {
    symbol_doubleUp, symbol_singleUp, symbol_fortyFiveUp, symbol_flat, symbol_fortyFiveDown, symbol_singleDown, symbol_doubleDown,
};

void BGDisplayFaceTextBase::showTrendArrow(const GlucoseReading reading, int16_t x, int16_t y) const {
    const uint8_t *trendSymbol = glucoseTrendSymbols[4];

    if (reading.trend > 0 && reading.trend < 8) {
        trendSymbol = glucoseTrendSymbols[reading.trend - 1];
    }

    DisplayManager.drawBitmap(x, y, trendSymbol, 5, 5, COLOR_WHITE);
}

#pragma endregion Show arrow