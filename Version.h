/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 * Version      : 2.0.0
 *
 * File         : Version.h
 *
 * Description:
 * Firmware version information.
 ******************************************************************************/

#ifndef VERSION_H
#define VERSION_H


namespace Version
{

    constexpr char Name[] =
        "ESP32 Matrix Clock Professional";


    constexpr char Number[] =
        "2.0.0";


    constexpr char Build[] =
        __DATE__ " " __TIME__;

}


#endif