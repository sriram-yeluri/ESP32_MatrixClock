#ifndef MESSAGE_WEB_SERVER_H
#define MESSAGE_WEB_SERVER_H

#include <Arduino.h>
#include <WebServer.h>
#include "MessageManager.h"

class MessageWebServer
{
public:
    explicit MessageWebServer(MessageManager& messages);

    void begin();
    void update();

private:
    void handleRoot();
    void handleAdd();
    void handleDelete();
    void handleClear();
    void handleUpdate();
    void handlePin();
    void sendHtml(const String& body);
    String buildPage() const;
    String escapeHtml(const String& value) const;
    void redirectToRoot();

    MessageManager& m_messages;
    WebServer m_server;
    bool m_started;
};

#endif
