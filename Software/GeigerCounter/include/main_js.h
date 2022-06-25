#ifndef MAIN_JS_H
#define MAIN_JS_H

#include "Arduino.h"

const char main_js[] PROGMEM = R"rawliteral(
const updateField = async function (url, id) {
    try {
        const response = await fetch(url, {
            method: 'GET',
            cache: 'no-cache'
        });
    
        const responseText = await response.text();
    
        document.getElementById(id).innerHTML = responseText;
        return responseText;
    } catch {
        return null;
    }
}

const updateVariables = async function () {
    await updateField("http://192.168.4.1/api/totalcount", "totalcount");
    var frequency = await updateField("http://192.168.4.1/api/frequency", "frequency");
    await updateField("http://192.168.4.1/api/frequency_max", "frequency_max");
    await updateField("http://192.168.4.1/api/dose", "dose");
    await updateField("http://192.168.4.1/api/dose_max", "dose_max");

    if (frequency > 100)
        document.querySelector("body").classList.add("danger");
    else
        document.querySelector("body").classList.remove("danger");

    window.setTimeout(updateVariables, 250);
}

window.setTimeout(updateVariables, 250);
)rawliteral";

#endif /* MAIN_JS_H */