#pragma once // 헤더 중복되지 않게 하는 설정
#include <vector>
#include "Course.h"

/* ---------- 학기 클래스 ---------- */
// 학기에 대한 정보과 자신이 가진 과목만 책임을 진다 (정보/ 제공)
class Semester
{
  private:
    int year;
    int semester; 
    std::vector<Course::Course> courses;

  public:
    Semester(int year, int semester);

    void addCourses(Course::Course c);      // 과목 추가 - 따로 Course구조체를 받아와서 push_back만 하도록

    // 선택한 과목을 vector에서 erase로 반복자로 제거 
    void removeCourses(std::vector<Course::Course>::iterator it);   

    int getYear() { return year; }
    int getSemester() { return semester; }
    std::vector<Course::Course>& getCourses() { return courses; }

};


double calculateGPA(const std::vector<Course::Course>& courses);
