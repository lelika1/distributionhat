#pragma once

#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include "storage.h"

namespace NFaculties {

class TFaculties {
public:
    TFaculties();
    ~TFaculties();

    void ScheduleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context);

private:
    void FacultiesList(fastcgi::Request *request, fastcgi::HandlerContext *context);
    void FacultyInfo(fastcgi::Request *request, fastcgi::HandlerContext *context,
                     const std::string& facultyId);
    void ApplicationCreate(fastcgi::Request *request, fastcgi::HandlerContext *context,
    					   const std::string& facultyId);
    void ApplicationDelete(fastcgi::Request *request, fastcgi::HandlerContext *context,
                           const std::string& facultyId, const std::string& applicationId);

private:
    TStorage storage;
};

} // namespace NFaculties
