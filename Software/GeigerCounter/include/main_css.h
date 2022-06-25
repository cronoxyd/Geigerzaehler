#ifndef MAIN_CSS_H
#define MAIN_CSS_H

#include "Arduino.h"

const char main_css[] PROGMEM = R"rawliteral(
html,
body {
    font-family: Arial, Helvetica, sans-serif;
    padding: 0;
    margin: 0;
}

body {
    display: flex;
    width: 100vw;
    height: 100vh;
    justify-content: center;
    align-items: center;
    flex-direction: column;
}

* {
    box-sizing: border-box;
}

.danger {
    background-color: red;
    color: white;
}

body::before {
    content: '\2622';
    position: fixed;
    font-size: 50vh;
    right: -10vh;
    bottom: -20vh;
    opacity: 0.25;
}

main {
    width: 35rem;
}

main *:first-child {
    margin-top: 0;
}

table {
    width: 100%;
    border-collapse: collapse;
}

th,
td {
    border-bottom: 1px solid gray;
    padding: 0.5rem;
}

tr:last-child td {
    border-bottom: 0;
}

@media only screen and (max-width: 35rem) {
    body {
        justify-content: flex-start;
    }

    main {
        margin-top: 10rem;
        width: 100%;
        padding: 1rem;
    }
}
)rawliteral";

#endif /* MAIN_CSS_H */