/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : MessageManager.cpp
 ******************************************************************************/

#include "include/MessageManager.h"


/******************************************************************************
 * Constructor
 ******************************************************************************/

MessageManager::MessageManager()

:
m_count(0),

m_current(0),

m_lastChange(0)

{

}



/******************************************************************************
 * Begin
 ******************************************************************************/

void MessageManager::begin()
{

    add(
        "Welcome",
        5
    );


    add(
        "ESP32 Matrix Clock",
        5
    );


    add(
        "Have A Nice Day",
        5
    );

}



/******************************************************************************
 * Update
 ******************************************************************************/

void MessageManager::update()
{

    if(
        m_count == 0
    )
    {
        return;
    }



    uint32_t now =
        millis();



    if(

        now - m_lastChange

        >=

        m_messages[m_current].duration * 1000UL

    )
    {

        next();


        m_lastChange =
            now;

    }

}



/******************************************************************************
 * Current
 ******************************************************************************/

const char* MessageManager::current() const
{

    if(
        m_count == 0
    )
    {
        return "";
    }


    return
        m_messages[m_current].text;

}



/******************************************************************************
 * Next
 ******************************************************************************/

void MessageManager::next()
{

    selectNext();

}



/******************************************************************************
 * Select next enabled message
 ******************************************************************************/

void MessageManager::selectNext()
{

    if(
        m_count == 0
    )
    {
        return;
    }



    for(
        uint8_t i = 0;

        i < m_count;

        i++
    )
    {


        m_current++;


        if(
            m_current >= m_count
        )
        {
            m_current = 0;
        }



        if(
            m_messages[m_current].enabled
        )
        {
            break;
        }

    }

}



/******************************************************************************
 * Add
 ******************************************************************************/

bool MessageManager::add(

    const char* text,

    uint16_t seconds

)
{

    if(
        m_count >=
        Constants::Limits::MaximumMessages
    )
    {
        return false;
    }



    DisplayMessage& msg =
        m_messages[m_count];



    strlcpy(

        msg.text,

        text,

        sizeof(msg.text)

    );



    msg.enabled =
        true;


    msg.duration =
        seconds;


    msg.type =
        MessageType::Normal;



    m_count++;



    return true;

}



/******************************************************************************
 * Clear
 ******************************************************************************/

void MessageManager::clear()
{

    m_count =
        0;


    m_current =
        0;


    m_lastChange =
        0;


    m_current =
        0;

}



/******************************************************************************
 * Count
 ******************************************************************************/

uint8_t MessageManager::count() const
{
    return m_count;
}