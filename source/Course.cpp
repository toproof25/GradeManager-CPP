#include <string>
#include <iomanip>
#include <iostream>
#include "Course.h"

// 정보 출력
void printCourse(const Course& c)
{
  std::cout << "  - " << std::left << std::setw(50) << c.courseName // 과목명 (20칸 좌측 정렬)
            << " | 학점: " << std::setw(10) << c.credits       // 이수학점 (2칸)
            << " | 점수: " << std::setw(10) << convertToGrade(c.grade) // 점수 (4칸)
            << " | 분류: " << std::left << std::setw(20) << convertToCategory(c.category) // 분류 (10칸 좌측 정렬)
            << std::endl;
}

std::string convertToGrade(int grade)
{
  std::string g[9] = {"A+", "A", "B+", "B", "C+", "C", "D+", "D", "F"};
  int index = 0;

  for (double i = 4.5; i>=0.5; i-=0.5)
  {
    if (grade >= i) 
    {
      return g[index];
    }
    index++;
  }
  return "X";
}

std::string convertToCategory(int category)
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

// 정렬을 위한 사용자 정의 규칙
bool courseCompare(const Course& lhs, const Course& rhs)
{
  return lhs.grade < rhs.grade;
}