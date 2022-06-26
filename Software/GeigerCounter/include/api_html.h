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
    <link rel="stylesheet" type="text/css" href="css/swagger-ui.css">
    <script type="text/javascript" src="js/swagger-ui-bundle.js"></script>
</head>

<body class="d-flex justify-content-center">
    <main>
        <h1>Geiger counter API reference</h1>
        <div class="api-reference">
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/totalcount</strong>
                    Returns the total count of disintegrations measured.
                </span>
            </div>
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/frequency</strong>
                    Returns the count of disintegrations per minute.
                </span>
            </div>
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/frequency_max</strong>
                    Returns the maximum count of disintegrations per minute.
                </span>
            </div>
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/dose</strong>
                    Returns the radiation dose in µSv/h.
                </span>
            </div>
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/dose_max</strong>
                    Returns the maximum measured radiation dose in µSv/h.
                </span>
            </div>
            <div class="api-item api-item-get">
                <span class="api-method">GET</span>
                <span class="api-description">
                    <strong>/api/uptime</strong>
                    Returns the uptime of the geiger counter in milliseconds.
                </span>
            </div>
            <div class="api-item api-item-post">
                <span class="api-method">POST</span>
                <span class="api-description">
                    <strong>/api/reset_max</strong>
                    Resets the maximum values to 0.
                </span>
            </div>
        </div>
    </main>
</body>

</html>
)rawliteral";

#endif /* API_HTML_H */