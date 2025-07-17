#include <string>
#include <iomanip>
#include <iostream>
#include <limits>
#include "Course.h"

namespace Course 
{

  std::string convertToGrade(const double& grade)
  {
    if (grade > 4.5)
      return "X";
    else if (grade >= 4.5)
      return "A+";
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