#pragma once // 헤더 중복되지 않게 하는 설정
#include "Semester.h"
#include "Course.h"

#include <vector>
#include <string>
#include <array>

class Semester;

class ConsoleUIManager
{
  public:
    int displaySemesterChoice(std::array<Semester, 8>& semesters); // 학기 선택
    int displayCourseChoice(int year, int semester); // 과목 선택
    int displaySortChoice();
    void displayCourse(const Course::Course& c);      // 한 과목의 정보 출력
    void displayCoursesInSemester(Semester& s);                 // 한 학기의 모든 과목 출력
    void displayMessage(const std::string& message);


    int promptFixValue(const Course::Course& fixC);
    std::string promptFixString(const std::string& promptMessage);
    int promptFixInt(const std::string& promptMessage, int min, int max);
    double promptFixDouble(const std::string& promptMessage, double min, double max);
    int promptChoiceCourseIndex(const std::vector<Course::Course>& courses);

    Course::Course createAddCourse();
};

