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
    width: 100vw;
    height: 100vh;
    display: flex;
    justify-content: center;
    align-items: center;
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

.d-flex {
    display: flex;
}

.flex-row {
    flex-direction: row;
}

.flex-column {
    flex-direction: column;
}

.justify-content-center {
    justify-content: center;
}

.align-items-center {
    align-items: center;
}

.flex-grow-1 {
    flex-grow: 1;
}

.float-right {
    float: right;
}

#apiError {
    visibility: hidden;
}

.api-reference {
    display: flex;
    flex-direction: column;
    row-gap: 1rem;
    margin-bottom: 3rem;
}

.api-item {
    display: flex;
    padding: 1rem;
    border-radius: 0.5rem;
    column-gap: 1rem;
    align-items: center;
}

.api-item-get {
    background-color: rgba(0, 0, 255, 0.2);
}

.api-item-get .api-method {
    background-color: rgba(0, 0, 255, 0.6);
    color: white;
}

.api-item-delete {
    background-color: rgba(255, 0, 0, 0.2);
}

.api-item-delete .api-method {
    background-color: rgba(255, 0, 0, 0.6);
    color: white;
}

.api-item-post {
    background-color: rgba(0, 255, 0, 0.2);
}

.api-item-post .api-method {
    background-color: rgba(0, 127, 0, 0.6);
    color: white;
}

.api-item-put {
    background-color: rgb(251, 241, 230);
}

.api-item-put .api-method {
    background-color: rgb(252, 161, 48);
    color: white;
}

.api-item .api-method {
    border: 1px solid gray;
    padding: 0.25rem;
    border-radius: 0.1rem;
    width: 5rem;
    flex-shrink: 0;
    text-align: center;
}

.api-item .api-description {
    display: flex;
    flex-direction: column;
}

@media only screen and (max-width: 35rem) {
    body {
        align-items: flex-start;
    }

    main {
        width: 100%;
        padding: 1rem;
    }
}
)rawliteral";

#endif /* MAIN_CSS_H */