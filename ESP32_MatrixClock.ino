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

#include "Config.h"
#include "Constants.h"
#include "Types.h"
#include "Version.h"

#include "Settings.h"
#include "ClockNetwork.h"
#include "Clock.h"
#include "Display.h"
#include "MessageManager.h"
#include "PageManager.h"
#include "DisplayScheduler.h"


/******************************************************************************
 * Global Services
 ******************************************************************************/

Settings settings;

ClockNetwork network(settings);

Clock clockService(settings);

Display display(settings);

MessageManager messages;

PageManager pages;

DisplayScheduler scheduler(
    display,
    pages,
    clockService,
    messages);

/******************************************************************************
 * Setup
 ******************************************************************************/

void setup()
{
    scheduler.begin();

    Serial.begin(115200);

    delay(500);

    Serial.println();

    Serial.println(Version::Name);

    Serial.print("Firmware: ");

    Serial.println(Version::Number);

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
    network.update();
    clockService.update();
    messages.update();
    pages.update();
    scheduler.update();
    display.update();
}

/******************************************************************************
 * Display Page Renderer
 ******************************************************************************/

void renderPage()
{
    switch(pages.current())
    {
        case PageType::Time:

            display.showTime(clockService.getTime());
            break;
        case PageType::Date:

            display.showDate(clockService.getDate());
            break;

        case PageType::Day:

            display.showDay( clockService.getDay());
            break;

        case PageType::Message:
        {
            display.showMessage(messages.current());
            break;
        }

        case PageType::Version:

            display.showMessage(Version::Number);
            break;

        case PageType::WiFi:
            if(
                network.isConnected()
            )
            {
                display.showMessage("WiFi OK");
            }
            else
            {
                display.showMessage("WiFi OFF");

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
            display.showMessage("Ready");
            break;
    }

}