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
#include "custom_characters.h"

WiFiServer server(80);
LiquidCrystal_I2C lcd(0x20, DISP_COLUMNS, DISP_ROWS); // Address, Characters per line, Line count
SMTPSession smtp;

char lcdBuffer[DISP_ROWS][DISP_COLUMNS];

bool notsend = true;
int mailcount = 0;

unsigned long impulseCountTotal = 0;
unsigned long impulseCountInterval = 0;
unsigned long impulseonline;
unsigned long cpmonline;

String newHostname = "GeigerCounter";

// Interrupt-Routine wenn Impuls eintrifft
ICACHE_RAM_ATTR void tube_impulse()
{ // ICACHE_RAM_ATRR schreibt Routine ins RAM
    impulseCountInterval++;
    impulseCountTotal++;
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

    Serial.begin(115200);
    if (!ssid.isEmpty())
    {
        WiFi.mode(WIFI_STA);
        WiFi.hostname(newHostname.c_str());
    }
    delay(1000);
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, alarmBell);

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

void loop()
{
    static unsigned long measureStart;
    unsigned long now = millis();
    static unsigned int cpm;
    static double radiationDose = 0; // µSv
    static double radiationDoseMax = 0;
    bool hasAlarm = cpm > ALARM_THRESHOLD;

    if (now - measureStart > MEASUREMENT_INTERVAL_MS)
    {
        measureStart = now;

        cpm = impulseCountInterval * CPM_COEFFICIENT;
        radiationDose = getRadiationDose(cpm);
        radiationDoseMax = max(radiationDose, radiationDoseMax);
        impulseonline = impulseCountInterval;
        cpmonline = cpm;

        impulseCountInterval = 0;
    }

    padLCDLine(lcdBuffer[0], sprintf(lcdBuffer[0], "%cn:   %lu ", 0b11110110, impulseCountTotal));
    if (hasAlarm)
    {
        lcdBuffer[0][19] = 0;
    }

    padLCDLine(lcdBuffer[1], sprintf(lcdBuffer[1], "f:    %u min%c ", cpm, 0b11101001));
    padLCDLine(lcdBuffer[2], sprintf(lcdBuffer[2], "H:    %.4f %cSv/h ", radiationDose, 0b11100100));
    padLCDLine(lcdBuffer[3], sprintf(lcdBuffer[3], "Hmax: %.4f %cSv/h ", radiationDoseMax, 0b11100100));

    for (int y = 0; y < DISP_ROWS; y++)
    {
        lcd.setCursor(0, y);
        for (int x = 0; x < DISP_COLUMNS; x++)
        {
            lcd.write(lcdBuffer[y][x]);
        }
        // Serial.println(lcdBuffer[y]); // TODO: delay this
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
    client.print(impulseonline * CPM_COEFFICIENT);
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
