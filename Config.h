/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 * Version      : 2.0.0
 *
 * File         : Config.h
 *
 * Description:
 * Hardware and user configuration.
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>


namespace Config
{

namespace WiFi
{
    constexpr char SSID[] = "VijayKrishna";
    constexpr char PASSWORD[] = "Welkome2Guest";
    constexpr char HOSTNAME[] = "MatrixClock";
}

namespace Time
{
    // India Standard Time UTC+5:30
    constexpr long TimezoneOffset = 19800;
    constexpr int DaylightOffset = 0;
    constexpr char NtpServer[] = "pool.ntp.org";
}

namespace Matrix
{
#if defined(ESP8266)
    // Wemos D1 mini: D7 (GPIO13), D5 (GPIO14), D2 (GPIO4).
    constexpr uint8_t DataPin = 13;
    constexpr uint8_t ClockPin = 14;
    constexpr uint8_t CSPin = 4;
#elif defined(ESP32)
    constexpr uint8_t DataPin = 23;
    constexpr uint8_t ClockPin = 18;
    constexpr uint8_t CSPin = 5;
#else
#error "Select an ESP32 or ESP8266 board."
#endif
    constexpr uint8_t Devices = 16;

    constexpr uint8_t ThreeZoneDevices = 16;
    constexpr uint8_t ThreeZoneClockDevices = 3;
    constexpr uint8_t ThreeZoneSecondsDevices = 1;
    constexpr uint8_t ThreeZoneScrollDevices =
        ThreeZoneDevices - ThreeZoneClockDevices - ThreeZoneSecondsDevices;
}

namespace Display
{
    constexpr uint8_t DefaultBrightness = 3;
}


}

#endif