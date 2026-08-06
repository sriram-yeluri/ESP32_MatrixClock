/******************************************************************************
 * Project      : ESP32 Matrix Clock Professional
 *
 * File         : ClockNetwork.cpp
 ******************************************************************************/

#include "ClockNetwork.h"
#include <time.h>

ClockNetwork::ClockNetwork(Settings& settings)
    : m_settings(settings),
      m_lastReconnect(0),
      m_lastStatusUpdate(0),
      m_lastSyncAttempt(0),
      m_webServer(80),
      m_apMode(false)
{
    m_status.state = NetworkState::Disconnected;
    m_status.connected = false;
    m_status.timeSynced = false;
    m_status.rssi = 0;
}

bool ClockNetwork::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(m_settings.hostname());

    connect();

    return true;
}

void ClockNetwork::connect()
{
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(m_settings.wifiSSID());

    m_status.state = NetworkState::Connecting;

    WiFi.begin(
        m_settings.wifiSSID(),
        m_settings.wifiPassword()
    );

    unsigned long start = millis();
    while (millis() - start < 8000)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return;
        }
        delay(100);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi connection failed, starting access point...");
        startAccessPoint();
    }
}

void ClockNetwork::startAccessPoint()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32-MatrixClock", "matrixclock");

    IPAddress apIP = WiFi.softAPIP();
    m_status.ip = apIP;
    m_status.connected = true;
    m_status.state = NetworkState::Connected;
    m_apMode = true;

    m_webServer.on("/", HTTP_GET, [this]() { handleRoot(); });
    m_webServer.begin();

    Serial.println("Access Point ready.");
    Serial.print("AP IP: ");
    Serial.println(apIP);
}

void ClockNetwork::handleRoot()
{
    String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>ESP32 Matrix Clock</title><style>body{font-family:Arial,sans-serif;background:#0f172a;color:#f8fafc;margin:0;padding:20px;line-height:1.5}div{background:#111827;padding:16px;border-radius:12px;max-width:480px;margin:auto}h2{margin-top:0}p{color:#cbd5e1}a{color:#38bdf8}</style></head><body><div><h2>ESP32 Matrix Clock</h2><p>Your clock is running in setup mode.</p><p>Connect this device to your Wi-Fi network, then open the message editor page at the device IP address.</p><p>Default access point: <b>ESP32-MatrixClock</b></p><p>Password: <b>matrixclock</b></p></div></body></html>";
    m_webServer.send(200, "text/html", html);
}

void ClockNetwork::update()
{
    if (m_apMode)
    {
        m_webServer.handleClient();
    }

    uint32_t now = millis();

    if (WiFi.status() != WL_CONNECTED)
    {
        m_status.connected = false;
        m_status.timeSynced = false;
        m_status.state = NetworkState::Disconnected;

        if (now - m_lastReconnect >= Constants::Timing::NetworkReconnect)
        {
            m_lastReconnect = now;
            connect();
        }

        return;
    }

    if (!m_status.connected)
    {
        m_status.connected = true;
        m_status.state = NetworkState::Connected;
        refreshStatus();

        Serial.println("WiFi connected.");
        Serial.print("IP address: ");
        Serial.println(m_status.ip.toString());
    }

    if (!m_status.timeSynced)
    {
        if (now - m_lastSyncAttempt >= 5000)
        {
            m_lastSyncAttempt = now;
            syncTime();
        }
    }

    if (now - m_lastStatusUpdate >= Constants::Timing::NetworkStatusUpdate)
    {
        m_lastStatusUpdate = now;
        refreshStatus();
    }
}

void ClockNetwork::syncTime()
{
    m_status.state = NetworkState::NtpSync;

    configTime(
        Config::Time::TimezoneOffset,
        Config::Time::DaylightOffset,
        Config::Time::NtpServer
    );

    struct tm timeInfo;

    if (getLocalTime(&timeInfo, 5000))
    {
        m_status.timeSynced = true;
        m_status.state = NetworkState::Connected;

        Serial.println("NTP synchronized.");
    }
    else
    {
        m_status.timeSynced = false;

        Serial.println("NTP synchronization failed.");
    }
}

void ClockNetwork::refreshStatus()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        m_status.ip = WiFi.localIP();
        m_status.rssi = WiFi.RSSI();
    }
}

void ClockNetwork::reconnect()
{
    WiFi.disconnect(true);

    m_status.connected = false;
    m_status.timeSynced = false;

    m_lastReconnect = 0;
    m_lastSyncAttempt = 0;

    connect();
}

NetworkStatus ClockNetwork::status() const
{
    return m_status;
}

bool ClockNetwork::isConnected() const
{
    return m_status.connected;
}