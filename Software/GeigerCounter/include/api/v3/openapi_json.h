#ifndef OPENAPI_JSON_H
#define OPENAPI_JSON_H

#include "Arduino.h"

const char openapi_json[] PROGMEM = R"rawliteral(
{
    "openapi": "3.0.2",
    "info": {
        "title": "Geiger counter",
        "version": "1.0.1"
    },
    "servers": [
        {
            "url": "/api"
        }
    ],
    "paths": {
        "/totalcount": {
            "get": {
                "summary": "Gets the total count of disintegrations measured.",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "integer",
                                    "example": "1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/frequency": {
            "get": {
                "summary": "Returns the count of disintegrations per minute.",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "integer",
                                    "example": "1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/frequency_max": {
            "get": {
                "summary": "Returns the maximum measured count of disintegrations per minute (non-persistent).",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "integer",
                                    "example": "1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/dose": {
            "get": {
                "summary": "Returns the radiation dose in µSv/h.",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "number",
                                    "example": "0.1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/dose_max": {
            "get": {
                "summary": "Returns the maximum measured radiation dose in µSv/h (non-persistent).",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "number",
                                    "example": "0.1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/uptime": {
            "get": {
                "summary": "Returns the uptime of the geiger counter in milliseconds.",
                "responses": {
                    "200": {
                        "content": {
                            "text/plain": {
                                "schema": {
                                    "type": "integer",
                                    "example": "1234"
                                }
                            }
                        }
                    }
                }
            }
        },
        "/reset_max": {
            "post": {
                "summary": "Resets the maximum values to 0.",
                "responses": {
                    "200": {}
                }
            }
        }
    }
}
)rawliteral";

#endif /* OPENAPI_JSON_H */
