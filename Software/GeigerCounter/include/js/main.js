var apiError = false;

function updateAPIError(enabled) {
    document.querySelector("#apiError").style.visibility = (enabled ? "visible" : "hidden");
    apiError = true;
}

function toSI(d, unit = "")
{
    incPrefixes = [ 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' ];
    decPrefixes = [ 'm', '\u03bc', 'n', 'p', 'f', 'a', 'z', 'y' ];

    let degree = Math.floor(Math.log10(Math.abs(d)) / 3);
    let scaled = isFinite(degree) ? d * Math.pow(1000, degree * -1) : 0;

    let prefix = null;
    switch (Math.sign(degree))
    {
        case 1:  prefix = incPrefixes[degree - 1]; break;
        case -1: prefix = decPrefixes[-degree - 1]; break;
    }

    if (prefix)
        return scaled.toFixed(3) + ' ' + prefix + unit;
    else
        return scaled.toFixed(3) + ' ' + unit;
}

async function updateField(url, id, select = ((s) => s)) {
    try {
        const response = await fetch(url, {
            method: 'GET',
            cache: 'no-cache'
        });
    
        let responseText = await response.text();
        responseText = select(responseText);
    
        document.getElementById(id).innerHTML = responseText;
        updateAPIError(false);
        return responseText;
    } catch (ex) {
        console.debug(ex);
        updateAPIError(true);
        return null;
    }
}

async function updateVariables() {
    await updateField("http://192.168.4.1/api/totalcount", "totalcount");
    var frequency = await updateField("http://192.168.4.1/api/frequency", "frequency");
    await updateField("http://192.168.4.1/api/frequency_max", "frequency_max");
    await updateField("http://192.168.4.1/api/doserate", "doserate", val => toSI(val, "Sv/h"));
    await updateField("http://192.168.4.1/api/doserate_max", "doserate_max", val => toSI(val, "Sv/h"));

    if (frequency > 100)
        document.querySelector("body").classList.add("danger");
    else
        document.querySelector("body").classList.remove("danger");


    window.setTimeout(updateVariables, 250);
}

window.onload = () => {
    window.setTimeout(updateVariables, 250);

    document.querySelector("#totalcount_delete").addEventListener("click", async () => {
        await fetch("http://192.168.4.1/api/reset_max", {
            method: 'POST',
            cache: 'no-cache'
        });
    });
};