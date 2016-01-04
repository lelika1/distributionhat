#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <sstream>

#include "faculties.h"

class DHatFastCGI : virtual public fastcgi::Component, virtual public fastcgi::Handler
{
public:
    DHatFastCGI(fastcgi::ComponentContext *context) :
            fastcgi::Component(context)
    {

    }

    virtual void onLoad()
    {

    }

    virtual void onUnload()
    {

    }

    virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context)
    {
        faculties.ScheduleRequest(request, context);
        // request->setContentType("text/plain");
        // auto method = request->getRequestMethod();
        // request->write(method.c_str(), method.length());
        // std::string body;
        // request->requestBody().toString(body);
        // request->write("\n\n", 2);
        // request->write(body.c_str(), body.length());
        // request->write("\n\n", 2);
        // std::stringbuf buffer("Hello " + (request->hasArg("name") ? request->getArg("name") : "Lelika"));
        // request->write(&buffer);
    }
private:
    NFaculties::TFaculties faculties;
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("DHatFastCGIFactory", DHatFastCGI)
FCGIDAEMON_REGISTER_FACTORIES_END()