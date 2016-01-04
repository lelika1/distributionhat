#include "data.h"
#include <map>

class TStorage {
public:
    TStorage();
    ~TStorage();
    std::vector<TReducedFacultyInfo> GetFaculties(uint32_t mark);
    bool GetFaculty(const std::string& facultyId, TFacultyInfo& info);
    bool AddApplication(const std::string& facultyId,
                        const std::string& applicationId,
                        uint32_t mark);
    bool DeleteAppliation(const std::string& facultyId,
                        const std::string& applicationId);

private:
    bool AddApplicationToFaculty(std::string facultyId, uint32_t mark);
private:
    std::map<std::string, TFacultyInfo> faculties;
    TApplications applications;
};
