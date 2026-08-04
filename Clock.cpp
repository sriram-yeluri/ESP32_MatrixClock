/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Clock.cpp
 ******************************************************************************/

#include "Clock.h"



Clock::Clock(
    Settings& settings
)

:
m_settings(settings),

m_lastUpdate(0)

{

    memset(
        &m_time,
        0,
        sizeof(m_time)
    );


    memset(
        &m_status,
        0,
        sizeof(m_status)
    );


    memset(
        m_timeBuffer,
        0,
        sizeof(m_timeBuffer)
    );


    memset(
        m_secondsBuffer,
        0,
        sizeof(m_secondsBuffer)
    );


    memset(
        m_dateBuffer,
        0,
        sizeof(m_dateBuffer)
    );


    memset(
        m_dayBuffer,
        0,
        sizeof(m_dayBuffer)
    );

}



/******************************************************************************
 * Begin
 ******************************************************************************/

void Clock::begin()
{

    refresh();

}



/******************************************************************************
 * Update
 ******************************************************************************/

void Clock::update()
{

    uint32_t now =
        millis();



    if(
        now - m_lastUpdate
        >=
        Constants::Timing::ClockUpdate
    )
    {

        m_lastUpdate =
            now;


        refresh();

    }

}



/******************************************************************************
 * Refresh Time
 ******************************************************************************/

void Clock::refresh()
{

    if(
        !getLocalTime(
            &m_time,
            100
        )
    )
    {

        m_status.state =
            ClockState::Invalid;


        return;

    }



    m_status.state =
        ClockState::Valid;



    m_status.hour =
        m_time.tm_hour;


    m_status.minute =
        m_time.tm_min;


    m_status.second =
        m_time.tm_sec;


    m_status.day =
        m_time.tm_mday;


    m_status.month =
        m_time.tm_mon + 1;


    m_status.year =
        m_time.tm_year + 1900;



    /*
     * Time format
     */

    if(
        m_settings.is24Hour()
    )
    {

        strftime(
            m_timeBuffer,
            sizeof(m_timeBuffer),
            "%H:%M",
            &m_time
        );

    }
    else
    {

        strftime(
            m_timeBuffer,
            sizeof(m_timeBuffer),
            "%I:%M %p",
            &m_time
        );


    }



    strftime(
        m_secondsBuffer,
        sizeof(m_secondsBuffer),
        "%S",
        &m_time
    );



    strftime(
        m_dateBuffer,
        sizeof(m_dateBuffer),
        "%d %b %Y",
        &m_time
    );



    strftime(
        m_dayBuffer,
        sizeof(m_dayBuffer),
        "%A",
        &m_time
    );

}



/******************************************************************************
 * Status
 ******************************************************************************/

ClockStatus Clock::status() const
{
    return m_status;
}



/******************************************************************************
 * Getters
 ******************************************************************************/

const char* Clock::getTime() const
{
    return m_timeBuffer;
}



const char* Clock::getSeconds() const
{
    return m_secondsBuffer;
}



const char* Clock::getDate() const
{
    return m_dateBuffer;
}



const char* Clock::getDay() const
{
    return m_dayBuffer;
}