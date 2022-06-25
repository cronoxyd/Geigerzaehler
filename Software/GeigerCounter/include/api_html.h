#ifndef API_HTML_H
#define API_HTML_H

#include "Arduino.h"

const char api_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Geiger counter API</title>
    <link rel="stylesheet" type="text/css" href="css/main.css">
</head>

<body>
    <main>
        <h1>Geiger counter API</h1>
        <h2>/api/totalcount</h2>
        Returns the total count of the disintegrations measured.
    
        <h2>/api/frequency</h2>
        Returns the count of disintegrations per minute.
    
        <h2>/api/frequency_max</h2>
        Returns the maximum measured count of disintegrations per minute (non-persistent).
    
        <h2>/api/dose</h2>
        Returns the radiation dose in µSv/h.
    
        <h2>/api/dose_max</h2>
        Returns the maximum measured radiation dose in µSv/h (non-persistent).
    
        <h2>/api/uptime</h2>
        Returns the uptime of the geiger counter in milliseconds.
    </main>
</body>

</html>
)rawliteral";

#endif /* API_HTML_H */