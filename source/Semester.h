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
    Semester(int year, int semester) : year(year), semester(semester) 
    {
      //std::cout << "✨ " << year << "학년 " << semester << "학기 데이터가 생성되었습니다! ✨" << std::endl;
    }
    
    void semesterInfo()
    {
      std::cout << year << "학년 " << semester << "학기 " << std::endl;
    }

    void printCourses();    // 모든 과목 조회
    void addCourses();      // 과목 추가 - 따로 Course구조체를 받아와서 push_back만 하도록
    void removeCourses();   // 선택한 과목을 vector에서 erase로 제거
    void fixCourses();      // 선택한 과목 데이터를 수정 - 수정은 다른 함수로 책임 넘기기

};


int getChoiceCourseIndex(); // 과목 목록에서 선택한 couesrs index를 받아오는 함수
