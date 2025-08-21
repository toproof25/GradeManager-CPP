#pragma once // 헤더 중복되지 않게 하는 설정
#include <string>


namespace Course 
{
  // 전공 및 교양 분류
  enum MajorClassification
  {
      BasicRequired        = 0,  // 기초(필수) - Basic Required
      GeneralElective      = 1,  // 일반(선택) - General Elective
      Balance              = 2,  // 균형 - Balance
      Division             = 3,  // 계열 - Division
      DepartmentBasic      = 4,  // 학부기초 - Department Basic
      MajorRequired        = 5,  // 전공필수 - Major Required
      MajorElective        = 6,  // 전공선택(기초포함) - Major Elective (including basic)
      DoubleMajorElective  = 7,  // 복전선택(기초포함) - Double Major Elective (including basic)
      MinorBasic           = 8,  // 부전기초 - Minor Basic
      MinorElective        = 9,  // 부전선택 - Minor Elective
      FreeElective         = 10, // 자선 - Free Elective
      OtherMajor           = 11  // 타전 - Other Major
  };


  /* ---------- 과목 구조체, 사용자 정의 규칙 정렬, 숫자점수->문자점수 변환 ---------- */

  // Course는 과목에 대한 정보를 관리하는 책임을 진다 (저장)
  struct Course
  {
    std::string courseName; // 컴퓨터구조
    int credits;            // 3 (학점)
    double grade;           // 4.5 (A+)
    int category;           // 0 (전공선택)

    void setCourseName(const std::string& name);
    void setCredits(int credits);
    void setGrade(double grade);
    void setCategory(int category);

    // 이진탐색 연산자 오버로딩
    bool operator<(const Course& otherCourse) const 
    {
        return grade < otherCourse.grade;
    }

    // find 연산자 오버로딩
    bool operator==(const Course& other) const {
		    return (courseName == other.courseName) && (credits == other.credits) && (grade == other.grade) && (category == other.category);
	  }
  };

  // 점수와 전공분류를 적절한 문자열로 반환
  std::string doubleToGradeString(const double& grade);
  std::string categoryToString(const int& category);
  
  double stringToGradeDouble(const std::string& grade);
  int stringToCategory(const std::string& category);

  bool courseNameCompareAscendingOrder(const Course& lhs, const Course& rhs);
  bool courseCreditsCompareAscendingOrder(const Course& lhs, const Course& rhs);
  bool courseGradeCompareAscendingOrder(const Course& lhs, const Course& rhs);
  bool courseCategoryCompareAscendingOrder(const Course& lhs, const Course& rhs);

  // 내림차순 정렬을 위한 사용자 정의 규칙
  bool courseNameCompareDescendingOrder(const Course& lhs, const Course& rhs);
  bool courseCreditsCompareDescendingOrder(const Course& lhs, const Course& rhs);
  bool courseGradeCompareDescendingOrder(const Course& lhs, const Course& rhs);
  bool courseCategoryCompareDescendingOrder(const Course& lhs, const Course& rhs);
}