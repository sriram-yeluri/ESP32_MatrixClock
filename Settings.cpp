/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Settings.cpp
 ******************************************************************************/

#include "Settings.h"

#include "Constants.h"

#include <Preferences.h>

Settings::Settings()

:
m_timezone(
    Config::Time::TimezoneOffset
),

m_24Hour(
    true
),

m_brightness(
    Config::Display::DefaultBrightness
),

m_scrollSpeed(
    Config::Display::DefaultScrollSpeed
)

{

    memset(
        m_ssid,
        0,
        sizeof(m_ssid)
    );


    memset(
        m_password,
        0,
        sizeof(m_password)
    );


    memset(
        m_hostname,
        0,
        sizeof(m_hostname)
    );

}



/******************************************************************************
 * Load Settings
 ******************************************************************************/

bool Settings::begin()
{

    strlcpy(
       m_ssid,
       Config::WiFi::SSID,
       sizeof(m_ssid)
    );


    strlcpy(
       m_password,
       Config::WiFi::PASSWORD,
       sizeof(m_password)
    );


    strlcpy(
       m_hostname,
       Config::WiFi::HOSTNAME,
       sizeof(m_hostname)
    );

    Preferences prefs;
    if (prefs.begin("matrixclock", true))
    {
       m_brightness = static_cast<uint8_t>(prefs.getUChar("brightness", Config::Display::DefaultBrightness));
       m_scrollSpeed = static_cast<uint16_t>(prefs.getUInt("scroll_speed", Config::Display::DefaultScrollSpeed));
       prefs.end();
    }

    setBrightness(m_brightness);
    setScrollSpeed(m_scrollSpeed);

    return true;

}



/******************************************************************************
 * WiFi
 ******************************************************************************/

const char* Settings::wifiSSID() const
{
    return m_ssid;
}



const char* Settings::wifiPassword() const
{
    return m_password;
}



/******************************************************************************
 * Device
 ******************************************************************************/

const char* Settings::hostname() const
{
    return m_hostname;
}



/******************************************************************************
 * Time
 ******************************************************************************/

long Settings::timezoneOffset() const
{
    return m_timezone;
}



bool Settings::is24Hour() const
{
    return m_24Hour;
}



/******************************************************************************
 * Display
 ******************************************************************************/

uint8_t Settings::brightness() const
{
    return m_brightness;
}

void Settings::setBrightness(uint8_t value)
{
    if (value > Constants::Limits::MaximumBrightness)
    {
       value = Constants::Limits::MaximumBrightness;
    }

    m_brightness = value;
}

uint16_t Settings::scrollSpeed() const
{
    return m_scrollSpeed;
}

void Settings::setScrollSpeed(uint16_t value)
{
    if (value < Constants::Display::MinimumScrollSpeed)
    {
       value = Constants::Display::MinimumScrollSpeed;
    }
    else if (value > Constants::Display::MaximumScrollSpeed)
    {
       value = Constants::Display::MaximumScrollSpeed;
    }

    m_scrollSpeed = value;
}



/******************************************************************************
 * Save Settings
 ******************************************************************************/

bool Settings::save()
{
    Preferences prefs;
    if (!prefs.begin("matrixclock", false))
    {
       return false;
    }

    prefs.putUChar("brightness", m_brightness);
    prefs.putUInt("scroll_speed", m_scrollSpeed);
    prefs.end();

    return true;

}