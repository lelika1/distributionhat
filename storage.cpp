#include "storage.h"
#include <iostream>

TStorage::TStorage() {
    faculties["famcs"] = TFacultyInfo("FAMCS", "famcs", 4, 0,
                                      "Faculty of applied mathematics and computer science");
    faculties["mmf"] = TFacultyInfo("MMF", "mmf", 5, 0,
                                    "Faculty of Mathematics and Mechanics");
    faculties["physfak"] = TFacultyInfo("Physics", "physfak", 3, 0,
                                        "Faculty of physics");      
}

TStorage::~TStorage() {}

std::vector<TFacultyInfo> TStorage::GetFaculties(uint32_t mark) {
    std::vector<TFacultyInfo> fs;
    for (const auto& faculty : faculties) {
        if (mark > MAX_MARK) {
            fs.push_back(faculty.second);
            continue;
        }
        uint32_t count_prior_slots = 0;
        uint32_t index = (mark - MIN_MARK) / DISTRIBUTION_STEP;
        for (size_t i = index; i < faculty.second.distributionHist.size(); ++i) {
            count_prior_slots += faculty.second.distributionHist[i];
        }

        if (count_prior_slots < faculty.second.slotsCount) {
            fs.push_back(faculty.second);   
        }
    }
    return fs;
}

bool TStorage::GetFaculty(const std::string& facultyId, TFacultyInfo& info) {
    auto facultyIt = faculties.find(facultyId);
    if (facultyIt == faculties.end())
        return false;
    info = facultyIt->second;
    return true;
}

bool TStorage::AddApplication(const std::string& facultyId,
                              const std::string& applicationId,
                              uint32_t mark)
{
    if (mark < MIN_MARK || mark > MAX_MARK) {
        return false;
    }

    if (applications.find(applicationId) != applications.end()) {
        return false;
    }
    applications[applicationId] = TApplicationDescription(applicationId,
                                                          facultyId, mark);
    return AddApplicationToFaculty(facultyId, mark);
}

bool TStorage::DeleteAppliation(const std::string& facultyId,
                                const std::string& applicationId)
{
    auto appIt = applications.find(applicationId);
    if (appIt == applications.end()) {
        return false;
    }

    auto facultyIt = faculties.find(appIt->second.facultyId);
    if (facultyIt == faculties.end())
        return false;
    facultyIt->second.applicationsCount -= 1;
    uint32_t mark = appIt->second.mark;
    uint32_t index = (mark - MIN_MARK) / DISTRIBUTION_STEP;
    facultyIt->second.distributionHist[index] -= 1;
    applications.erase(appIt);
    return true;
    return  false;
}

bool TStorage::AddApplicationToFaculty(std::string facultyId, uint32_t mark) {
    auto facultyIt = faculties.find(facultyId);
    if (facultyIt == faculties.end())
        return false;
    facultyIt->second.applicationsCount += 1;
    uint32_t index = (mark - MIN_MARK) / DISTRIBUTION_STEP;
    facultyIt->second.distributionHist[index] += 1;
    return true;
}