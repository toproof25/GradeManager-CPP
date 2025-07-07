#pragma once // 헤더 중복되지 않게 하는 설정
#include <string>

namespace Course 
{

  // 전공 분류
  enum MajorClassification
  {
    MajorRequired   = 0,  // 전선 (전공 필수 - Major Required)
    DoubleMajor     = 1,  // 복전 (복수 전공 - Double Major)
    Minor           = 2,  // 부전 (부전공 - Minor)
    Division        = 3,  // 계열 (계열 필수/선택 - Division)
    GeneralElective = 4,  // 교양 (교양 - General Elective)
    Elective        = 5,  // 자선 (자유 선택 - Elective)
    OtherMajor      = 6   // 타전 (타 전공 - Other Major)
  };

  /* ---------- 과목 구조체, 사용자 정의 규칙 정렬, 숫자점수->문자점수 변환 ---------- */

  // Course는 과목에 대한 정보를 관리하는 책임을 진다 (저장)
  struct Course
  {
    std::string courseName; // 컴퓨터구조
    int credits;            // 3 (학점)
    double grade;           // 4.5 (A+)
    int category;           // 0 (전공선택)

    bool operator<(const Course& otherCourse) const 
    {
        return grade < otherCourse.grade;
    }
  };

  // Course 구조체를 참조하여 정보를 출력
  void printCourse(const Course& c);

  // 점수와 전공분류를 적절한 문자열로 반환
  std::string convertToGrade(const int& grade);
  std::string convertToCategory(const int& category);

  // Course 객체의 변수값을 변경
  // template는 과목명(string), 점수(double), 전공분류와 학점(int)
  //template <typename T>
  //void fixCourseValue(Course& c, int choiceFixValue, T& value);

  // template가 오류가 발생하여 각 setter 함수를 제작
  void setCourseName(Course& c, const std::string& name);
  void setCredits(Course& c, int credits);
  void setGrade(Course& c, double grade);
  void setCategory(Course& c, int category);

  Course getAddCourse(); // 각 데이터 입력받아서 완성된 Course구조체를 반환

  bool courseCompare(const Course& lhs, const Course& rhs);


}