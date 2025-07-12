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

  public:
    std::array<Semester, 8> loadJson();

    void createBasicJson();

    void createJsonData(int index, Course::Course& c);
    void deleteJsonData(int index, int removeIndex);

    // 오버로딩으로 데이터 수정 함수 구현
    void updateJsonData(int index, int course_index, const std::string& key, std::string& updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, int updateValue);
    void updateJsonData(int index, int course_index, const std::string& key, double updateValue);

    nlohmann::json courseToJson(Course::Course& c);
    Course::Course jsonToCourse(nlohmann::json& course_json);
    

    //void readJson();
    //void updateJson();

};


/*
void SemesterJSON::createJson()
{
  std::string filename = "semesters.json";
  ConsoleUIManager consoleUIManager;

  // JSON 생성 및 입력 - JSON 객체를 만들고 파일로 저장
  std::cout << "--- JSON 데이터 생성 ---" << std::endl;

  // json 객체 생성
  nlohmann::json create_data;

  // json 객체에 직접 데이터 입력
  create_data["year"] = 4;
  create_data["semester"] = 1;  
  create_data["test"] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  

  // json객체 먼저 생성 후 데이터 입력
  nlohmann::json courseName = "자료구조";
  nlohmann::json credits = 3;
  nlohmann::json grade = 4.5;
  nlohmann::json category = 1;

  nlohmann::json courses;
  courses["courseName"] = courseName;
  courses["credits"] = credits;
  courses["grade"] = grade;
  courses["category"] = category;

  create_data["courses"] = courses;

  // filename으로  json파일 생성
  std::ofstream create_file(filename);

  // json파일에 데이터 입력
  create_file << create_data.dump(4); // 2칸 들여쓰기로 보기 좋게 저장
  create_file.close();

  std::cout << "[" << filename << "] 파일 생성 완료.\n" << std::endl;
}


void SemesterJSON::readJson()
{
// JSON Read - 파일에서 JSON 데이터를 읽고 출력
std::cout << "--- JSON 데이터 읽기 ---" << std::endl;
std::ifstream read_file(filename);

// JSON 데이터를 바로 읽어오기
nlohmann::json semester_json;
read_file >> semester_json;  

// JSON 데이터를 객체로 가져오기
std::vector<Semester> student_list;

// 단일 데이터 불러오기 - json.get<type>() 으로 데이터를 가져올 수 있다  
int year = semester_json.at("year").get<int>();
int semester = semester_json.at("semester").get<int>();
Semester s(year, semester);
std::cout << s.getYear() << "학년 " << s.getSemester() << "학기" << std::endl;

// 내부 데이터 불러오기 - json 객체로 먼저 읽기
Course::Course c;
nlohmann::json course_json = semester_json.at("courses");

// 내부 json 객체를 이용하여 각 구조체에 값 할당
c.courseName = course_json.at("courseName").get<std::string>();
c.credits = course_json.at("credits").get<int>();
c.grade = course_json.at("grade").get<double>();
c.category= course_json.at("category").get<int>();
s.addCourses(c);

std::vector<Course::Course>& test_c = s.getCourses();
consoleUIManager.displayCourse(test_c.back());  

std::vector<int> v = semester_json.at("test").get<std::vector<int>>();
for (int i : v)
  std::cout << i << ", ";

read_file.close();
}


void SemesterJSON::updateJson()
{

// JSON Update - 데이터를 수정하고 새로운 데이터 추가
std::cout << "--- JSON 데이터 수정 ---" << std::endl;
semester_json["year"] = 9999999;            // 기존 값 수정
semester_json["courses"]["courseName"] = "이것은 첫번째 레슨";
semester_json["test"][5] = 55555;           // 배열 데이터 값 수정

semester_json["student_name"] = "홍길동";   // 새로운 값 추가
semester_json["student_id"] = 201904022;    // 새로운 값 추가
semester_json["test"].push_back(9999999);   // 배열 데이터 값 추가

semester_json["courses"]["update_value"] = "내부 데이터에 새로운 데이터 추가";

std::ofstream update_file(filename);
update_file << semester_json.dump(2);
update_file.close();
std::cout << "데이터 수정 완료. 수정된 내용:\n" << semester_json.dump(2) << "\n" << std::endl;

}


void SemesterJSON::deleteJson()
{
// JSON  Delete - 특정 데이터 삭제
std::cout << "--- JSON 데이터 삭제 ---" << std::endl;
semester_json.erase("year");       // "year" 키와 값 삭제
semester_json.erase("semester");   // "semester" 키와 값 삭제
semester_json["courses"].erase("update_value"); // 내부 데이터 제거

// 배열 데이터 값 제거 - erase(index)를 이용하여 제거
semester_json["test"].erase(5);
semester_json["test"].erase(9);

std::ofstream delete_file(filename);
delete_file << semester_json.dump(2);
delete_file.close();
std::cout << "데이터 삭제 완료. 최종 내용:\n" << semester_json.dump(2) << std::endl;
}

*/