#include "storage.h"
#include <iostream>

TStorage::TStorage() {
    faculties["famcs"] = TFacultyInfo("FAMCS", "famcs",
                                      "Faculty of applied mathematics and computer science", 4);
    faculties["mmf"] = TFacultyInfo("MMF", "mmf",
                                    "Faculty of Mathematics and Mechanics", 5);
    faculties["physfak"] = TFacultyInfo("Physics", "physfak",
                                    "Faculty of physics", 3);
       
}

TStorage::~TStorage() {}

std::vector<TReducedFacultyInfo> TStorage::GetFaculties(uint32_t mark) {
    std::vector<TReducedFacultyInfo> fs;
    std::cout << "mark = " << mark << "\n";
    for (const auto& faculty : faculties) {
        if (mark > MAX_MARK) {
            fs.push_back(TReducedFacultyInfo(faculty.second.name, faculty.second.facultyId,
                                             faculty.second.slotsCount, faculty.second.applicationsCount));
            continue;
        }
        uint32_t count_prior_slots = 0;
        for (const auto& item : faculty.second.distributionHist) {
            if (mark <= item.markMax) {
                count_prior_slots += item.applications;
            }
        }
        std::cout << count_prior_slots << "\n";
        if (count_prior_slots < faculty.second.slotsCount) {
            fs.push_back(TReducedFacultyInfo(faculty.second.name, faculty.second.facultyId,
                                             faculty.second.slotsCount,
                                             faculty.second.applicationsCount));   
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
    for (auto& bin : facultyIt->second.distributionHist) {
        if ((mark >= bin.markMin) && (mark <= bin.markMax)) {
            bin.applications -= 1;
            break;
        }
    }
    applications.erase(appIt);
    return true;
}

bool TStorage::AddApplicationToFaculty(std::string facultyId, uint32_t mark) {
    auto facultyIt = faculties.find(facultyId);
    if (facultyIt == faculties.end())
        return false;
    facultyIt->second.applicationsCount += 1;
    for (auto& bin : facultyIt->second.distributionHist) {
        if ((mark >= bin.markMin) && (mark <= bin.markMax)) {
            bin.applications += 1;
            break;
        }
    }
    return true;
}