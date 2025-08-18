#include "json.hpp"
#include "Semester.h"
#include "Course.h"
#include "SemesterJSON.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>


SemesterJSON::SemesterJSON() : filename("semesters.json") {}
SemesterJSON::SemesterJSON(std::string filename) : filename(filename) {}


nlohmann::json SemesterJSON::getSemesterJson()
{
  std::ifstream read_json(filename);
  nlohmann::json json;
  read_json >> json;  
  read_json.close();
  return json;
}

// 기본 JSON 생성
void SemesterJSON::createBasicJson()
{
  // filename으로  json파일 생성
  std::ofstream create_file(filename);

  // json 객체 생성
  nlohmann::json create_json;
  create_json["semesters"] = {};

  // 과목 json
  nlohmann::json course_json;
  course_json["courseName"] = "빈과목";
  course_json["credits"] = 0;
  course_json["grade"] = 0.0;
  course_json["category"] = 0;

  // 1학년 1학기 ~ 4학년 2학기까지 json 파일 생성
  int year = 1;
  int semester = 1;
  Course::Course c = {"", 0, 0.0, 0};
  std::vector<nlohmann::json> v = {course_json};
  while(year <= 4)
  {
    // 학기 json
    nlohmann::json semesters_json;
    if (year == 1 && semester == 1)
    {
      semesters_json = {
        std::pair("year", year),
        std::pair("semester", semester),
        std::pair("courses", v)
      };
    }
    else
    {
      semesters_json = {
        std::pair("year", year),
        std::pair("semester", semester),
        std::pair("courses", std::vector<int>(0))
      };
    }

    if (++semester == 3)
    {
      year++;
      semester = 1;
    }  
  
    create_json["semesters"].push_back(semesters_json);
    
  }

  // json파일에 데이터 입력
  create_file << create_json.dump(4);
  create_file.close();
}

std::array<Semester, 8> SemesterJSON::loadJson()
{
  std::ifstream check_json(filename);
  if (!check_json.is_open())
  {
    createBasicJson();
  }
  check_json.close();

  nlohmann::json json = getSemesterJson();

  std::array<Semester, 8> semesters
  {
    jsonToSemester(json["semesters"], 0),
    jsonToSemester(json["semesters"], 1),
    jsonToSemester(json["semesters"], 2),
    jsonToSemester(json["semesters"], 3),
    jsonToSemester(json["semesters"], 4),
    jsonToSemester(json["semesters"], 5),
    jsonToSemester(json["semesters"], 6),
    jsonToSemester(json["semesters"], 7)
  };

  return semesters;
}

std::array<Semester, 8> SemesterJSON::loadJson(std::string filePath)
{
  std::ifstream read_json(filePath);
  nlohmann::json json;
  read_json >> json;  
  read_json.close();

  std::array<Semester, 8> semesters
  {
    jsonToSemester(json["semesters"], 0),
    jsonToSemester(json["semesters"], 1),
    jsonToSemester(json["semesters"], 2),
    jsonToSemester(json["semesters"], 3),
    jsonToSemester(json["semesters"], 4),
    jsonToSemester(json["semesters"], 5),
    jsonToSemester(json["semesters"], 6),
    jsonToSemester(json["semesters"], 7)
  };

  return semesters;
}



// JSON 데이터 저장
void SemesterJSON::saveJson(const std::array<Semester, 8>& semesters)
{
  // json 객체 생성
  nlohmann::json create_json;
  create_json["semesters"] = {};

  // 1학년 1학기 ~ 4학년 2학기까지 json 파일 생성
  for (Semester s : semesters)
  {
    int year = s.getYear();
    int semester = s.getSemester();

    std::vector<nlohmann::json> courses;
    for (Course::Course& c : s.getCourses())
    {
      nlohmann::json cJson = courseToJson(c);
      courses.push_back(cJson);
    }

    nlohmann::json semesters_json;
    semesters_json = {
      std::pair("year", year),
      std::pair("semester", semester),
      std::pair("courses", courses)
    };

    create_json["semesters"].push_back(semesters_json);
  }

  // filename으로  json파일 생성
  std::ofstream save_file(filename);

  // json파일에 데이터 입력
  save_file << create_json.dump(4);
  save_file.close();
}


Semester SemesterJSON::jsonToSemester(nlohmann::json& semester_json, int index)
{
  // 학년, 학기 가져오기
  int year, semester;
  year = semester_json.at(index)["year"].get<int>();
  semester = semester_json.at(index)["semester"].get<int>();

  // 기본 학기 생성
  Semester return_semester(year, semester);

  // 과목이 있으면 모두 push
  nlohmann::json& course_json = semester_json.at(index)["courses"];
  for (int i=0; i < course_json.size(); i++)
  {
    Course::Course c = jsonToCourse(course_json[i]);
    return_semester.addCourses(c);
  }
  
  return return_semester;
}

