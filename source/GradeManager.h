#pragma once // 헤더 중복되지 않게 하는 설정
#include "Semester.h"
#include "Course.h"
#include "ConsoleUIManager.h"
#include "SemesterJSON.h"
#include <array>
#include <vector>

class GradeManager
{
  private:

    enum Menu // 메뉴
    {
      MainChoise       = 0, // 학기 선택 or 모든 과목 정렬
      SemesterChoise   = 1, // 학기 선택
      CourseChoise     = 2, // 학기 내 과목
      CourseSort       = 3, // 모든 과목 조회 및 정렬
    };

    ConsoleUIManager consoleUIManager;
    SemesterJSON semesterJson;

    // 1학년 1학기 ~ 4학년 2학기
    std::array<Semester, 8> semesters;

    // 메뉴 선택, 학기 선택, 과목 선택 변수
    int menu = 1;
    int choiceSemester = 1;
    int choiceCourse = 1;

    std::vector<Course::Course> allCourseVector();

    void sortCourse(std::vector<Course::Course>& courses, int choiceSort);

    void handleSelectSemesters(); // 학기 선택 핸들링
    void handleSelectCourse();    // 과목 선택 핸들링
    void handleSortAllCourse();  // 모든 과목 정렬

    void handleAddCourse();       // 과목 추가
    void handleRemoveCourse();    // 과목 제거
    void handleFixCourse();       // 과목 수정
    void handleSortCourse();      // 과목 정렬

  public:
    GradeManager();
    void run();
};