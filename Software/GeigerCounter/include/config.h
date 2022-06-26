#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

// Tube configuration
#define MEASUREMENT_INTERVAL_MS 15000
#define CPM_COEFFICIENT 60000 / MEASUREMENT_INTERVAL_MS
#define COUNTER_PIN D7
#define TUBE_COEFFICIENT 0.00662251655629 // Conversion factor for M4011 Geiger-Müller tube

// Email settings
#define SMTP_SERVER "IHR_SMTP_SERVER"
#define SMTP_PORT 485
#define SENDER_MAIL_ADDRESS "IHRE_EMAIL_ADRESSE"
#define SENDER_PASSWORD "SMTP_PASSWORT"
#define RECIPIENT_MAIL_ADDRESS "EMPFÄNGERADRESSE"

// WiFi settings
#define WIFI_AP_SSID "Geiger counter"
#define WIFI_AP_PSK NULL

// Display configuration
#define DISP_COLUMNS 20
#define DISP_ROWS 4

// Monitoring settings
#define ALARM_THRESHOLD 100

#endif /* CONFIG_H */