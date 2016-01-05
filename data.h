#pragma once

#include <map>
#include <string>
#include <vector>

const uint32_t MIN_MARK = 0;
const uint32_t MAX_MARK = 100;
const uint32_t DISTRIBUTION_STEP = 10;

struct TFacultyInfo {
    TFacultyInfo(const std::string& nm = "", const std::string& id = "",
                 uint32_t slots = 0, uint32_t applications = 0, const std::string& desc = "")
        : name(nm)
        , facultyId(id)
        , facultyDescription(desc)
        , slotsCount(slots)
        , applicationsCount(applications)
    {
        for (uint32_t current_mark = MIN_MARK; current_mark + DISTRIBUTION_STEP <= MAX_MARK;
             current_mark += DISTRIBUTION_STEP) {
            distributionHist.push_back(0);
        }
    }

    std::string name;
    std::string facultyId;
    std::string facultyDescription;
    uint32_t slotsCount;
    uint32_t applicationsCount;
    std::vector<uint32_t> distributionHist;
};

struct TApplicationDescription {
    TApplicationDescription(const std::string& id = "", const std::string& fId = "",
                            uint32_t m = 0)
        : applicationId(id)
        , facultyId(fId)
        , mark(m)
    {}

    std::string applicationId;
    std::string facultyId;
    uint32_t mark;
};

typedef std::map<std::string, TApplicationDescription> TApplications;