/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Clock.h
 *
 * Description:
 * NTP synchronized clock service.
 ******************************************************************************/

#ifndef CLOCK_H
#define CLOCK_H


#include <Arduino.h>
#include <time.h>


#include "Settings.h"
#include "Types.h"
#include "Constants.h"



class Clock
{

public:


    explicit Clock(
        Settings& settings
    );



    /*
     * Initialize clock service.
     */
    void begin();



    /*
     * Background update.
     */
    void update();



    /*
     * Current status.
     */
    ClockStatus status() const;



    /*
     * Formatted output.
     */
    const char* getTime() const;


    const char* getSeconds() const;


    const char* getDate() const;


    const char* getDay() const;



private:


    void refresh();



    Settings& m_settings;



    ClockStatus m_status;



    tm m_time;



    uint32_t m_lastUpdate;



    char m_timeBuffer[
        Constants::Buffer::TimeSize
    ];



    char m_secondsBuffer[
        4
    ];



    char m_dateBuffer[
        Constants::Buffer::DateSize
    ];



    char m_dayBuffer[
        Constants::Buffer::DaySize
    ];

};



#endif