/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : NetworkManager.h
 *
 * Description:
 * WiFi and NTP network service.
 ******************************************************************************/

#ifndef NETWORK_H
#define NETWORK_H


#include <Arduino.h>
#include <WiFi.h>

// This bridges old libraries with ESP32 Core 3.x.
#ifndef wifi_event_id_t
#define wifi_event_id_t arduino_event_id_t
#endif

#include "Settings.h"
#include "Types.h"
#include "Config.h"
#include "Constants.h"



class NetworkManager
{

public:


    explicit Network(
        Settings& settings
    );



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


    void syncTime();


    void refreshStatus();



    Settings& m_settings;



    NetworkStatus m_status;



    uint32_t m_lastReconnect;


    uint32_t m_lastStatusUpdate;


    uint32_t m_lastSyncAttempt;

};



#endif