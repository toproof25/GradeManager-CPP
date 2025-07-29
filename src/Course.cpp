#include <string>
#include <iomanip>
#include <iostream>
#include <limits>
#include <cstring>
#include "Course.h"

namespace Course 
{

  std::string convertToGrade(const double& grade)
  {
    if (grade > 4.5) return "X";
    else if (grade >= 4.5) return "A+";
    else if (grade >= 4.0)
      return "A";
    else if (grade >= 3.5)
      return "B+";
    else if (grade >= 3.0)
      return "B";
    else if (grade >= 2.5)
      return "C+";
    else if (grade >= 2.0)
      return "C";
    else if (grade >= 1.5)
      return "D+";
    else if (grade >= 1.0)
      return "D";
    else if (grade >= 0.0)
      return "P";
    else
      return "X";
  }

  std::string convertToCategory(const int& category)
  {
    switch (category)
    {
    case MajorRequired:
      return "전공선택";
      break;

    case DoubleMajor:
      return "복수전공";
      break;

    case Minor:
      return "기초(필수)";
      break;

    case Division:
      return "일반(선택)";
      break;

    case GeneralElective:
      return "균형교양";
      break;

    case Elective:
      return "계열교양";
      break;

    case OtherMajor:
      return "타전공";
      break;
    
    default:
      return "ERROR";
      break;
    }
  }

  double gradeToConvert(const std::string& grade_str)
  {
      if (grade_str == "A+")    return 4.5;
      if (grade_str == "A")     return 4.0;
      if (grade_str == "B+")    return 3.5;
      if (grade_str == "B")     return 3.0;
      if (grade_str == "C+")    return 2.5;
      if (grade_str == "C")     return 2.0;
      if (grade_str == "D+")    return 1.5;
      if (grade_str == "D")     return 1.0;
      if (grade_str == "P")     return 0.0;
      if (grade_str == "NP")    return 0.0;
      
      // 그 외 "X", "F" 또는 잘못된 입력은 0.0으로 처리
      return 0.0;
  }

  int categoryToConvert(const std::string& category_str)
  {
      if (category_str == "전공선택")   return 0;       // MajorRequired
      if (category_str == "복수전공")   return 1;       // DoubleMajor
      if (category_str == "기초(필수)") return 2;       // Minor
      if (category_str == "일반(선택)") return 3;       // Division
      if (category_str == "균형교양")   return 4;       // GeneralElective
      if (category_str == "계열교양")   return 5;       // Elective
      if (category_str == "타전공")     return 6;       // OtherMajor

      // 그 외 "ERROR" 또는 잘못된 입력은 0 (전공선택)으로 처리
      return 0;
  }

  // 각 데이터를 변경하는 setter함수
  void Course::setCourseName(const std::string& name) { this->courseName = name; }
  void Course::setCredits(int credits) { this->credits = credits; }
  void Course::setGrade(double grade) { this->grade = grade; }
  void Course::setCategory(int category) { this->category = category; }

  // 정렬을 위한 사용자 정의 규칙
  bool courseNameCompare(const Course& lhs, const Course& rhs) { return lhs.courseName < rhs.courseName; }
  bool courseCreditsCompare(const Course& lhs, const Course& rhs) { return lhs.credits < rhs.credits; }
  bool courseGradeCompare(const Course& lhs, const Course& rhs) { return lhs.grade < rhs.grade; }
  bool courseCategoryCompare(const Course& lhs, const Course& rhs) { return lhs.category < rhs.category; }


  
}