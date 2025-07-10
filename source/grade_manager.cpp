#include <iostream>
#include <stdexcept>
#include <limits>
#include <array>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "Course.h" // 과목 구조체
#include "Semester.h" // 학기 클래스



enum Menu // 메뉴
{
  MainChoise       = 0, // 학기 선택 or 모든 과목 정렬
  SemesterChoise   = 1, // 학기 선택
  CourseChoise     = 2, // 학기 내 과목
  CourseSort       = 3, // 모든 과목 조회 및 정렬
};

class GradeManager
{
  private:
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

  public:
    GradeManager()
    {
      std::cout << "*----------학점 관리 프로그램----------*" << std::endl;
    }

    void printCourses(Semester& s);
    int choiceFixValue(const Course::Course& fixC);
    int getChoiceCourseIndex(const std::vector<Course::Course>& courses); // 과목 목록에서 선택한 couesrs index를 받아오는 함수
    void fixCourses(std::vector<Course::Course>& courses);
    void sortCourse(std::vector<Course::Course>& courses, int choiceSort);
    void selectSemester();
    void selectCourse();
    void handleAddCourse();
    void handleRemoveCourse();
    void handleFixCourse();
    void handleSortCourse();
    std::vector<Course::Course> allCourseVector();

    void run()
    {
      while (true)
      {
        switch(menu)
        {
          case Menu::SemesterChoise:
            selectSemester();
            break;

          case Menu::CourseChoise:
            selectCourse();
            break;

          case Menu::CourseSort:
            // 모든 과목을 담을 벡터 생성 후 가져오기
            std::vector<Course::Course> allCourse = allCourseVector();

            std::cout << "*----------1학년 1학기 ~ 4학년 2학기 모든 과목 정렬전----------*\n" << std::endl;
            for (Course::Course& c : allCourse)
              Course::printCourse(c);

            int choiceSort;
            std::cout << "1. 과목 이름순 정렬" << std::endl;
            std::cout << "2. 과목 학점순 정렬" << std::endl;
            std::cout << "3. 과목 점수순 정렬" << std::endl;
            std::cout << "4. 과목 분류순 정렬" << std::endl;
            std::cout << ">>> ";
            std::cin >> choiceSort;

            sortCourse(allCourse, choiceSort);

            std::cout << "*----------1학년 1학기 ~ 4학년 2학기 모든 과목 정렬후----------*\n" << std::endl;
            for (Course::Course& c : allCourse)
              Course::printCourse(c);

            // 메인 메뉴로 돌아가기
            menu = Menu::SemesterChoise;

            break;
        }
      }
    }

};


// 학기 내 과목 조회
void GradeManager::printCourses(Semester& s)
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

// 수정할 과목 목록에서 하나 선택 후 반환
int GradeManager::choiceFixValue(const Course::Course& fixC)
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

// 과목 목록에서 수정/제거 시 특정 conrse index를 입력 후 반환
int GradeManager::getChoiceCourseIndex(const std::vector<Course::Course>& courses)
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

// 과목 수정 - 클래스 내부가 아닌 외부 함수로 구현하여 클래스 책임을 줄임
void GradeManager::fixCourses(std::vector<Course::Course>& courses)
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

// 정렬할 과목 벡터와 정렬 번호를 통해 정렬
void GradeManager::sortCourse(std::vector<Course::Course>& courses, int choiceSort)
{
  if (choiceSort == 1)
    std::sort(courses.begin(), courses.end(), Course::courseNameCompare);
  else if (choiceSort == 2)
    std::sort(courses.begin(), courses.end(), Course::courseCreditsCompare);
  else if (choiceSort == 3)
    std::sort(courses.begin(), courses.end(), Course::courseGradeCompare);
  else if (choiceSort == 4)
    std::sort(courses.begin(), courses.end(), Course::courseCategoryCompare);
}

// 학기 선택 메뉴 함수
void GradeManager::selectSemester()
{
  while(true)
  {
    std::cout << "\n--- 학기를 선택하세요 ---" << std::endl;

    std::cout << "0. 모든 과목 조회 및 정렬 조회" << std::endl;
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
    menu = Menu::CourseSort;
  }
  else{
    menu = Menu::CourseChoise;
    choiceSemester--;
  }
}


void GradeManager::handleAddCourse() 
{
  Course::Course c = Course::getAddCourse();
  semesters.at(choiceSemester).addCourses(c);
  std::cout << "✅ [" << c.courseName << "] 과목이 성공적으로 추가되었습니다! ✅" << std::endl;
}
void GradeManager::handleRemoveCourse()  
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
void GradeManager::handleFixCourse()  
{
  Semester& s = semesters.at(choiceSemester);
  std::vector<Course::Course>& courses = s.getCourses();
  fixCourses(courses);
}
void GradeManager::handleSortCourse()  
{
  Semester& s = semesters.at(choiceSemester);
  
  int choiceSort;
  std::cout << "1. 과목 이름순 정렬" << std::endl;
  std::cout << "2. 과목 학점순 정렬" << std::endl;
  std::cout << "3. 과목 점수순 정렬" << std::endl;
  std::cout << "4. 과목 분류순 정렬" << std::endl;
  std::cout << ">>> ";
  std::cin >> choiceSort;

  sortCourse(s.getCourses(), choiceSort);
}


// 과목 메뉴 선택
void GradeManager::selectCourse()
{
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
    return;
  }
  
  // 학기 선택으로 돌아가기
  if (choiceCourse == 0)
    menu = Menu::SemesterChoise;
  // 과목 조회
  else if (choiceCourse == 1) 
    printCourses(semesters.at(choiceSemester));
  // 과목 추가
  else if (choiceCourse == 2) 
    handleAddCourse();
  // 과목 제거
  else if (choiceCourse == 3) 
    handleRemoveCourse();
  // 과목 수정
  else if (choiceCourse == 4)
    handleFixCourse();
  // 과목 정렬
  else if (choiceCourse == 5)
    handleSortCourse();
}


// 전체 학기 array에서 모든 과목을 추출하고 저장한 vector를 반환
std::vector<Course::Course> GradeManager::allCourseVector()
{
  std::vector<Course::Course> allCourse;
  for (Semester& s : semesters)
  {
    for (Course::Course& c : s.getCourses())
      allCourse.push_back(c);
  }
  return allCourse;
} 

int main()
{
  GradeManager gm;
  gm.run();

  return 0;
}


