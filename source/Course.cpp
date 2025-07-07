#include <string>
#include <iomanip>
#include <iostream>
#include "Course.h"

namespace Course 
{

  // Course 정보 출력
  void printCourse(const Course& c)
  {
    std::cout << "  - " << std::left << std::setw(50) << c.courseName // 과목명 (20칸 좌측 정렬)
              << " | 학점: " << std::setw(10) << c.credits       // 이수학점 (2칸)
              << " | 점수: " << std::setw(10) << convertToGrade(c.grade) // 점수 (4칸)
              << " | 분류: " << std::left << std::setw(20) << convertToCategory(c.category) // 분류 (10칸 좌측 정렬)
              << std::endl;
  }

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

  /* 
  // template를 이용하여 다양한 자료형에 대한 값 처리 (이외에는 오버로딩 혹은 각 value마다 변경 함수를 제작하여 사용할 수 있을듯)
  template <typename T>
  void fixCourseValue(Course& c, int choiceFixValue, T& value)
  {
      if (choiceFixValue == 1)
        c.courseName = value;
      else if (choiceFixValue == 2)
        c.credits = value;
      else if (choiceFixValue == 3)
        c.grade = value;
      else if (choiceFixValue == 4)
        c.category = value;
      else
        std::cout << "잘못입력했습니다..." << std::endl;
  }
  */

  // 각 데이터를 변경하는 setter함수
  void setCourseName(Course& c, const std::string& name) { c.courseName = name; }
  void setCredits(Course& c, int credits) { c.credits = credits; }
  void setGrade(Course& c, double grade) { c.grade = grade; }
  void setCategory(Course& c, int category) { c.category = category; }

  // 정렬을 위한 사용자 정의 규칙
  bool courseCompare(const Course& lhs, const Course& rhs)
  {
    return lhs.grade < rhs.grade;
  }

  // 입력을 받아 Course를 생성 후 반환
  Course getAddCourse()
  {
    std::string courseName; // 과목명
    int credits;            // 이수학점
    double grade;           // 최종점수
    int category;           // 전공분류

    std::cout << "\n--- 추가할 과목 데이터 입력 ---" << std::endl;

    // 과목명 입력
    while (true)
    {
      try
      {
        std::cout << "🏷️ 과목명을 입력하세요 (예: 컴퓨터구조) >>> ";
        std::cin >> courseName;
      
        if (std::cin.fail()) 
        {
          std::cin.clear(); // 입력 스트림의 상태를 초기화
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
          throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
        }
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        continue;
      }
      break;
    };

    // 이수학점 정수 입력
    while (true)
    {
      try
      {
        std::cout << "🔢 이수학점을 입력하세요 (예: 3) >>> ";
        std::cin >> credits;

        if (std::cin.fail()) 
        {
          std::cin.clear(); // 입력 스트림의 상태를 초기화
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
          throw std::invalid_argument("❌ 오류: 숫자를 입력해주세요!");
        }
        else if (credits < 0 || credits > 3)
        {
          throw std::invalid_argument("⚠️ 오류: 학점은 0~3점 범위 내에서 입력해주세요!");
        }
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        continue;
      }
      break;
    };

      
    // 최종점수 실수 입력
    while (true)
    {
      try
      {
        std::cout << "💯 최종점수를 입력하세요 (예: 1.0, 4.5) >>> ";
        std::cin >> grade;

        if (std::cin.fail()) 
        {
          std::cin.clear(); // 입력 스트림의 상태를 초기화
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
          throw std::invalid_argument("❌ 오류: 실수를 입력해주세요!");
        }
        else if (grade < 0.0 || grade > 4.5)
        {
          throw std::invalid_argument("⚠️ 오류: 점수는 0.0~4.5점 범위 내에서 입력해주세요!");
        }
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        continue;
      }
      break;
    };

      
    // 전공분류 정수 입력
    while (true)
    {
      try
      {
        std::cout << "📚 전공 분류를 선택하세요:" << std::endl;
        std::cout << "  1. 전공 필수    2. 복수 전공    3. 부전공   4. 계열   5. 교양  6. 자유 선택   7. 타 전공\n>>> ";
        std::cin >> category;

        if (std::cin.fail()) 
        {
          std::cin.clear(); // 입력 스트림의 상태를 초기화
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
          throw std::invalid_argument("❌ 오류: 숫자를 입력해주세요!");
        }
        else if (category <= 0 || category >=8)
        {
          throw std::invalid_argument("⚠️ 오류: 1~7 사이의 유효한 분류 번호를 입력해주세요!");
        }
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << std::endl;
        continue;
      }
      break;
    };

    Course c = {courseName, credits, grade, --category};
    return c;
  }

  
}