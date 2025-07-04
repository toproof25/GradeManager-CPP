#include <iostream>
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>
#include <array>
#include <vector>

#include "Course.h" // 과목 구조체

// 출력 결과 std::cout부분의 경우 gemini로 작성 (보다 직관적이로 이쁘게 출력하게 하기 위함)
#include <iomanip>      // <--- 이 부분 추가해야 setw, left 등 사용 가능



/* ---------- 학기 클래스 ---------- */
class Semester
{
  private:
    int year;
    int semester; 
    std::vector<Course> courses;

  public:
    Semester(int year, int semester) : year(year), semester(semester) 
    {
      //std::cout << "✨ " << year << "학년 " << semester << "학기 데이터가 생성되었습니다! ✨" << std::endl;
    }
    
    void semesterInfo()
    {
      std::cout << year << "학년 " << semester << "학기 " << std::endl;
    }

    void printCourses();    // 모든 과목 조회
    void addCourses();      // 과목 추가
    void removeCourses();
    void fixCourses();

};

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

    for (std::vector<Course>::iterator it = courses.begin(); it != courses.end(); it++)
    {
      printCourse(*it);
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
  std::string courseName; // 과목명
  int credits;            // 이수학점
  double grade;           // 최종점수
  int category;           // 전공분류

  std::cout << "\n--- 새 과목 추가 ---" << std::endl;

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
    std::cout << "  [번호] 과목명" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    int choiceRemoveCourse;
    int i=1;
    for (const Course& c : courses)
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

    --choiceRemoveCourse;
    std::string removeName = (courses.begin() + choiceRemoveCourse)->courseName;
    courses.erase(courses.begin() + choiceRemoveCourse);
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
    std::cout << "  [번호] 과목명" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    int choiceFixCourse;
    int i=1;
    for (const Course& c : courses)
    {
      std::cout << i++ << ". " << c.courseName << '\n';
    }
    std::cout << "----------------------------------------\n>>> " << std::endl;
    std::cin >> choiceFixCourse;

    int choiceFixValue;
    Course& fixC = courses.at(--choiceFixCourse);

    std::cout << "\n--- 수정할 과목의 정보 ---" << std::endl;
    printCourse(fixC);
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "수정할 데이터를 선택하세요" << std::endl;
    std::cout << "1. 과목명\n2. 이수학점\n3. 점수\n4. 전공분류\n>>> ";

    std::cin >> choiceFixValue;

    if (choiceFixValue == 1)
    {
      std::cout << "변경할 과목명을 입력하세요 >>> ";
      std::string fixName;
      std::cin >> fixName;
      fixC.courseName = fixName;
    }
    else if (choiceFixValue == 2)
    {
      std::cout << "변경할 이수학점을 입력하세요 >>> ";
      int fixCredits;
      std::cin >> fixCredits;
      fixC.credits = fixCredits;
    }
    else if (choiceFixValue == 3)
    {
      std::cout << "변경할 점수를 입력하세요 >>> ";
      double fixGrade;
      std::cin >> fixGrade;
      fixC.grade = fixGrade;
    }
    else if (choiceFixValue == 4)
    {
      std::cout << "변경할 전공분류를 입력하세요 >>> ";
      int fixCategory;
      std::cin >> fixCategory;
      fixC.category = fixCategory;
    }
    else
    {
      std::cout << "잘못입력했습니다..." << std::endl;
    }

    std::cout << "\n--- 수정된 과목의 정보 ---" << std::endl;
    printCourse(fixC);
    std::cout << "----------------------------------------" << std::endl;
  }
}


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
    std::cin >> choiceSemester;
    
    if (choiceSemester < 1 || choiceSemester > 8)
      continue;
    break;
  }

  choiceSemester--;
}



int main()
{
  std::cout << "*----------학점 관리 프로그램----------*" << std::endl;

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


  int menu = 1;
  int choiceSemester;
  int choiceCourse;

  while (true)
  {
    if (menu == 1) // 학기 선택
    {
      choiseReturnSemester(semesters, choiceSemester);
      menu = 2;
    }
    else if (menu == 2) // 학기 관리
    {
      std::cout << "\n--- 메뉴를 선택하세요 ---" << std::endl;
      std::cout << "0. 학기 선택" << std::endl;
      std::cout << "1. 과목 조회" << std::endl;
      std::cout << "2. 과목 추가" << std::endl;
      std::cout << "3. 과목 제거" << std::endl;
      std::cout << "4. 과목 수정" << std::endl;
      std::cout << "-----------------------" << std::endl;
      std::cout << ">>> ";
      std::cin >> choiceCourse;
      
      if (choiceCourse == 0)
      {
        menu = 1;
      }
      else if (choiceCourse == 1) 
      {
        semesters.at(choiceSemester).printCourses();
      }
      else if (choiceCourse == 2) 
      {
        semesters.at(choiceSemester).addCourses();
      }
      else if (choiceCourse == 3) 
      {
        semesters.at(choiceSemester).removeCourses();
      }
      else if (choiceCourse == 4)
      {
        semesters.at(choiceSemester).fixCourses();
      }
      else
      {
        std::cout << "\n프로그램을 종료합니다. 안녕히 계세요! 👋" << std::endl;
        return 0;
      }  
    }
  }


  return 0;
}


