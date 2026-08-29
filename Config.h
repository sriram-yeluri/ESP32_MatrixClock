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
    constexpr char PASSWORD[] = "xxxxxxx";
    constexpr char HOSTNAME[] = "ESP32-MatrixClock";
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
    constexpr uint8_t DataPin = 23;
    constexpr uint8_t ClockPin = 18;
    constexpr uint8_t CSPin = 5;
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
    constexpr uint16_t DefaultScrollSpeed = 65;
}


}

#endif