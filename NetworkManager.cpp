/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : NetworkManager.cpp
 ******************************************************************************/

#include "include/NetworkManager.h"
#include <time.h>


Network::Network(
    Settings& settings
)

:
m_settings(settings),

m_lastReconnect(0),

m_lastStatusUpdate(0),

m_lastSyncAttempt(0)

{


    m_status.state =
        NetworkState::Disconnected;
        m_status.timeSynced =
            false;


    m_status.connected =
        false;


    m_status.timeSynced =
        false;


    m_status.rssi =
        0;

}



/******************************************************************************
 * Begin
 ******************************************************************************/

bool Network::begin()
{

    WiFi.mode(
        WIFI_STA
    );


    WiFi.setHostname(
        m_settings.hostname()
    );


    connect();


    return true;

}



/******************************************************************************
 * Start Connection
 ******************************************************************************/

void Network::connect()
{

    Serial.print(
        "Connecting WiFi"
    );


    Serial.println();



    m_status.state =
        NetworkState::Connecting;



    WiFi.begin(
        m_settings.wifiSSID(),
        m_settings.wifiPassword()
    );

}



/******************************************************************************
 * Background Update
 ******************************************************************************/

void Network::update()
{

    uint32_t now =
        millis();



    if(
        WiFi.status()
        !=
        WL_CONNECTED
    )
    {

        m_status.connected =
            false;


        m_status.state =
            NetworkState::Disconnected;



        if(
            now - m_lastReconnect
            >=
            Constants::Timing::NetworkReconnect
        )
        {

            m_lastReconnect =
                now;


            connect();

        }


        return;

    }



    /*
     * Connected
     */

    m_status.connected =
        true;


    m_status.state =
        NetworkState::Connected;



    if(
        !m_timeSyncStarted
    )
    {

        syncTime();


        m_timeSyncStarted =
            true;

    }



    if(
        now - m_lastStatusUpdate
        >=
        Constants::Timing::NetworkStatusUpdate
    )
    {

        m_lastStatusUpdate =
            now;


        refreshStatus();

    }

}



/******************************************************************************
 * NTP Synchronization
 ******************************************************************************/

void Network::syncTime()
{

    m_status.state =
        NetworkState::NtpSync;



    configTime(

        Config::Time::TimezoneOffset,

        Config::Time::DaylightOffset,

        Config::Time::NtpServer

    );



    struct tm timeInfo;



    if(
        getLocalTime(
            &timeInfo,
            5000
        )
    )
    {
        m_status.timeSynced =
            true;
        m_status.state =
            NetworkState::Connected;
    }
    else
    {
        m_status.timeSynced =
            false;
    }
    }



}



/******************************************************************************
 * Update Network Information
 ******************************************************************************/

void Network::refreshStatus()
{

    if(
        WiFi.status()
        ==
        WL_CONNECTED
    )
    {

        m_status.ip =
            WiFi.localIP();


        m_status.rssi =
            WiFi.RSSI();

    }

}



/******************************************************************************
 * Manual Reconnect
 ******************************************************************************/

void Network::reconnect()
{

    WiFi.disconnect();


    m_status.timeSynced =
        false;


    m_lastSyncAttempt =
        0;


    connect();

}



/******************************************************************************
 * Status
 ******************************************************************************/

NetworkStatus Network::status() const
{
    return m_status;
}



bool Network::isConnected() const
{
    return m_status.connected;
}
