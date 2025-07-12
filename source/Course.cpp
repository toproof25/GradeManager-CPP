#include <string>
#include <iomanip>
#include <iostream>
#include <limits>
#include "Course.h"

namespace Course 
{

  std::string convertToGrade(const int& grade)
  {
    std::string g[9] = {"A+", "A", "B+", "B", "C+", "C", "D+", "D", "F"};
    int index = 0;

    for (double i = 4.5; i>=0.5; i-=0.5)
    {
      if (grade >= i) 
        return g[index];
      index++;
    }
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
      return "부전공";
      break;

    case Division:
      return "계열";
      break;

    case GeneralElective:
      return "교양";
      break;

    case Elective:
      return "자선";
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