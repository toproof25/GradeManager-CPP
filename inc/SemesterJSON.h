#pragma once

#include "json.hpp"
#include "Semester.h"
#include "Course.h"
#include <string>

class SemesterJSON
{
  private:
    const std::string filename = "./semesters.json";
    Semester jsonToSemester(nlohmann::json& semester_json, int index);
    nlohmann::json getSemesterJson();

    nlohmann::json courseToJson(Course::Course& c);
    Course::Course jsonToCourse(nlohmann::json& course_json);

  public:
    SemesterJSON();
    SemesterJSON(std::string filename);

    std::array<Semester, 8> loadJson();
    std::array<Semester, 8> loadJson(std::string filePath);
    void saveJson(const std::array<Semester, 8>& semesters);

    void createBasicJson();

    void createJsonData(int index, Course::Course& c);
    void deleteJsonData(int index, int removeIndex);

    // 오버로딩으로 데이터 수정 함수 구현
    void updateJsonData(int index, int course_index, const std::string& key, std::string& updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, int updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, double updateValue);

    void sortJsonData(int index, int sort_index);

    
  

};

// 정렬을 위한 사용자 정의 규칙
bool courseJsonNameCompare(const nlohmann::json& lhs, const nlohmann::json& rhs); 
bool courseJsonCreditsCompare(const nlohmann::json& lhs, const nlohmann::json& rhs);
bool courseJsonGradeCompare(const nlohmann::json& lhs, const nlohmann::json& rhs);
bool courseJsonCategoryCompare(const nlohmann::json& lhs, const nlohmann::json& rhs);