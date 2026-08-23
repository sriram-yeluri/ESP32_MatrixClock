#ifndef PLATFORM_H
#define PLATFORM_H

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
using MatrixClockWebServer = ESP8266WebServer;
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
using MatrixClockWebServer = WebServer;
#else
#error "ESP32_MatrixClock supports ESP32 and ESP8266 targets only."
#endif

#endif