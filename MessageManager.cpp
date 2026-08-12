/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : MessageManager.cpp
 ******************************************************************************/

#include "MessageManager.h"

#include <Preferences.h>


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
    if (!loadFromStorage())
    {
        add("Welcome Home ...!",5);
        add("Have A Nice Day",5);
        add("VijayKrishnaSai is a smart kid",5);
        add("Vishwateja is a naughty kid", 5);
        add("Devaki Devi is the Queen of our House", 5);
        add(" Jai Sri Ram *** Jai Sri Ram *** Jai Sri Ram", 5);
        saveToStorage();
    }

    m_current = 0;
}

/******************************************************************************
 * Update
 *
 * Message rotation is now controlled by PageManager.
 ******************************************************************************/

void MessageManager::update()
{
    // Intentionally empty.
    //
    // The application advances to the next message only when the
    // Message page becomes active.
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

    if (m_count == 0)
        return;

    selectNext();

}



/******************************************************************************
 * Select next enabled message
 ******************************************************************************/

void MessageManager::selectNext()
{
    if (m_count == 0) return;

    // Priority pass: advance to the next pinned message if one exists
    for (uint8_t i = 0; i < m_count; ++i)
    {
        uint8_t candidate = (m_current + 1 + i) % m_count;
        if (m_messages[candidate].enabled && m_messages[candidate].type == MessageType::Priority)
        {
            m_current = candidate;
            return;
        }
    }

    // Fallback pass: advance to the next enabled message
    for (uint8_t i = 0; i < m_count; ++i)
    {
        uint8_t candidate = (m_current + 1 + i) % m_count;
        if (m_messages[candidate].enabled)
        {
            m_current = candidate;
            return;
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
    saveToStorage();

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

    saveToStorage();
}

/******************************************************************************
 * Remove one message
 ******************************************************************************/

bool MessageManager::remove(uint8_t index)
{
    if (index >= m_count)
    {
        return false;
    }

    for (uint8_t i = index; i + 1 < m_count; ++i)
    {
        m_messages[i] = m_messages[i + 1];
    }

    m_count--;

    if (m_current >= m_count)
    {
        m_current = (m_count == 0) ? 0 : m_count - 1;
    }

    saveToStorage();
    return true;
}

bool MessageManager::updateAt(uint8_t index, const char* text, uint16_t seconds, bool priority)
{
    if (index >= m_count)
    {
        return false;
    }

    strlcpy(m_messages[index].text, text, sizeof(m_messages[index].text));
    m_messages[index].duration = seconds;
    m_messages[index].enabled = true;
    m_messages[index].type = priority ? MessageType::Priority : MessageType::Normal;
    saveToStorage();
    return true;
}

bool MessageManager::pinAt(uint8_t index, bool priority)
{
    if (index >= m_count)
    {
        return false;
    }

    m_messages[index].type = priority ? MessageType::Priority : MessageType::Normal;
    saveToStorage();
    return true;
}

/****************************************************************************
 * Count
 ******************************************************************************/

uint8_t MessageManager::count() const
{
    return m_count;
}

/****************************************************************************
 * Get by index
 ******************************************************************************/

const char* MessageManager::get(uint8_t index) const
{
    if (index >= m_count)
    {
        return "";
    }

    return m_messages[index].text;
}

bool MessageManager::loadFromStorage()
{
    Preferences prefs;
    if (!prefs.begin("matrixclock", true))
    {
        return false;
    }

    clear();

    uint8_t count = prefs.getUChar("msg_count", 0);
    for (uint8_t i = 0; i < count && i < Constants::Limits::MaximumMessages; ++i)
    {
        char key[16];
        snprintf(key, sizeof(key), "msg_%u", i);
        String value = prefs.getString(key, "");
        if (value.length() == 0)
        {
            continue;
        }

        add(value.c_str(), 5);
    }

    prefs.end();
    return count > 0;
}

bool MessageManager::saveToStorage() const
{
    Preferences prefs;
    if (!prefs.begin("matrixclock", false))
    {
        return false;
    }

    prefs.putUChar("msg_count", m_count);
    for (uint8_t i = 0; i < m_count; ++i)
    {
        char key[16];
        snprintf(key, sizeof(key), "msg_%u", i);
        prefs.putString(key, m_messages[i].text);
    }

    prefs.end();
    return true;
}