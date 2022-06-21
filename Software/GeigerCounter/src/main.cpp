/*************************************************************************
 * Programm: geigerzaehler.ino                                                *
 * Author:   Heinz Behling *                                          *
 * Datum:    8.5.2022                                                *
 *************************************************************************/
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
#include <ESP_Mail_Client.h>
#include "Arduino.h"
#include "config.h"

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x20, 20, 4); // Address, Characters per line, Line count
SMTPSession smtp;

bool notsend = true;
int mailcount = 0;

unsigned long impulseCountTotal = 0;
unsigned long impulseCountInterval = 0;
double cpm;
unsigned long impulseonline;
unsigned long cpmonline;
unsigned int mult; // CPM = (counts in a given interval) * multiplier
double radiationDoseMax = 0;
bool initialized = false;

String newHostname = "GeigerCounter";

// Interrupt-Routine wenn Impuls eintrifft
ICACHE_RAM_ATTR void tube_impulse()
{ // ICACHE_RAM_ATRR schreibt Routine ins RAM
    impulseCountInterval++;
    impulseCountTotal++;

    if (initialized)
    {
        Serial.print("+");

        lcd.setCursor(0, 0);
        lcd.print((char)0b11110110); // Σ
        lcd.print("n:   ");
        lcd.print(impulseCountTotal);
    }
}

void setup()
{
    Serial.begin(115200);
    if (!ssid.isEmpty())
    {
        WiFi.mode(WIFI_STA);
        WiFi.hostname(newHostname.c_str());
    }
    delay(1000);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Make: Geiger counter");
    Serial.println("=========================================");
    Serial.println("Make: Geiger counter");
    Serial.println("=========================================");
    Serial.println();

    if (!ssid.isEmpty())
    {
        Serial.print("Connecting to ");
        Serial.println(ssid);
        lcd.setCursor(0, 1);
        lcd.print("connecting to ");
        lcd.setCursor(0, 2);

        lcd.print(ssid);

        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(50);
            Serial.print(".");
        }
        Serial.println("");
        Serial.println("connected");
        lcd.setCursor(0, 3);
        lcd.print("connected");
        server.begin();
    }

    lcd.init();
    lcd.setCursor(0, 0);
    lcd.print("Make: Geiger counter");
    // lcd.backlight();
    impulseCountInterval = 0;
    cpm = 0;
    mult = 60000 / MEASUREMENT_INTERVAL_MS;
    pinMode(COUNTER_PIN, INPUT);

    if (!ssid.isEmpty())
    {
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        lcd.setCursor(0, 1);
        lcd.print("IP address:");
        lcd.setCursor(0, 2);
        lcd.print(WiFi.localIP());
    }
    Serial.print("Please wait ");
    Serial.print(MEASUREMENT_INTERVAL_MS / 1000);
    Serial.println(" sec until first measurement");
    Serial.println();

    attachInterrupt(digitalPinToInterrupt(COUNTER_PIN), tube_impulse, FALLING);
}

double getRadiationDose(int CPM)
{
    return cpm * TUBE_COEFFICIENT;
}

