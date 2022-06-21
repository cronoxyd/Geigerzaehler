#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

// Tube configuration
#define MEASUREMENT_INTERVAL_MS 15000
#define COUNTER_PIN D7
#define TUBE_COEFFICIENT 0.0067 // Umrechnungsfaktor für M4011-Zählrohre

// Email settings
#define SMTP_SERVER "IHR_SMTP_SERVER"
#define SMTP_PORT 485
#define SENDER_MAIL_ADDRESS "IHRE_EMAIL_ADRESSE"
#define SENDER_PASSWORD "SMTP_PASSWORT"
#define RECIPIENT_MAIL_ADDRESS "EMPFÄNGERADRESSE"

// WiFi settings
const String ssid = "";
const String password = "";

// Monitoring settings
#define ALARM_THRESHOLD 100

#endif /* CONFIG_H */