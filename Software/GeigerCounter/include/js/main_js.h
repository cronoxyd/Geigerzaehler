#ifndef MAIN_JS_H
#define MAIN_JS_H

#include "Arduino.h"

const char main_js[] PROGMEM = R"rawliteral(
var apiError = false;

const updateAPIError = function(enabled) {
    document.querySelector("#apiError").style.visibility = (enabled ? "visible" : "hidden");
    apiError = true;
}

const updateField = async function (url, id) {
    try {
        const response = await fetch(url, {
            method: 'GET',
            cache: 'no-cache'
        });
    
        const responseText = await response.text();
    
        document.getElementById(id).innerHTML = responseText;
        updateAPIError(false);
        return responseText;
    } catch {
        updateAPIError(true);
        return null;
    }
}

const updateVariables = async function () {
    await updateField("http://%laddr%/api/totalcount", "totalcount");
    var frequency = await updateField("http://%laddr%/api/frequency", "frequency");
    await updateField("http://%laddr%/api/frequency_max", "frequency_max");
    await updateField("http://%laddr%/api/dose", "dose");
    await updateField("http://%laddr%/api/dose_max", "dose_max");

    if (frequency > 100)
        document.querySelector("body").classList.add("danger");
    else
        document.querySelector("body").classList.remove("danger");


    window.setTimeout(updateVariables, 250);
}


window.onload = () => {
    window.setTimeout(updateVariables, 250);

    document.querySelector("#totalcount_delete").addEventListener("click", async () => {
        await fetch("http://%laddr%/api/reset_max", {
            method: 'POST',
            cache: 'no-cache'
        });
    });
};
)rawliteral";

#endif /* MAIN_JS_H */