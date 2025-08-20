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
    if (grade > 4.5) return "OVER";
    else if (grade >= 4.5) return "A+";
    else if (grade >= 4.0) return "A";
    else if (grade >= 3.5) return "B+";
    else if (grade >= 3.0) return "B";
    else if (grade >= 2.5) return "C+";
    else if (grade >= 2.0) return "C";
    else if (grade >= 1.5) return "D+";
    else if (grade >= 1.0) return "D";
    else if (grade >= 0.0) return "P";
    else if (grade >= -1.0) return "NP";
    else if (grade >= -2.0) return "F";
    else return "ERROR";
  }

  std::string convertToCategory(const int& category)
  {
      if      (category == BasicRequired)       return "기초";
      else if (category == GeneralElective)     return "일반";
      else if (category == Balance)             return "균형";
      else if (category == Division)            return "계열";
      else if (category == DepartmentBasic)     return "학부기초";
      else if (category == MajorRequired)       return "전공필수";
      else if (category == MajorElective)       return "전공선택";
      else if (category == DoubleMajorElective) return "복전선택";
      else if (category == MinorBasic)          return "부전기초";
      else if (category == MinorElective)       return "부전선택";
      else if (category == FreeElective)        return "자선";
      else if (category == OtherMajor)          return "타전";
      else return "ERROR";
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
      if (grade_str == "NP")    return -1.0; 
      if (grade_str == "F")     return -2.0; 

      // 그 외 잘못된 입력은 0.0으로 처리
      return 0.0;
  }

int categoryToConvert(const std::string& category_str)
{
    if      (category_str == "기초")     return BasicRequired;
    else if (category_str == "일반")     return GeneralElective;
    else if (category_str == "균형")     return Balance;
    else if (category_str == "계열")     return Division;
    else if (category_str == "학부기초") return DepartmentBasic;
    else if (category_str == "전공필수") return MajorRequired;
    else if (category_str == "전공선택") return MajorElective;
    else if (category_str == "복전선택") return DoubleMajorElective;
    else if (category_str == "부전기초") return MinorBasic;
    else if (category_str == "부전선택") return MinorElective;
    else if (category_str == "자선")     return FreeElective;
    else if (category_str == "타전")     return OtherMajor;
    else return -1;
}


  // 각 데이터를 변경하는 setter함수
  void Course::setCourseName(const std::string& name) { this->courseName = name; }
  void Course::setCredits(int credits) { this->credits = credits; }
  void Course::setGrade(double grade) { this->grade = grade; }
  void Course::setCategory(int category) { this->category = category; }

  // 오름차순 정렬을 위한 사용자 정의 규칙
  bool courseNameCompareAscendingOrder(const Course& lhs, const Course& rhs) { return lhs.courseName < rhs.courseName; }
  bool courseCreditsCompareAscendingOrder(const Course& lhs, const Course& rhs) { return lhs.credits < rhs.credits; }
  bool courseGradeCompareAscendingOrder(const Course& lhs, const Course& rhs) { return lhs.grade < rhs.grade; }
  bool courseCategoryCompareAscendingOrder(const Course& lhs, const Course& rhs) { return lhs.category < rhs.category; }

  // 내림차순 정렬을 위한 사용자 정의 규칙
  bool courseNameCompareDescendingOrder(const Course& lhs, const Course& rhs) { return lhs.courseName > rhs.courseName; }
  bool courseCreditsCompareDescendingOrder(const Course& lhs, const Course& rhs) { return lhs.credits > rhs.credits; }
  bool courseGradeCompareDescendingOrder(const Course& lhs, const Course& rhs) { return lhs.grade > rhs.grade; }
  bool courseCategoryCompareDescendingOrder(const Course& lhs, const Course& rhs) { return lhs.category > rhs.category; }
  
}