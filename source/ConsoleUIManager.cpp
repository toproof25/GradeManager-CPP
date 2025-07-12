#include "ConsoleUIManager.h"
#include "Semester.h"
#include "Course.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>
#include <array>
#include <vector>

int ConsoleUIManager::displaySemesterChoice(std::array<Semester, 8>& semesters)
{
  int choiceSemester;
  while(true)
  {
    std::cout << "\n--- 학기를 선택하세요 ---" << std::endl;

    std::cout << "0. 모든 과목 조회 및 정렬 조회" << std::endl;
    for (int i=0; i<semesters.size(); ++i)
    {
      std::cout << i+1 << ". ";
      std::cout << semesters[i].getYear() << "학년 " << semesters[i].getSemester() << "학기 " << std::endl;
    }

    std::cout << "-----------------------" << std::endl;
    std::cout << ">>> ";

    try
    {
      std::cin >> choiceSemester;
      if (std::cin.fail()) 
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
      }
      else if (choiceSemester < 0 || choiceSemester > 8)
      {
        throw std::invalid_argument("❌ 오류: 0~8 범위 내에서 입력하세요");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  }

  if (choiceSemester == 0){
    //menu = Menu::CourseSort;
    return -1;
  }
  else{
    return --choiceSemester;
  }
}
int ConsoleUIManager::displayCourseChoice()
{
  int choiceCourse;
  
  std::cout << "\n--- 메뉴를 선택하세요 ---" << std::endl;
  std::cout << "0. 학기 선택" << std::endl;
  std::cout << "1. 과목 조회" << std::endl;
  std::cout << "2. 과목 추가" << std::endl;
  std::cout << "3. 과목 제거" << std::endl;
  std::cout << "4. 과목 수정" << std::endl;
  std::cout << "5. 과목 정렬" << std::endl;
  std::cout << "-----------------------" << std::endl;
  std::cout << ">>> ";

  try
  {
    std::cin >> choiceCourse;
    if (std::cin.fail()) 
    {
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
    }
    else if (choiceCourse < 0 || choiceCourse > 5)
    {
      throw std::invalid_argument("❌ 오류: 0~5 범위 내에서 입력하세요");
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  return choiceCourse;
}
int ConsoleUIManager::promptFixValue(const Course::Course& fixC)
{
  int choiceFixValue;
  std::cout << "\n--- 수정할 과목의 정보 ---" << std::endl;
  displayCourse(fixC);
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "수정할 데이터를 선택하세요" << std::endl;
  std::cout << "1. 과목명\n2. 이수학점\n3. 점수\n4. 전공분류\n>>> ";

  while(true)
  {
    try
    {
      std::cin >> choiceFixValue;
      if (std::cin.fail()) 
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
      }
      else if (choiceFixValue < 1 || choiceFixValue > 4)
      {
        throw std::invalid_argument("❌ 오류: 1~4 범위 내에서 입력하세요");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  }

  return choiceFixValue;
}
int ConsoleUIManager::displaySortChoice()
{
  int choiceSort;
  std::cout << "1. 과목 이름순 정렬" << std::endl;
  std::cout << "2. 과목 학점순 정렬" << std::endl;
  std::cout << "3. 과목 점수순 정렬" << std::endl;
  std::cout << "4. 과목 분류순 정렬" << std::endl;
  std::cout << ">>> ";

  try
  {
    std::cin >> choiceSort;
    if (std::cin.fail()) 
    {
      std::cin.clear(); 
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
    }
    else if (choiceSort < 1 || choiceSort > 4)
    {
      throw std::invalid_argument("❌ 오류: 1~4 범위 내에서 입력하세요");
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  return choiceSort;
}

// 과목 목록에서 수정/제거 시 특정 conrse index를 입력 후 반환
int ConsoleUIManager::promptChoiceCourseIndex(const std::vector<Course::Course>& courses)
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

// 학기 내 과목 조회
void ConsoleUIManager::displayCoursesInSemester(Semester& s)
{
  std::vector<Course::Course>& courses = s.getCourses();
  
  if (courses.size() > 0)
  {
    std::cout << "\n--- " << s.getYear() << "학년 " << s.getSemester() << "학기 과목 목록 ---" << std::endl;
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
    std::cout << "\n--- " << s.getYear() << "학년 " << s.getSemester() << "학기 ---" << std::endl;
    std::cout << "  ❗ 이 학기에는 등록된 과목이 없습니다. ❗" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl; // 구분선
  }
}

void ConsoleUIManager::displayMessage(const std::string& message)
{
  std::cout << message << std::endl;
}

// Course 정보 출력
void displayCourse(const Course::Course& c)
{
  std::cout << "  - " << std::left << std::setw(50) << c.courseName // 과목명 (20칸 좌측 정렬)
            << " | 학점: " << std::setw(10) << c.credits       // 이수학점 (2칸)
            << " | 점수: " << std::setw(10) << Course::convertToGrade(c.grade) // 점수 (4칸)
            << " | 분류: " << std::left << std::setw(20) << Course::convertToCategory(c.category) // 분류 (10칸 좌측 정렬)
            << std::endl;
}



std::string ConsoleUIManager::promptFixString(const std::string& promptMessage)
{
  std::cout << promptMessage << std::endl;
  std::cout << "exit -> 입력 종료" << std::endl;
  std::string value;
  while (true)
  {
    try
    {
      std::cin >> value;
      if (std::cin.fail())
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
      }
      else if(value == "exit")
      {
        std::cout << "입력을 종료합니다" << std::endl;
        return "";
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << " 다시시도해주세요!" << '\n';
      continue;
    }
    break;
  }
  return value;
}
int ConsoleUIManager::promptFixInt(const std::string& promptMessage, int min, int max)
{
  std::cout << promptMessage << std::endl;
  std::cout << "-1 -> 입력 종료" << std::endl;
  int value;
  while (true)
  {
    try
    {
      std::cin >> value;
      if (std::cin.fail())
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
      }
      else if(value == -1)
      {
        std::cout << "입력을 종료합니다" << std::endl;
        return -1;
      }
      else if (value < min || value > max)
      {
        throw std::invalid_argument("❌ 오류: 범위 내에서 입력해주세요!!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << " 다시시도해주세요!" << '\n';
      continue;
    }
    break;
  }
  return value;
}
double ConsoleUIManager::promptFixDouble(const std::string& promptMessage, double min, double max)
{
  std::cout << promptMessage << std::endl;
  std::cout << "-1 -> 입력 종료" << std::endl;
  double value;
  while (true)
  {
    try
    {
      std::cin >> value;
      if (std::cin.fail())
      {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("❌ 오류: 입력값이 올바르지 않습니다!");
      }
      else if(value == -1)
      {
        std::cout << "입력을 종료합니다" << std::endl;
        return -1;
      }
      else if (value < min || value > max)
      {
        throw std::invalid_argument("❌ 오류: 범위 내에서 입력해주세요!!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << " 다시시도해주세요!" << '\n';
      continue;
    }
    break;
  }
  return value;
}

// 입력을 받아 Course를 생성 후 반환
Course::Course ConsoleUIManager::createAddCourse()
{
  std::string courseName; // 과목명
  int credits;            // 이수학점
  double grade;           // 최종점수
  int category;           // 전공분류

  std::cout << "\n--- 추가할 과목 데이터 입력 ---" << std::endl;

  courseName = promptFixString("🏷️ 과목명을 입력하세요 (예: 컴퓨터구조) >>> ");
  credits = promptFixInt("🔢 이수학점을 입력하세요 (예: 3) >>> ", 0, 3);
  grade = promptFixDouble("💯 최종점수를 입력하세요 (예: 1.0, 4.5) >>> ", 0.0, 4.5);
  category = promptFixInt("📚 전공 분류를 선택하세요:\n 1. 전공 필수    2. 복수 전공    3. 부전공   4. 계열   5. 교양  6. 자유 선택   7. 타 전공\n>>> ", 1, 7);
    
  Course::Course c = {courseName, credits, grade, --category};
  return c;
}



