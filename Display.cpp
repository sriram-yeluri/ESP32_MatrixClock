/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Display.cpp
 ******************************************************************************/

#include "Display.h"
#include "Font_Data.h"

namespace
{
    constexpr uint16_t ScrollSpeed = Constants::Display::DefaultScrollSpeed;
}


/******************************************************************************
 * Constructor
 ******************************************************************************/

Display::Display(Settings& settings)
:

m_settings(settings),

m_display(

    MD_MAX72XX::FC16_HW,
    Config::Matrix::DataPin,
    Config::Matrix::ClockPin,
    Config::Matrix::CSPin,
    Config::Matrix::Devices
)

{
    memset(m_currentText,0,sizeof(m_currentText));
    memset(m_clockText,0,sizeof(m_clockText));
    memset(m_secondsText,0,sizeof(m_secondsText));

    m_threeZoneLayout = false;

    m_brightness = m_settings.brightness();

}

/******************************************************************************
 * Begin
 ******************************************************************************/

void Display::begin()
{
    const uint8_t deviceCount = Config::Matrix::Devices;
    const uint8_t zoneCount = (deviceCount >= 8) ? 3 : ((deviceCount >= 2) ? 2 : 1);

    m_display.begin(zoneCount);

    m_display.setIntensity(
        m_brightness
    );

    m_display.displayClear();

    if (zoneCount == 1)
    {
        m_threeZoneLayout = false;
        m_display.setZone(0, 0, 0);
    }
    else if (zoneCount == 3)
    {
        m_threeZoneLayout = true;

        // Exact 3-zone reference layout for 8 modules:
        // zone0 = 0..3 (scroll content), zone1 = 4..4 (seconds), zone2 = 5..7 (time)
        m_display.setZone(0, 0, 3);
        m_display.setZone(1, 4, 4);
        m_display.setZone(2, 5, 7);

        m_display.setFont(1, const_cast<MD_MAX72XX::fontType_t*>(numeric7Seg));
        m_display.setFont(2, const_cast<MD_MAX72XX::fontType_t*>(numeric7Se));
        m_display.setCharSpacing(1, 0);
        m_display.setCharSpacing(2, 0);
    }
    else
    {
        m_threeZoneLayout = false;

        const uint8_t leftEnd = (deviceCount / 2) - 1;
        const uint8_t rightStart = deviceCount / 2;

        m_display.setZone(0, rightStart, deviceCount - 1);
        m_display.setZone(1, 0, leftEnd);
    }

}


/******************************************************************************
 * Update
 ******************************************************************************/

void Display::update()
{
    m_display.displayAnimate();
}

/******************************************************************************
 * Internal renderer
 ******************************************************************************/

void Display::renderZone(
    uint8_t zone,
    const char* text,
    textPosition_t position,
    textEffect_t effect
)
{
    if (text == nullptr)
        return;

    const bool isClockZone   = (zone == 0 && !m_threeZoneLayout) || (zone == 2 && m_threeZoneLayout);
    const bool isSecondsZone = (zone == 1 && m_threeZoneLayout);

    if (isClockZone)
    {
        if (strcmp(m_clockText, text) == 0) return;
        strlcpy(m_clockText, text, sizeof(m_clockText));
    }
    else if (isSecondsZone)
    {
        if (strcmp(m_secondsText, text) == 0) return;
        strlcpy(m_secondsText, text, sizeof(m_secondsText));
    }
    else
    {
        strlcpy(m_currentText, text, sizeof(m_currentText));
    }

    const char* zoneText = isClockZone   ? m_clockText
                         : isSecondsZone ? m_secondsText
                         : m_currentText;

    if (isClockZone || isSecondsZone || effect == PA_PRINT)
        m_display.displayZoneText(zone, zoneText, position, 0, 0, PA_PRINT, PA_NO_EFFECT);
    else
        m_display.displayZoneText(zone, zoneText, position, ScrollSpeed, 0, effect, effect);
}



/******************************************************************************
 * Time
 ******************************************************************************/

void Display::showTime(const char* text)
{
    if (text == nullptr)
    {
        return;
    }

    char clockText[Constants::Buffer::TimeSize];
    strlcpy(clockText, text, sizeof(clockText));

    char* firstColon = strchr(clockText, ':');
    char* lastColon = strrchr(clockText, ':');
    if (firstColon != nullptr && lastColon != nullptr && firstColon != lastColon)
    {
        *lastColon = '\0';
    }

    char formattedClockText[Constants::Buffer::TimeSize];
    strlcpy(formattedClockText, clockText, sizeof(formattedClockText));

    if (strlen(clockText) >= 5 && clockText[2] == ':')
    {
        snprintf(
            formattedClockText,
            sizeof(formattedClockText),
            "%c %c : %c %c",
            clockText[0],
            clockText[1],
            clockText[3],
            clockText[4]
        );
    }
    else
    {
        char* separator = strchr(clockText, ':');
        if (separator != nullptr)
        {
            const int leftLen = static_cast<int>(separator - clockText);
            snprintf(
                formattedClockText,
                sizeof(formattedClockText),
                "%.*s : %s",
                leftLen,
                clockText,
                separator + 1
            );
        }
    }

    renderZone(m_threeZoneLayout ? 2 : 0, formattedClockText, PA_LEFT, PA_PRINT);
}

/******************************************************************************
 * Seconds
 ******************************************************************************/

void Display::showSeconds(const char* text)
{
    if (text == nullptr || !m_threeZoneLayout)
    {
        return;
    }

    const char s0 = (text[0] != '\0') ? text[0] : '0';
    const char s1 = (text[1] != '\0') ? text[1] : '0';

    char secondsText[4];
    snprintf(secondsText, sizeof(secondsText), "%c %c", s0, s1);
    renderZone(1, secondsText, PA_LEFT, PA_PRINT);
}


/******************************************************************************
 * Date
 ******************************************************************************/

void Display::showDate(const char* text)
{
    renderZone(m_threeZoneLayout ? 0 : 1, text, PA_LEFT, PA_SCROLL_LEFT);
}


/******************************************************************************
 * Day
 ******************************************************************************/

void Display::showDay(
    const char* text
)
{

    renderZone(

        m_threeZoneLayout ? 0 : 1,

        text,

        PA_LEFT,

        PA_SCROLL_LEFT

    );

}



/******************************************************************************
 * Message
 ******************************************************************************/

void Display::showMessage(
    const char* text
)
{

    renderZone(

        m_threeZoneLayout ? 0 : 1,

        text,

        PA_LEFT,

        PA_SCROLL_LEFT

    );

}



/******************************************************************************
 * Brightness
 ******************************************************************************/

void Display::setBrightness(
    uint8_t value
)
{

    if(
        value >
        Constants::Limits::MaximumBrightness
    )
    {

        value =
            Constants::Limits::MaximumBrightness;

    }

    m_brightness =
        value;

    m_display.setIntensity(
        value
    );

}

/******************************************************************************
 * Brightness getter
 ******************************************************************************/

uint8_t Display::brightness() const
{
    return m_brightness;
}

/******************************************************************************
 * Clear
 ******************************************************************************/

void Display::clear()
{

    m_display.displayClear();


    memset(

        m_currentText,

        0,

        sizeof(m_currentText)

    );

    memset(

        m_clockText,

        0,

        sizeof(m_clockText)

    );

    memset(

        m_secondsText,

        0,

        sizeof(m_secondsText)

    );

}