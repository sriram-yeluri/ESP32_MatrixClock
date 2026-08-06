/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : Display.h
 *
 * Description:
 * MAX7219 matrix display controller.
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H


#include <Arduino.h>


#include <MD_Parola.h>
#include <MD_MAX72xx.h>


#include "Settings.h"
#include "Config.h"
#include "Constants.h"



class Display
{

public:


    explicit Display(
        Settings& settings
    );



    /*
     * Initialize display.
     */
    void begin();



    /*
     * Display animation update.
     */
    void update();



    /*
     * Display content.
     */

    void showTime(
        const char* text
    );


    void showSeconds(
        const char* text
    );


    void showDate(
        const char* text
    );


    void showDay(
        const char* text
    );


    void showMessage(
        const char* text
    );


    /*
     * Brightness.
     */

    void setBrightness(
        uint8_t value
    );

    uint8_t brightness() const;



    /*
     * Clear display.
     */

    void clear();



private:

    void renderZone(
        uint8_t zone,
        const char* text,
        textPosition_t position,
        textEffect_t effect
    );

    Settings& m_settings;

    MD_Parola m_display;

    char m_currentText[
        Constants::Buffer::MessageSize
    ];

    char m_clockText[
        Constants::Buffer::TimeSize
    ];

    char m_secondsText[
        8
    ];

    bool m_threeZoneLayout;

    uint8_t m_brightness;

};



#endif