// 과목 구조체 -> JSON
nlohmann::json SemesterJSON::courseToJson(Course::Course& c)
{
  nlohmann::json course_json;
  course_json["courseName"] = c.courseName;
  course_json["credits"] = c.credits;
  course_json["grade"] = c.grade;
  course_json["category"] = c.category;
  return course_json;
}

// JSON -> 과목 구조체
Course::Course SemesterJSON::jsonToCourse(nlohmann::json& course_json)
{
  Course::Course c;
  c.courseName = course_json["courseName"].get<std::string>();
  c.credits    = course_json["credits"].get<int>();
  c.grade      = course_json["grade"].get<double>();
  c.category   = course_json["category"].get<int>();
  return c;
}

// 과목 추가 시 과목 구조체를 JSON에 업데이트
void SemesterJSON::createJsonData(int index, Course::Course& c)
{
  nlohmann::json json = getSemesterJson();
  nlohmann::json& semesters_json = json.at("semesters");
  semesters_json.at(index)["courses"].push_back(courseToJson(c));

  std::ofstream update_file(filename);
  update_file << json.dump(4);
  update_file.close();
}

// 과목 제거 시 제거할 과목 Index를 JSON에서 제거
void SemesterJSON::deleteJsonData(int index, int removeIndex)
{
  nlohmann::json json = getSemesterJson();
  nlohmann::json& semesters_json = json.at("semesters");
  semesters_json.at(index)["courses"].erase(removeIndex);

  std::ofstream delete_file(filename);
  delete_file << json.dump(4);
  delete_file.close();
}


void SemesterJSON::updateJsonData(int index, int course_index, const std::string& key, std::string& updateValue)
{
  nlohmann::json json = getSemesterJson();
  nlohmann::json& semesters_json = json.at("semesters");
  nlohmann::json& courses_json = semesters_json.at(index)["courses"];
  courses_json.at(course_index)[key] = updateValue;

  std::ofstream update_file(filename);
  update_file << json.dump(4);
  update_file.close();
}
void SemesterJSON::updateJsonData(int index, int course_index, const std::string& key, int updateValue)
{
  nlohmann::json json = getSemesterJson();
  nlohmann::json& semesters_json = json.at("semesters");
  nlohmann::json& courses_json = semesters_json.at(index)["courses"];
  courses_json.at(course_index)[key] = updateValue;

  std::ofstream update_file(filename);
  update_file << json.dump(4);
  update_file.close();
}
void SemesterJSON::updateJsonData(int index, int course_index, const std::string& key, double updateValue)
{
  nlohmann::json json = getSemesterJson();
  nlohmann::json& semesters_json = json.at("semesters");
  nlohmann::json& courses_json = semesters_json.at(index)["courses"];
  courses_json.at(course_index)[key] = updateValue;

  std::ofstream update_file(filename);
  update_file << json.dump(4);
  update_file.close();
}


void SemesterJSON::sortJsonData(int index, int sort_index)
{
  nlohmann::json json = getSemesterJson();

  nlohmann::json& semesters_json = json.at("semesters");
  nlohmann::json& courses_json = semesters_json.at(index)["courses"];

  if (sort_index == 1)
    std::sort(courses_json.begin(), courses_json.end(), courseJsonNameCompare);
  else if (sort_index == 2)
    std::sort(courses_json.begin(), courses_json.end(), courseJsonCreditsCompare);
  else if (sort_index == 3)
    std::sort(courses_json.begin(), courses_json.end(), courseJsonGradeCompare);
  else if (sort_index == 4)
    std::sort(courses_json.begin(), courses_json.end(), courseJsonCategoryCompare);

  std::ofstream update_file(filename);
  update_file << json.dump(4);
  update_file.close();
}


// 정렬을 위한 사용자 정의 규칙
bool courseJsonNameCompare(const nlohmann::json& lhs, const nlohmann::json& rhs) { return lhs["courseName"].get<std::string>() < rhs["courseName"].get<std::string>(); }
bool courseJsonCreditsCompare(const nlohmann::json& lhs, const nlohmann::json& rhs) { return lhs["credits"].get<int>() < rhs["credits"].get<int>(); }
bool courseJsonGradeCompare(const nlohmann::json& lhs, const nlohmann::json& rhs) { return lhs["grade"].get<double>() < rhs["grade"].get<double>(); }
bool courseJsonCategoryCompare(const nlohmann::json& lhs, const nlohmann::json& rhs) { return lhs["category"].get<int>() < rhs["category"].get<int>(); }



