#ifndef CAPTIVEREQUESTHANDLER_H
#define CAPTIVEREQUESTHANDLER_H

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index_html.h"

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    String redirectTarget;

    CaptiveRequestHandler(String redirectTarget)
    {
        this->redirectTarget = redirectTarget;
    }
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        request->redirect(redirectTarget);
    }
};

#endif /* CAPTIVEREQUESTHANDLER_H */