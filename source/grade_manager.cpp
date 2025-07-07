#include <iostream>
#include <stdexcept>
#include <limits>
#include <array>
#include <vector>
#include <iomanip>

#include "Course.h" // 과목 구조체
#include "Semester.h" // 학기 클래스

void choiseReturnSemester(std::array<Semester, 8>& semesters ,int& choiceSemester);
void selectSemester(std::array<Semester, 8>& semesters ,int& choiceSemester, int& menu);
void fixCourses(std::vector<Course::Course>& courses);
int choiceFixValue(Course::Course& fixC);
void selectCourse(std::array<Semester, 8>& semesters ,int& choiceSemester, int& choiceCourse, int& menu);



enum Menu // 메뉴
{
  SemesterChoise   = 1, // 학기 선택
  CourseChoise     = 2, // 학기 내 과목
};



// 학기 목록 중 선택
void choiseReturnSemester(std::array<Semester, 8>& semesters ,int& choiceSemester)
{
  while(true)
  {
    std::cout << "\n--- 학기를 선택하세요 ---" << std::endl;

    for (int i=0; i<semesters.size(); ++i)
    {
      std::cout << i+1 << ". ";
      semesters[i].semesterInfo();
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
      else if (choiceSemester < 1 || choiceSemester > 8)
      {
        throw std::invalid_argument("❌ 오류: 1~8 범위 내에서 입력하세요");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  }

  choiceSemester--;
}

// 학기 선택 메뉴 함수
void selectSemester(std::array<Semester, 8>& semesters ,int& choiceSemester, int& menu)
{
  choiseReturnSemester(semesters, choiceSemester);
  menu = Menu::CourseChoise;
}

// 과목 목록에서 하나 선택 후 반환
int choiceFixValue(Course::Course& fixC)
{
  int choiceFixValue;
  std::cout << "\n--- 수정할 과목의 정보 ---" << std::endl;
  Course::printCourse(fixC);
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

// 과목 수정 - 클래스 내부가 아닌 외부 함수로 구현하여 클래스 책임을 줄임
void fixCourses(std::vector<Course::Course>& courses)
{
  if (courses.size() <= 0)
  {
    std::cout << "\n❌ 제거할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌" << std::endl;
  }
  else
  {
    std::cout << "\n--- 수정할 과목을 선택하세요 ---" << std::endl;

    // 수정할 과목 Index를 설정
    int fixIndex = getChoiceCourseIndex(courses);
    Course::Course& fixC = courses.at(fixIndex);

    // 수정할 과목 value를 설정
    int fixValue = choiceFixValue(fixC);

    // 각 데이터 (과목명, 이수학점, 점수, 전공분류)에 맞는 타입을 수정
    if (fixValue == 1)
    {
      std::cout << "변경할 과목명을 입력하세요 >>> ";
      std::string fixName;
      std::cin >> fixName;
      Course::setCourseName(fixC, fixName);
    }
    else if (fixValue == 2)
    {
      std::cout << "변경할 이수학점을 입력하세요 >>> ";
      int fixCredits;
      std::cin >> fixCredits;
      Course::setCredits(fixC, fixCredits);
    }
    else if (fixValue == 3)
    {
      std::cout << "변경할 점수를 입력하세요 >>> ";
      double fixGrade;
      std::cin >> fixGrade;
      Course::setGrade(fixC, fixGrade);
    }
    else if (fixValue == 4)
    {
      std::cout << "변경할 전공분류를 입력하세요 >>> ";
      int fixCategory;
      std::cin >> fixCategory;
      Course::setCategory(fixC, fixCategory);
    }

    std::cout << "\n--- 수정된 과목의 정보 ---" << std::endl;
    Course::printCourse(fixC);
    std::cout << "----------------------------------------" << std::endl;
  }
}

// 과목 메뉴 선택
void selectCourse(std::array<Semester, 8>& semesters ,int& choiceSemester, int& choiceCourse, int& menu)
{
  std::cout << "\n--- 메뉴를 선택하세요 ---" << std::endl;
  std::cout << "0. 학기 선택" << std::endl;
  std::cout << "1. 과목 조회" << std::endl;
  std::cout << "2. 과목 추가" << std::endl;
  std::cout << "3. 과목 제거" << std::endl;
  std::cout << "4. 과목 수정" << std::endl;
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
    else if (choiceCourse < 0 || choiceCourse > 4)
    {
      throw std::invalid_argument("❌ 오류: 0~4 범위 내에서 입력하세요");
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return;
  }
  
  
  if (choiceCourse == 0)
  {
    menu = Menu::SemesterChoise;
  }

  else if (choiceCourse == 1) 
  {
    semesters.at(choiceSemester).printCourses();
  }

  else if (choiceCourse == 2) 
  {
    Course::Course c = Course::getAddCourse();
    semesters.at(choiceSemester).addCourses(c);
    std::cout << "✅ [" << c.courseName << "] 과목이 성공적으로 추가되었습니다! ✅" << std::endl;
  }

  else if (choiceCourse == 3) 
  {
    Semester& s = semesters.at(choiceSemester);
    const std::vector<Course::Course>& courses = s.getCourses();

    if (courses.size() <= 0)
    {
      std::cout << "\n❌ 제거할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌" << std::endl;
    }
    else
    {
      std::cout << "\n--- 제거할 과목을 선택하세요 ---" << std::endl;
      int removeIndex = getChoiceCourseIndex(courses);
      std::string removeName = (courses.begin() + removeIndex)->courseName;
      semesters.at(choiceSemester).removeCourses(removeIndex); // 실제 제거 부분
      std::cout << "\n✅ [" << removeName << "] 과목이 성공적으로 제거되었습니다! ✅" << std::endl;
    }
  }

  else if (choiceCourse == 4)
  {
    Semester& s = semesters.at(choiceSemester);
    std::vector<Course::Course>& courses = s.getCourses();
    fixCourses(courses);
  }
}





int main()
{
  // 1학년 1학기 ~ 4학년 2학기
  std::array<Semester, 8> semesters = {
    Semester(1, 1), // 1학년 1학기
    Semester(1, 2), // 1학년 2학기 

    Semester(2, 1), // 2학년 1학기 
    Semester(2, 2), // 2학년 2학기 

    Semester(3, 1), // 3학년 1학기 
    Semester(3, 2), // 3학년 2학기 

    Semester(4, 1), // 4학년 1학기 
    Semester(4, 2), // 4학년 2학기 
  };

  // 메뉴 선택, 학기 선택, 과목 선택 변수
  int menu = 1;
  int choiceSemester = 1;
  int choiceCourse = 1;

  std::cout << "*----------학점 관리 프로그램----------*" << std::endl;

  while (true)
  {
    switch(menu)
    {
      case Menu::SemesterChoise:
        selectSemester(semesters, choiceSemester, menu);
        break;

      case Menu::CourseChoise:
        selectCourse(semesters, choiceSemester, choiceCourse, menu);
        break;
    }
  }

  return 0;
}


