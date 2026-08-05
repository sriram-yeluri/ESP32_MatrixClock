/******************************************************************************
 * DisplayScheduler.h
 *
 * Coordinates page transitions and rendering.
 *
 * Responsibilities
 * ----------------
 * - Update PageManager
 * - Detect page changes
 * - Advance messages
 * - Render current page
 *
 * Does NOT
 * --------
 * - Draw pixels
 * - Animate text
 * - Manage fonts
 ******************************************************************************/

#ifndef DISPLAY_SCHEDULER_H
#define DISPLAY_SCHEDULER_H

#include "Clock.h"
#include "Display.h"
#include "MessageManager.h"
#include "PageManager.h"

class DisplayScheduler
{
public:

    DisplayScheduler(
        Display& display,
        PageManager& pages,
        Clock& clock,
        MessageManager& messages);

    void begin();
    void update();

private:

    void renderCurrentPage();

    Display&        m_display;
    PageManager&    m_pages;
    Clock&          m_clock;
    MessageManager& m_messages;

    bool            m_firstRender;
    PageType        m_lastPage;
};

#endif