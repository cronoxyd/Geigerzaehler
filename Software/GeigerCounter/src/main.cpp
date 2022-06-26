/*************************************************************************
 * Programm: geigerzaehler.ino                                                *
 * Author:   Heinz Behling *                                          *
 * Datum:    8.5.2022                                                *
 *************************************************************************/
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP_Mail_Client.h>
#include <DNSServer.h>
#include <dhcpserver.h>
#include "Arduino.h"
#include "config.h"
#include "custom_characters.h"
#include "CaptiveRequestHandler.hpp"
#include "staticcontent.h"

LiquidCrystal_I2C lcd(0x20, DISP_COLUMNS, DISP_ROWS); // Address, Characters per line, Line count
SMTPSession smtp;
AsyncWebServer server(80);
DNSServer dnsServer;

char lcdBuffer[DISP_ROWS][DISP_COLUMNS];

static unsigned long impulseCountTotal = 0;
static unsigned long impulseCountInterval = 0;

static unsigned int cpm;
static unsigned int cpmMax;
static double radiationDose = 0;    // µSv
static double radiationDoseMax = 0; // µSv

IPAddress laddr;
String newHostname = "GeigerCounter";

// Interrupt-Routine wenn Impuls eintrifft
ICACHE_RAM_ATTR void tube_impulse()
{ // ICACHE_RAM_ATRR schreibt Routine ins RAM
    impulseCountInterval++;
    impulseCountTotal++;
}

String templateProcessor(const String &var)
{
    if (var == "laddr")
        return laddr.toString();

    return "";
}

void setup()
{
    for (int y = 0; y < DISP_ROWS; y++)
    {
        for (int x = 0; x < DISP_COLUMNS; x++)
        {
            lcdBuffer[y][x] = 0x20;
        }
    }

    lcd.init();
    lcd.backlight();
    lcd.createChar(0, alarmBell);
    lcd.createChar(1, radioWaves);
    lcd.clear();
    lcd.print("Starting...");

    Serial.begin(115200);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PSK);
    delay(100);
    laddr = WiFi.softAPIP();
    // IPAddress subnet;
    // subnet.fromString("0.0.0.0");
    // WiFi.softAPConfig(laddr, laddr, subnet);
    dnsServer.start(53, "*", laddr);

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->redirect("http://" + laddr.toString()); });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });

    server.on("/api/v3/openapi.json", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/css", openapi_json); });

    server.on("/css/main.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/css", main_css); });

    server.on("/media/image/wifi_off_black_24dp.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "image/svg+xml", wifi_off_black_24dp_svg); });

    server.on("/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/javascript", main_js, templateProcessor); });

    server.on("/api/totalcount", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(impulseCountTotal).c_str()); });

    server.on("/api/reset_max", HTTP_POST, [](AsyncWebServerRequest *request)
              { impulseCountTotal = 0;
                radiationDoseMax = radiationDose;
                cpmMax = cpm;
                request->send_P(200, "text/plain", ""); });

    server.on("/api/frequency", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(cpm).c_str()); });

    server.on("/api/frequency_max", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(cpmMax).c_str()); });

    server.on("/api/dose", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(radiationDose, 4).c_str()); });

    server.on("/api/dose_max", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(radiationDoseMax, 4).c_str()); });

    server.on("/api/uptime", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", String(millis()).c_str()); });

    server.on("/api", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", api_html); });

    server.addHandler(new CaptiveRequestHandler("http://" + laddr.toString())).setFilter(ON_AP_FILTER);

    server.begin();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SSID: ");
    lcd.print(WIFI_AP_SSID);
    // IPAddress subnet;
    // subnet.fromString("255.255.255.0");
    // WiFi.softAPConfig(laddr, laddr, subnet);

    lcd.setCursor(0, 1);
    lcd.print("IP: ");
    lcd.print(laddr);

    delay(2500);

    pinMode(COUNTER_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(COUNTER_PIN), tube_impulse, FALLING);
}

double getRadiationDose(int CPM)
{
    return CPM * TUBE_COEFFICIENT;
}

/*
 * Fills the line of the LCD with spaces.
 * @param writtenLength The value returned by sprintf().
 */
void padLCDLine(char *line, int writtenLength)
{
    for (int x = writtenLength - 1; x < DISP_COLUMNS; x++)
        line[x] = 0x20;
}

void updateLCD()
{
    for (int y = 0; y < DISP_ROWS; y++)
    {
        lcd.setCursor(0, y);
        for (int x = 0; x < DISP_COLUMNS; x++)
        {
            lcd.write(lcdBuffer[y][x]);
        }
    }
}

void loop()
{
    static unsigned long measureStart;
    unsigned long now = millis();
    static unsigned long updateTimer;
    bool hasAlarm = cpm > ALARM_THRESHOLD;

    dnsServer.processNextRequest();

    if (now - measureStart > MEASUREMENT_INTERVAL_MS)
    {
        measureStart = now;

        cpm = impulseCountInterval * CPM_COEFFICIENT;
        cpmMax = max(cpm, cpmMax);
        radiationDose = getRadiationDose(cpm);
        radiationDoseMax = max(radiationDose, radiationDoseMax);

        impulseCountInterval = 0;
    }

    if (now - updateTimer > 100 || updateTimer == 0)
    {

        padLCDLine(lcdBuffer[0], sprintf(lcdBuffer[0], "%cn:   %lu ", 0b11110110, impulseCountTotal));

        if (hasAlarm)
            lcdBuffer[0][19] = 0;

        if (WiFi.softAPgetStationNum())
            lcdBuffer[0][18] = 1;

        padLCDLine(lcdBuffer[1], sprintf(lcdBuffer[1], "f:    %u min%c ", cpm, 0b11101001));
        padLCDLine(lcdBuffer[2], sprintf(lcdBuffer[2], "H:    %.4f %cSv/h ", radiationDose, 0b11100100));
        padLCDLine(lcdBuffer[3], sprintf(lcdBuffer[3], "Hmax: %.4f %cSv/h ", radiationDoseMax, 0b11100100));

        updateLCD();
        updateTimer = now;
    }
}
