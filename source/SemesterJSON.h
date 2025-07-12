#pragma once

#include "json.hpp"
#include "Semester.h"
#include "Course.h"
#include "ConsoleUIManager.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>

class SemesterJSON
{
  private:
    std::string filename = "semesters.json";
    Semester jsonToSemester(nlohmann::json& semester_json, int index);
    nlohmann::json getSemesterJson();

    nlohmann::json courseToJson(Course::Course& c);
    Course::Course jsonToCourse(nlohmann::json& course_json);

  public:
    std::array<Semester, 8> loadJson();

    void createBasicJson();

    void createJsonData(int index, Course::Course& c);
    void deleteJsonData(int index, int removeIndex);

    // 오버로딩으로 데이터 수정 함수 구현
    void updateJsonData(int index, int course_index, const std::string& key, std::string& updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, int updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, double updateValue);

};