void loop()
{
    static unsigned long measureStart;
    unsigned long now = millis();
    double radiationDose = 0; // µSv

    // Immer wenn MESSZEITvergangen ist, Werte ausgeben
    if (now - measureStart > MEASUREMENT_INTERVAL_MS)
    {
        initialized = true;
        measureStart = now;
        if (impulseCountInterval)
        {
            cpm = impulseCountInterval * mult;
            radiationDose = getRadiationDose(cpm);
            radiationDoseMax = max(radiationDose, radiationDoseMax);
            impulseonline = impulseCountInterval;
            cpmonline = cpm;
        }

        lcd.init();

        Serial.println("");
        Serial.print((char)0b11110110); // Σ
        Serial.print("n:   ");
        Serial.println(impulseCountTotal);

        Serial.print("f:    ");
        Serial.println(cpm);
        lcd.setCursor(0, 1);
        lcd.print("f:    ");
        lcd.print(cpm);
        lcd.print(" min");
        lcd.print((char)0b11101001); // ⁻¹

        Serial.print("H:    ");
        Serial.print(radiationDose, 4); // mit 4 Nachkommastellen
        Serial.println(" µSv/h");
        lcd.setCursor(0, 2);
        lcd.print("H:    ");
        lcd.print(radiationDose, 4);
        lcd.print((char)0b00100000); // SPACE
        lcd.print((char)0b11100100); // µ
        lcd.print("Sv/h");

        Serial.print("Hmax: ");
        Serial.print(radiationDoseMax, 4); // mit 4 Nachkommastellen
        Serial.println("µSv/h");
        lcd.setCursor(0, 3);
        lcd.print("Hmax: ");
        lcd.print(radiationDoseMax, 4);
        lcd.print((char)0b00100000); // SPACE
        lcd.print((char)0b11100100); // µ
        lcd.print("Sv/h");

        if (cpm > ALARM_THRESHOLD)
        {
            Serial.println("Alarm: Threshold exceeded");
            lcd.init();
            lcd.setCursor(0, 0);
            lcd.print("Make: Geigerzaehler");
            lcd.setCursor(0, 1);
            lcd.print("Impulse: ");
            lcd.print(impulseCountInterval);
            lcd.setCursor(0, 2);
            lcd.print("   CPM: ");
            lcd.print(cpm);
            lcd.setCursor(0, 3);
            lcd.println("      Alarm!      ");
        }
        Serial.println("");
        impulseCountInterval = 0, cpm = 0;
    }

    if (ssid.isEmpty())
        return;

    WiFiClient client = server.available();
    client.print("<head><title>Make:-Geigerz&auml;hler</title><meta http-equiv='refresh' content='5' /></head>");
    client.print("<h1>Make:-Geigerz&auml;hler </h1><br>");
    client.print("<table>");
    client.print("<tr><td><b>Impulse:</b> </td><td>");
    client.print(impulseonline);
    client.print("<br></td></tr>");
    client.print("<tr><td><b>Impulse/min:</b> </td><td>");
    client.print(impulseonline * mult);
    client.print("<br></td></tr>");
    client.print("<tr><td><b>uSv/h:</b> </td><td>");
    client.print(radiationDose);
    client.print("<br></td></tr>");
    client.print("<tr><td><b>Schwellwert:</b> </td><td>");
    client.print(ALARM_THRESHOLD);
    client.print("<br></td></tr>");
    if (cpmonline > ALARM_THRESHOLD)
    {
        client.print("<tr><td><b><big><big><big>ALARM!</big></big></big></b> </td><td>");
        client.print("Schwellwert ueberschritten!");
        client.print("<br></td></tr>");
    }
    client.print("</table>");

    delay(100);

    if (cpmonline > ALARM_THRESHOLD)
    {
        mailcount = mailcount - 1;
        Serial.println(mailcount);
        if (mailcount == 0)
        {
            notsend = true;
        }
        if (notsend == true)
        {
            notsend = false;
            mailcount = 1200;
            smtp.debug(1);
            ESP_Mail_Session session;
            session.server.host_name = SMTP_SERVER;
            session.server.port = SMTP_PORT;
            session.login.email = SENDER_MAIL_ADDRESS;
            session.login.password = SENDER_PASSWORD;
            session.login.user_domain = "";
            SMTP_Message message;
            message.sender.name = "Geigerzaehler";
            message.sender.email = SENDER_MAIL_ADDRESS;
            message.subject = "Strahlungsalarm";
            message.addRecipient("Heinz", RECIPIENT_MAIL_ADDRESS);
            // Send HTML message
            String htmlMsg = "<div style=\"color:#FF0000;\"><h1>ALARM!</h1><p>Der Strahlungsgrenzwert wurde überschritten!</p></div>";
            message.html.content = htmlMsg.c_str();
            message.html.content = htmlMsg.c_str();
            message.text.charSet = "us-ascii";
            message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
            if (!smtp.connect(&session))
                return;
            if (!MailClient.sendMail(&smtp, &message))
                Serial.println("Error sending Email, " + smtp.errorReason());
            Serial.println("Mail gesendet");
        }
    }
}
