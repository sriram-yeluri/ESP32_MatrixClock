/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Settings.h
 *
 * Description:
 * Central application settings.
 ******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H


#include <Arduino.h>

#include "Config.h"


class Settings
{

public:

    Settings();
    bool begin();


    /*
     * WiFi
     */

    const char* wifiSSID() const;
    const char* wifiPassword() const;

    /*
     * Device
     */

    const char* hostname() const;


    /*
     * Time
     */

    long timezoneOffset() const;
    bool is24Hour() const;



    /*
     * Display
     */

    uint8_t brightness() const;



    /*
     * Future:
     * ESP32 Preferences storage
     */

    bool save();



private:

    char m_ssid[64];

    char m_password[64];

    char m_hostname[32];

    long m_timezone;

    bool m_24Hour;

    uint8_t m_brightness;

};



#endif