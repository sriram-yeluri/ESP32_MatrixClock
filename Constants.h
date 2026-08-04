/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 ******************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <Arduino.h>


namespace Constants
{


namespace Buffer
{

constexpr uint16_t MessageSize = 128;

constexpr uint8_t TimeSize = 10;

constexpr uint8_t DateSize = 16;

constexpr uint8_t DaySize = 12;

}



namespace Timing
{

constexpr uint32_t NetworkReconnect =
    15000UL;


constexpr uint32_t NetworkStatusUpdate =
    5000UL;


constexpr uint32_t ClockUpdate =
    1000UL;


constexpr uint32_t DisplayUpdate =
    20UL;


}



namespace Display
{

constexpr uint16_t DefaultScrollSpeed =
    65;

}



namespace Message
{

constexpr uint16_t DefaultDuration =
    5;

}



namespace Limits
{

constexpr uint8_t MaximumMessages =
    10;


constexpr uint8_t MaximumPages =
    8;


constexpr uint8_t MaximumBrightness =
    15;

}



}


#endif