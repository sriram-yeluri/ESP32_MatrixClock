/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Display.cpp
 ******************************************************************************/

#include "Display.h"

namespace
{

    constexpr uint16_t ScrollSpeed = Constants::Display::DefaultScrollSpeed;

}



/******************************************************************************
 * Constructor
 ******************************************************************************/

Display::Display(
    Settings& settings
)

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

    memset(
        m_currentText,
        0,
        sizeof(m_currentText)
    );


    m_brightness =
        m_settings.brightness();

}

/******************************************************************************
 * Begin
 ******************************************************************************/

void Display::begin()
{

    m_display.begin();

    m_display.setIntensity(
        m_brightness
    );

    m_display.displayClear();

    m_display.setTextAlignment(
        PA_CENTER
    );

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

void Display::render(
    const char* text,
    textPosition_t position,
    textEffect_t effect
)
{

    strlcpy(
        m_currentText,
        text,
        sizeof(m_currentText)
    );

    m_display.displayText(

        m_currentText,

        position,

        ScrollSpeed,

        0,

        effect,

        effect

    );

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

    if (strcmp(text, m_currentText) == 0)
    {
        return;
    }

    render(text, PA_CENTER, PA_NO_EFFECT);
}


/******************************************************************************
 * Date
 ******************************************************************************/

void Display::showDate(const char* text)
{
    render(text,PA_CENTER,PA_SCROLL_LEFT);
}


/******************************************************************************
 * Day
 ******************************************************************************/

void Display::showDay(
    const char* text
)
{

    render(

        text,

        PA_CENTER,

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

    render(

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

}