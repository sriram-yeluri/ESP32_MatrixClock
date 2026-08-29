/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Settings.cpp
 ******************************************************************************/

#include "Settings.h"

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

    /*
     * Current version:
     * Load defaults from Config.h
     *
     * Future:
     * Load from ESP32 Preferences
     */

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

/******************************************************************************
 * Save Placeholder
 ******************************************************************************/

bool Settings::save()
{

    /*
     * Future implementation:
     *
     * Preferences prefs;
     *
     * prefs.begin("clock");
     *
     */
    return true;
}