#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include "Arduino.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Geiger counter</title>
    <link rel="stylesheet" type="text/css" href="css/main.css">
    <script type="text/javascript" src="js/main.js"></script>
</head>

<body class="d-flex">
    <main>
        <div class="d-flex">
            <h1 class="flex-grow-1">Geiger counter</h1>
            <span class="float-right" id="apiError">
                <img src="media/image/wifi_off_black_24dp.svg" style="width: 2rem;"/>
            </span>
        </div>
        <table>
            <thead>
                <tr>
                    <th></th>
                    <th>Current</th>
                    <th>Max</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td title="Total count of disintegrations">&sum;n:</td>
                    <td><span id="totalcount">0</span></td>
                    <td>
                        <input type="button" value="Reset" id="totalcount_delete"/>
                    </td>
                </tr>
                <tr>
                    <td title="Frequency of disintegrations">f:</td>
                    <td><span id="frequency">0</span> min<sup>-1</sup></td>                    
                    <td><span id="frequency_max">0</span> min<sup>-1</sup></td>
                </tr>
                <tr>
                    <td title="Radiation dose">H:</td>
                    <td><span id="dose">0</span> µSv/h</td>
                    <td><span id="dose_max">0</span> µSv/h</td>
                </tr>
            </tbody>
        </table>
    </main>
</body>

</html>
)rawliteral";

#endif /* INDEX_HTML_H */
