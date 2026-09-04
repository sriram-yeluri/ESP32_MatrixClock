#include "DisplayScheduler.h"

namespace
{

const char* pageName(PageType page)
{
    switch (page)
    {
        case PageType::Time: return "Time";
        case PageType::Date: return "Date";
        case PageType::Day: return "Day";
        case PageType::Message: return "Message";
        case PageType::PacMan: return "PacMan";
        case PageType::Version: return "Version";
        case PageType::WiFi: return "WiFi";
        case PageType::IP: return "IP";
        default: return "Unknown";
    }
}

}

DisplayScheduler::DisplayScheduler(
    Display& display,
    PageManager& pages,
    Clock& clock,
    MessageManager& messages)
:
    m_display(display),
    m_pages(pages),
    m_clock(clock),
    m_messages(messages),
    m_firstRender(true),
    m_lastPage(PageType::Time)
{
}

void DisplayScheduler::begin()
{
}

void DisplayScheduler::update()
{
    PageType current = m_pages.current();

    m_display.showTime(m_clock.getTime());
    m_display.showSeconds(m_clock.getSeconds());

    bool pageChanged = m_firstRender || (current != m_lastPage);

    if (pageChanged)
    {
        Serial.print("[Display] Page: ");
        Serial.println(pageName(current));

        renderCurrentPage();

        if (current == PageType::Message)
            m_messages.next();

        m_lastPage = current;
        m_firstRender = false;
    }
}

void DisplayScheduler::renderCurrentPage()
{
    switch (m_pages.current())
    {
        case PageType::Time:
        case PageType::Date:
            m_display.showDate(m_clock.getDate());
            Serial.print("[Display] Content(Date): ");
            Serial.println(m_clock.getDate());
            break;

        case PageType::Day:
            m_display.showDay(m_clock.getDay());
            Serial.print("[Display] Content(Day): ");
            Serial.println(m_clock.getDay());
            break;

        case PageType::Message:
            m_display.showMessage(m_messages.current());
            Serial.print("[Display] Content(Message): ");
            Serial.println(m_messages.current());
            break;

        case PageType::PacMan:
            m_display.showPacMan();
            break;

        default:
            break;
    }
}