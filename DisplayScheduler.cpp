#include "DisplayScheduler.h"

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

    if (current == PageType::Time)
    {
        m_display.showTime(m_clock.getTime());
    }

    bool pageChanged = m_firstRender || (current != m_lastPage);

    if (pageChanged)
    {
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
            m_display.showTime(m_clock.getTime());
            break;

        case PageType::Date:
            m_display.showDate(m_clock.getDate());
            break;

        case PageType::Day:
            m_display.showDay(m_clock.getDay());
            break;

        case PageType::Message:
            m_display.showMessage(m_messages.current());
            break;

        default:
            break;
    }
}