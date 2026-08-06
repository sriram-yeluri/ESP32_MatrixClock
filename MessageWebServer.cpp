#include "MessageWebServer.h"

MessageWebServer::MessageWebServer(MessageManager& messages)
    : m_messages(messages), m_server(80), m_started(false)
{
}

void MessageWebServer::begin()
{
    if (m_started)
    {
        return;
    }

    m_server.on("/", HTTP_GET, [this]() { handleRoot(); });
    m_server.on("/add", HTTP_POST, [this]() { handleAdd(); });
    m_server.on("/delete", HTTP_POST, [this]() { handleDelete(); });
    m_server.on("/clear", HTTP_POST, [this]() { handleClear(); });
    m_server.on("/update", HTTP_POST, [this]() { handleUpdate(); });
    m_server.on("/pin", HTTP_POST, [this]() { handlePin(); });
    m_server.begin();
    m_started = true;
}

void MessageWebServer::update()
{
    if (m_started)
    {
        m_server.handleClient();
    }
}

void MessageWebServer::handleRoot()
{
    sendHtml(buildPage());
}

void MessageWebServer::handleAdd()
{
    if (m_server.hasArg("message") && m_server.arg("message").length() > 0)
    {
        m_messages.add(m_server.arg("message").c_str(), 5);
    }

    redirectToRoot();
}

void MessageWebServer::handleDelete()
{
    if (m_server.hasArg("index"))
    {
        uint8_t index = m_server.arg("index").toInt();
        m_messages.remove(index);
    }

    redirectToRoot();
}

void MessageWebServer::handleClear()
{
    m_messages.clear();
    redirectToRoot();
}

void MessageWebServer::handleUpdate()
{
    if (m_server.hasArg("index") && m_server.hasArg("message") && m_server.hasArg("seconds"))
    {
        uint8_t index = m_server.arg("index").toInt();
        uint16_t seconds = m_server.arg("seconds").toInt();
        bool priority = m_server.hasArg("priority") && m_server.arg("priority") == "1";
        m_messages.updateAt(index, m_server.arg("message").c_str(), seconds, priority);
    }

    redirectToRoot();
}

void MessageWebServer::handlePin()
{
    if (m_server.hasArg("index") && m_server.hasArg("priority"))
    {
        uint8_t index = m_server.arg("index").toInt();
        bool priority = m_server.arg("priority") == "1";
        m_messages.pinAt(index, priority);
    }

    redirectToRoot();
}

void MessageWebServer::sendHtml(const String& body)
{
    m_server.send(200, "text/html", body);
}

String MessageWebServer::buildPage() const
{
    String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>Matrix Clock Messages</title><style>body{font-family:Arial,sans-serif;margin:0;padding:16px;background:#0f172a;color:#f8fafc;line-height:1.4}*{box-sizing:border-box}h2{margin:0 0 8px 0}p{margin:0 0 12px 0;color:#cbd5e1}.card{background:#111827;border:1px solid #334155;border-radius:14px;padding:12px;margin-bottom:12px}.row{display:flex;gap:8px;flex-wrap:wrap}.row > *{flex:1 1 140px}input,button,select{padding:10px;border-radius:10px;border:1px solid #475569;width:100%;margin-top:8px;font-size:16px}input[type=checkbox]{width:auto;margin:0;padding:0;accent-color:#22c55e}.checkbox-row{display:flex;align-items:center;gap:8px;margin-top:8px;color:#e2e8f0;font-size:15px;cursor:pointer}button{background:#22c55e;color:white;font-weight:bold;cursor:pointer}button.secondary{background:#475569}table{width:100%;border-collapse:collapse;margin-top:10px}th,td{padding:10px;border-bottom:1px solid #334155;text-align:left;vertical-align:top}td form{margin:0}small{color:#94a3b8}@media(max-width:600px){body{padding:12px}.card{padding:10px}}</style></head><body>";
    html += "<div class='card'><h2>Matrix Clock Message Manager</h2><p>Manage the messages shown on your display.</p><form action='/add' method='post'><div class='row'><input name='message' maxlength='127' placeholder='New message'></div><div class='row'><input name='seconds' type='number' min='1' max='30' value='5' placeholder='Seconds'></div><button type='submit'>Add Message</button></form></div>";
    html += "<div class='card'><form action='/clear' method='post'><button class='secondary' type='submit'>Clear All</button></form></div>";
    html += "<div class='card'><h3>Current Messages</h3>";
    if (m_messages.count() == 0)
    {
        html += "<small>No messages yet.</small>";
    }
    else
    {
        html += "<table><tr><th>#</th><th>Message</th><th>Duration</th><th>Action</th></tr>";
        for (uint8_t i = 0; i < m_messages.count(); ++i)
        {
            String row = "<tr><td>";
            row += String(i + 1);
            row += "</td><td>";
            row += escapeHtml(m_messages.get(i));
            row += "</td><td>5s</td><td><form action='/delete' method='post'><input type='hidden' name='index' value='";
            row += String(i);
            row += "'><button class='secondary' type='submit'>Delete</button></form><form action='/update' method='post' style='margin-top:6px'><input type='hidden' name='index' value='";
            row += String(i);
            row += "'><input type='text' name='message' maxlength='127' value='";
            row += escapeHtml(m_messages.get(i));
            row += "'><input type='number' name='seconds' min='1' max='30' value='5'><label class='checkbox-row'><input type='checkbox' name='priority' value='1'> Pin to top</label><button type='submit'>Update</button></form></td></tr>";
            html += row;
        }
        html += "</table>";
    }

    html += "</div></body></html>";
    return html;
}

String MessageWebServer::escapeHtml(const String& value) const
{
    String out = value;
    out.replace("&", "&amp;");
    out.replace("<", "&lt;");
    out.replace(">", "&gt;");
    out.replace("\"", "&quot;");
    return out;
}

void MessageWebServer::redirectToRoot()
{
    m_server.sendHeader("Location", "/");
    m_server.send(303);
}
