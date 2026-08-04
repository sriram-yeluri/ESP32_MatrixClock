/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : ESP32_MatrixClock.ino
 *
 * Description:
 * Main application controller.
 *
 * Features:
 * - ESP32 WiFi
 * - NTP Clock
 * - MAX7219 Matrix Display
 * - Date / Day display
 * - Scrolling messages
 *
 ******************************************************************************/

#include "include/Config.h"
#include "include/Constants.h"
#include "include/Types.h"
#include "include/Version.h"


#include "include/Settings.h"
#include "include/NetworkManager.h"
#include "include/Clock.h"
#include "include/Display.h"
#include "include/MessageManager.h"
#include "include/PageManager.h"


/******************************************************************************
 * Global Services
 ******************************************************************************/

Settings settings;


NetworkManager network(
    settings
);


Clock clockService(
    settings
);


Display display(
    settings
);


MessageManager messages;


PageManager pages;



/******************************************************************************
 * Setup
 ******************************************************************************/

void setup()
{

    Serial.begin(
        115200
    );


    delay(500);



    Serial.println();


    Serial.println(
        Version::Name
    );


    Serial.print(
        "Firmware: "
    );


    Serial.println(
        Version::Number
    );



    /*
     * Load configuration
     */

    settings.begin();



    /*
     * Initialize hardware/services
     */

    display.begin();


    network.begin();


    clockService.begin();


    messages.begin();


    pages.begin();



    Serial.println(
        "System Ready"
    );

}



/******************************************************************************
 * Main Loop
 ******************************************************************************/

void loop()
{

    /*
     * Background services
     */

    network.update();


    clockService.update();


    messages.update();


    pages.update();



    /*
     * Display animation
     */

    display.update();



    /*
     * Render selected page
     */

    renderPage();



}



/******************************************************************************
 * Display Page Renderer
 ******************************************************************************/

void renderPage()
{

    switch(
        pages.current()
    )

    {



        case PageType::Time:

            display.showTime(

                clockService.getTime()

            );

            break;



        case PageType::Date:

            display.showDate(

                clockService.getDate()

            );

            break;



        case PageType::Day:

            display.showDay(

                clockService.getDay()

            );

            break;



        case PageType::Message:

            display.showMessage(

                messages.current()

            );

            break;



        case PageType::Version:

            display.showMessage(

                Version::Number

            );

            break;



        case PageType::WiFi:


            if(
                network.isConnected()
            )
            {

                display.showMessage(

                    "WiFi OK"

                );

            }
            else
            {

                display.showMessage(

                    "WiFi OFF"

                );

            }


            break;



        case PageType::IP:

            display.showMessage(

                network.status()
                       .ip
                       .toString()
                       .c_str()

            );

            break;



        default:


            display.showMessage(

                "Ready"

            );


            break;

    }

}