/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : PageManager.cpp
 ******************************************************************************/

#include "PageManager.h"


/******************************************************************************
 * Constructor
 ******************************************************************************/

PageManager::PageManager()

:
m_count(0),

m_current(0),

m_lastChange(0)

{

}


/******************************************************************************
 * Begin
 ******************************************************************************/

void PageManager::begin()
{

    m_pages[m_count++] =
    {

        PageType::Time,

        true,

        10

    };


    m_pages[m_count++] =
    {

        PageType::Date,

        true,

        10

    };

    m_pages[m_count++] =
    {

        PageType::Day,

        true,

        10

    };

    m_pages[m_count++] =
    {

        PageType::Message,

        true,

        15

    };

    /*
     * Future pages
     */

    m_pages[m_count++] =
    {

        PageType::Version,

        false,

        5

    };

    m_pages[m_count++] =
    {

        PageType::WiFi,

        false,

        5

    };

    m_lastChange =
        millis();

}

/******************************************************************************
 * Update
 ******************************************************************************/

void PageManager::update()
{

    if( m_count == 0)
    {
        return;
    }

    uint32_t now =
        millis();

    if( now - m_lastChange >= m_pages[m_current].duration * 1000UL)
    {
        next();
        m_lastChange =now;
    }

}


/******************************************************************************
 * Current Page
 ******************************************************************************/

PageType PageManager::current() const
{

    return
        m_pages[m_current].page;

}



/******************************************************************************
 * Next
 ******************************************************************************/

void PageManager::next()
{

    selectNext();

    m_lastChange =
        millis();

}

/******************************************************************************
 * Find next enabled page
 ******************************************************************************/

void PageManager::selectNext()
{

    if( m_count == 0)
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

        if(m_current >= m_count)
        {
            m_current = 0;
        }

        if(m_pages[m_current].enabled)
        {
            break;
        }

    }

}


/******************************************************************************
 * Select page
 ******************************************************************************/

void PageManager::set(
    PageType page
)
{

    for(
        uint8_t i = 0;

        i < m_count;

        i++
    )
    {

        if(m_pages[i].page == page)
        {
            m_current = i;
            m_lastChange =
                millis();

            return;
        }

    }

}


/******************************************************************************
 * Enable Page
 ******************************************************************************/

void PageManager::enable(

    PageType page,

    bool state

)
{

    for(
        uint8_t i = 0;

        i < m_count;

        i++
    )
    {

        if(m_pages[i].page == page)
        {
            m_pages[i].enabled =
                state;

            if(!state &&i == m_current)
            {
                selectNext();
                m_lastChange =
                    millis();
            }
            return;
        }

    }

}