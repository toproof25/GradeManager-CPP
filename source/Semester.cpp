#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>

#include "Course.h"
#include "Semester.h"

// 과목 목록에서 제거할 conrse index를 입력 후 반환
int getChoiceCourseIndex(const std::vector<Course::Course>& courses)
{
  std::cout << "  [번호] 과목명" << std::endl;
  std::cout << "----------------------------------------" << std::endl;

  int choiceRemoveCourse;
  int i=1;
  for (const Course::Course& c : courses)
  {
    std::cout << i++ << ". " << c.courseName << '\n';
  }
  std::cout << "----------------------------------------" << std::endl;

  // 제거할 과목 선택
  while (true)
  {
    try
    {
      std::cout << ">>> ";
      std::cin >> choiceRemoveCourse;

      if (std::cin.fail()) 
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 숫자를 입력해주세요!");
      }
      else if (choiceRemoveCourse < 1 || choiceRemoveCourse > courses.size())
      {
        throw std::invalid_argument("⚠️ 오류: 제시된 범위 내에서 과목 번호를 선택하세요.");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      std::cout << "다시 시도해주세요." << std::endl;
      continue;
    }
    break;
  };

  return --choiceRemoveCourse;
}




// 과목 조회
void Semester::printCourses()
{
  if (courses.size() > 0)
  {
    std::cout << "\n--- " << year << "학년 " << semester << "학기 과목 목록 ---" << std::endl;
    std::cout << "  " << std::left << std::setw(50) << "과목명" // 과목명 헤더
              << " | " << std::setw(10) << "학점" // 이수학점 헤더
              << " | " << std::setw(10) << "점수" // 점수 헤더
              << " | " << std::left << std::setw(20) << "분류" // 분류 헤더
              << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;

    for (std::vector<Course::Course>::iterator it = courses.begin(); it != courses.end(); it++)
    {
      Course::printCourse(*it);
    }
    
    std::cout << "----------------------------------------------------" << std::endl;
  }
  else
  {
    std::cout << "\n--- " << year << "학년 " << semester << "학기 ---" << std::endl;
    std::cout << "  ❗ 이 학기에는 등록된 과목이 없습니다. ❗" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl; // 구분선
  }
}

// 과목 추가
void Semester::addCourses()
{
  Course::Course c = getAddCourse();
  courses.push_back(c);
  std::cout << "✅ [" << courses.back().courseName << "] 과목이 성공적으로 추가되었습니다! ✅" << std::endl;
}

// 과목 제거
void Semester::removeCourses()
{
  if (courses.size() <= 0)
  {
    std::cout << "\n❌ 제거할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌" << std::endl;
  }
  else
  {
    std::cout << "\n--- 제거할 과목을 선택하세요 ---" << std::endl;
    int removeIndex = getChoiceCourseIndex(courses);
    std::string removeName = (courses.begin() + removeIndex)->courseName;
    courses.erase(courses.begin() + removeIndex);
    std::cout << "\n✅ [" << removeName << "] 과목이 성공적으로 제거되었습니다! ✅" << std::endl;
  }
}

// 과목 수정
void Semester::fixCourses()
{
  if (courses.size() <= 0)
  {
    std::cout << "\n❌ 수정할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌" << std::endl;
  }
  else
  {
    std::cout << "\n--- 수정할 과목을 선택하세요 ---" << std::endl;
    int fixIndex = getChoiceCourseIndex(courses);
    Course::Course fixC = courses.at(fixIndex);


    int choiceFixValue;
    std::cout << "\n--- 수정할 과목의 정보 ---" << std::endl;
    Course::printCourse(fixC);
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "수정할 데이터를 선택하세요" << std::endl;
    std::cout << "1. 과목명\n2. 이수학점\n3. 점수\n4. 전공분류\n>>> ";

    std::cin >> choiceFixValue;

    if (choiceFixValue == 1)
    {
      std::cout << "변경할 과목명을 입력하세요 >>> ";
      std::string fixName;
      std::cin >> fixName;
      Course::setCourseName(fixC, fixName);
    }
    else if (choiceFixValue == 2)
    {
      std::cout << "변경할 이수학점을 입력하세요 >>> ";
      int fixCredits;
      std::cin >> fixCredits;
      Course::setCredits(fixC, fixCredits);
    }
    else if (choiceFixValue == 3)
    {
      std::cout << "변경할 점수를 입력하세요 >>> ";
      double fixGrade;
      std::cin >> fixGrade;
      Course::setGrade(fixC, fixGrade);
    }
    else if (choiceFixValue == 4)
    {
      std::cout << "변경할 전공분류를 입력하세요 >>> ";
      int fixCategory;
      std::cin >> fixCategory;
      Course::setCategory(fixC, fixCategory);
    }
    else
    {
      std::cout << "잘못입력했습니다..." << std::endl;
    }

    std::cout << "\n--- 수정된 과목의 정보 ---" << std::endl;
    Course::printCourse(fixC);
    std::cout << "----------------------------------------" << std::endl;
  }
}




// 입력을 받아 Course를 생성 후 반환
Course::Course getAddCourse()
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

  Course::Course c = {courseName, credits, grade, --category};
  return c;
}

