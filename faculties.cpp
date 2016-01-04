#include "faculties.h"

#include <fastcgi2/stream.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace {

const size_t FACULTIES_PREFIX_SIZE = sizeof("/faculties/") - 1;

bool IsFacultiesList(const std::string& path, const std::string& method) {
    return path == "/faculties" && method == "GET";
}

bool IsFacultyInfo(const std::string& path, const std::string& method,
                   std::string* facultyId)
{
    if (method == "GET" &&
        path.find("/faculties/") == 0 &&
        path.find('/', FACULTIES_PREFIX_SIZE) == std::string::npos)
    {
        *facultyId = path.substr(FACULTIES_PREFIX_SIZE);
        return true;
    }
    return false;
}

bool IsAddApplication(const std::string& path, const std::string& method,
                      std::string* facultyId)
{
    if (method == "POST" &&
        path.find("/faculties/") == 0 &&
        path.find('/', FACULTIES_PREFIX_SIZE) == std::string::npos)
    {
        *facultyId = path.substr(FACULTIES_PREFIX_SIZE);
        return true;
    }
    return false;
}

bool IsDeleteApplication(const std::string& path, const std::string& method,
                         std::string* facultyId, std::string* applicationId)
{
    return false;
}

void PrintReducedFacultyInfo(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
                             const TReducedFacultyInfo& facultyInfo)
{
    writer.StartObject();
    writer.String("id");
    writer.String(facultyInfo.facultyId.c_str());
    writer.String("name");
    writer.String(facultyInfo.name.c_str());
    writer.String("slotsCount");
    writer.Int(facultyInfo.slotsCount);
    writer.String("applicationsCount");
    writer.Int(facultyInfo.applicationsCount);
    writer.EndObject();
}

void PrintFacultyInfo(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer,
                      const TFacultyInfo& facultyInfo)
{
    writer.StartObject();
    writer.String("id");
    writer.String(facultyInfo.facultyId.c_str());
    writer.String("name");
    writer.String(facultyInfo.name.c_str());
    writer.String("description");
    writer.String(facultyInfo.facultyDescription.c_str());
    writer.String("slotsCount");
    writer.Int(facultyInfo.slotsCount);
    writer.String("applicationsCount");
    writer.Int(facultyInfo.applicationsCount);
    writer.String("distributionHist");
    writer.StartArray();
    for (const auto& bin : facultyInfo.distributionHist) {
        writer.StartObject();
        writer.String("markmin");
        writer.Int(bin.markMin);
        writer.String("markMax");
        writer.Int(bin.markMax);
        writer.String("applications");
        writer.Int(bin.applications);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
}

} // anonymous namespace

namespace NFaculties {

TFaculties::TFaculties() {}

TFaculties::~TFaculties() = default;

void TFaculties::ScheduleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
    auto& path = request->getScriptName();
    auto& method = request->getRequestMethod();
    request->setContentType("application/json");
    std::string facultyId;
    std::string applicationId;
    // std::cout << path << std::endl;
    if (IsFacultiesList(path, method)) {
        FacultiesList(request, context);
    } else if (IsFacultyInfo(path, method, &facultyId)) {
        FacultyInfo(request, context, facultyId);
    } else if (IsAddApplication(path, method, &facultyId)) {
        ApplicationCreate(request, context, facultyId);
    } else if (IsDeleteApplication(path, method, &facultyId, &applicationId)) {
        ApplicationDelete(request, context, facultyId, applicationId);
    } else {
        request->setStatus(404);
    }
}

void TFaculties::FacultiesList(fastcgi::Request *request,
                               fastcgi::HandlerContext *context)
{
    uint32_t mark = request->hasArg("mark") ? stoi(request->getArg("mark")) : MAX_MARK + 1;
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();
    writer.String("faculties");
    writer.StartArray();
    for (const auto& facultyInfo : storage.GetFaculties(mark)) {
        PrintReducedFacultyInfo(writer, facultyInfo);
    }
    writer.EndArray();
    writer.EndObject();
    request->write(sb.GetString(), sb.GetSize());
    request->setStatus(200);
}

void TFaculties::FacultyInfo(fastcgi::Request *request, fastcgi::HandlerContext *context,
                             const std::string& facultyId)
{
    TFacultyInfo info;
    bool st = storage.GetFaculty(facultyId, info);
    if (!st) {
        request->setStatus(404);
        return;
    }
    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    PrintFacultyInfo(writer, info);
    request->write(sb.GetString(), sb.GetSize());
    request->setStatus(200);
}

void TFaculties::ApplicationCreate(fastcgi::Request *request, fastcgi::HandlerContext *context,
                                   const std::string& facultyId)
{
    std::string inputJson;
    request->requestBody().toString(inputJson);
    rapidjson::Document document;
    document.Parse(inputJson.c_str());
    if (!(document.HasMember("passport") && document["passport"].IsString() &&
          document.HasMember("mark") && document["mark"].IsInt()))
    {
        request->setStatus(400);
        return;
    }

    bool st = storage.AddApplication(facultyId, document["passport"].GetString(),
                                     document["mark"].GetInt());
    if (!st) {
        request->setStatus(400);
    } else {
        request->setStatus(200);
    }
}

void TFaculties::ApplicationDelete(fastcgi::Request *request, fastcgi::HandlerContext *context,
                                   const std::string& facultyId, const std::string& applicationId) {
    std::cout << "ApplicationDelete\n";
    bool st = storage.DeleteAppliation(facultyId, applicationId);
    if (!st) {
        request->setStatus(404);
    } else {
        request->setStatus(200);
    }
}

} // NFaculties
