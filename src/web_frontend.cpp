#ifndef _WEB_FRONTEND_H
#include "web_frontend.h"
#define _WEB_FRONTEND_H_
#endif

void handle_wifi(WiFiClient client) {
    if (client) {                  
        String header;           // Bei einem Aufruf des Servers
        Serial.println("Client available");
        String currentLine = "";                // String definieren für die Anfrage des Clients

            while (client.connected()) { // Loop, solange Client verbunden ist

                if (client.available()) {
                    char c = client.read();             // Ein (1) Zeichen der Anfrage des Clients lesen
                    Serial.write(c);                    // und es im Seriellen Monitor ausgeben
                    header += c;
                    if (c == '\n') {                    // bis eine Neue Zeile ausgegeben wird

                        // Wenn der Client eine Leerzeile sendet, ist das Ende des HTTP Request erreicht
                        if (currentLine.length() == 0) {
                            // Der Server sendet nun eine Antwort an den Client
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            // Die Webseite anzeigen
                            client.println("<!DOCTYPE html><html>");
                            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                            client.println("<link rel=\"icon\" href=\"data:,\"></head>");
                            client.println("<body><h1 align=\"center\">Temperaturen</h1></body></html>");
                            client.println("<br>");

                            // Den Loop beenden
                            break;
                        } else { // Bei einer Neuen Zeile, die Variable leeren
                            currentLine = "";
                        }
                    } else if (c != '\r') {  // alles andere als eine Leerzeile wird
                    currentLine += c;      // der Variable hinzugefüht
                }
            }
        }
        // Variable für den Header leeren
        header = "";
        // Die Verbindung beenden
        client.stop();
        Serial.println("Client disconnected");
    }
}