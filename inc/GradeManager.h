#pragma once // 헤더 중복되지 않게 하는 설정
#include "Semester.h"
#include "Course.h"
#include "SemesterJSON.h"
#include <array>
#include <vector>
#include <Windows.h>

struct GradeGraphEditValue
{
    bool isShowGradeGraphEditWindow = false;
    float col[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float scale[2] = {0.0f, 5.5f};
    float size[2] = {250, 250};
};


class GradeManager
{
  private:
    GradeGraphEditValue gradeGraphEditValue;

    SemesterJSON semesterJson;

    Semester* semester = nullptr;
    Course::Course* course = nullptr;

    bool m_showAddWindow = false;
    bool m_showEditWindow = false;

    // 1학년 1학기 ~ 4학년 2학기
    std::array<Semester, 8> semesters;
    void setSemesters(std::array<Semester, 8> semesters);

    // 정렬 기준
    bool isNameAscending = false;
    bool isCreditAscending = false;
    bool isGradeAscending = false;
    bool isCategoryAscending = false;

  public:
    GradeManager();
    ~GradeManager();

    GradeGraphEditValue& getGradeGraphEditValue() { return gradeGraphEditValue; }

    void setSelectSemester(Semester& semester);
    void setSelectCourse(Course::Course& course);
    Semester& getSelectSemester();
    Course::Course& getSelectCourse();

    void setShowAddWindow(bool);
    void setShowEditWindow(bool);
    bool& getShowAddWindow();
    bool& getShowEditWindow();


    void handleAddCourse(Course::Course& newCourse);       // 과목 추가
    void handleRemoveCourse();                             // 과목 제거
    void handleFixCourse(Course::Course& fixCourse);       // 과목 수정
    void handleSortCourse(int choiceSort);                 // 과목 정렬

    void handleLoadJson(std::string& filePath);
    void handleSaveJson();
    
    std::array<Semester, 8>& getSemesters() {return this->semesters;};

    std::string handleLoadJsonFile(HWND& hwnd);
};