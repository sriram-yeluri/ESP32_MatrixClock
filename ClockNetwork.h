/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : ClockNetwork.h
 *
 * Description:
 * WiFi and NTP network service.
 ******************************************************************************/

#ifndef CLOCK_NETWORK_H
#define CLOCK_NETWORK_H


#include "Platform.h"

#include "Settings.h"
#include "Types.h"
#include "Config.h"
#include "Constants.h"



class ClockNetwork
{

public:
    explicit ClockNetwork(Settings& settings);


    /*
     * Initialize network.
     */
    bool begin();

    /*
     * Background service.
     */
    void update();

    /*
     * Force reconnect.
     */
    void reconnect();

    /*
     * Status.
     */
    NetworkStatus status() const;

    bool isConnected() const;

private:
    void connect();
    void startAccessPoint();
    void syncTime();
    void refreshStatus();

    Settings& m_settings;
    NetworkStatus m_status;
    uint32_t m_lastReconnect;
    uint32_t m_lastStatusUpdate;
    uint32_t m_lastSyncAttempt;
    bool m_apMode;
};



#endif