/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : PageManager.h
 *
 * Description:
 * Controls display page rotation.
 ******************************************************************************/

#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H


#include <Arduino.h>


#include "Types.h"
#include "Constants.h"


struct PageConfig
{

    PageType page;

    bool enabled;

    uint16_t duration;

};


class PageManager
{

public:

    PageManager();


    /*
     * Initialize default pages.
     */
    void begin();


    /*
     * Background update.
     */
    void update();


    /*
     * Current page.
     */
    PageType current() const;


    /*
     * Next page.
     */
    void next();


    /*
     * Select specific page.
     */
    void set(
        PageType page
    );



    /*
     * Enable / disable page.
     */
    void enable(
        PageType page,
        bool state
    );



private:

    void selectNext();

    PageConfig m_pages[
        Constants::Limits::MaximumPages
    ];

    uint8_t m_count;
    uint8_t m_current;
    uint32_t m_lastChange;

};


#endif