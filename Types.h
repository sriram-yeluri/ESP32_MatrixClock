/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 ******************************************************************************/

#ifndef TYPES_H
#define TYPES_H


#include <Arduino.h>


enum class ClockState
{

    Invalid,

    Valid

};



struct ClockStatus
{

    ClockState state;


    uint8_t hour;

    uint8_t minute;

    uint8_t second;


    uint8_t day;

    uint8_t month;


    uint16_t year;

};



enum class NetworkState
{

    Disconnected,

    Connecting,

    Connected,

    NtpSync

};



struct NetworkStatus
{

    NetworkState state;


    bool connected;


    bool timeSynced;


    int32_t rssi;


    IPAddress ip;

};



enum class PageType
{

    Time,

    Date,

    Day,

    Message,

    PacMan,

    Version,

    WiFi,

    IP

};



enum class MessageType
{

    Normal,

    Priority

};



#endif