#include <iostream>
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>
#include <array>
#include <vector>

// 전공 분류
enum MajorClassification
{
  MajorRequired   = 0,  // 전선 (전공 필수 - Major Required)
  DoubleMajor     = 1,  // 복전 (복수 전공 - Double Major)
  Minor           = 2,  // 부전 (부전공 - Minor)
  Division        = 3,  // 계열 (계열 필수/선택 - Division)
  GeneralElective = 4,  // 교양 (교양 - General Elective)
  Elective        = 5,  // 자선 (자유 선택 - Elective)
  OtherMajor      = 6   // 타전 (타 전공 - Other Major)
};



/* ---------- 과목 구조체, 사용자 정의 규칙 정렬, 숫자점수->문자점수 변환 ---------- */
struct Course
{
  std::string courseName; // 컴퓨터구조
  int credits;            // 3 (학점)
  double grade;           // 4.5 (A+)
  int category;           // 0 (전공선택)

  // 정보 출력
  void printCourse()
  {
    std::cout << "과목명 : " + courseName << ", 이수학점 : " << credits << ", 점수 : " << convertToGrade() << ", 분류 : " << convertToCategory() << std::endl;
  }
  std::string convertToGrade();
  std::string convertToCategory();

	bool operator<(const Course& otherCourse) const 
  {
	    return grade < otherCourse.grade;
	}
};
std::string Course::convertToGrade()
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
std::string Course::convertToCategory()
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

bool courseCompare(const Course& lhs, const Course& rhs)
{
  return lhs.grade < rhs.grade;
}



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
      std::cout << year << "학년 " << semester << "학기 데이터가 생성되었습니다" << std::endl;
    }

    
    void printCourses();    // 모든 과목 조회
    void addCourses();      // 과목 추가
    void removeCourses();
    

};

// 과목 조회
void Semester::printCourses()
{
  if (courses.size() > 0)
  {
    std::cout << year << "학년 " << semester << "학기 과목을 조회합니다" << std::endl;
    for (std::vector<Course>::iterator it = courses.begin(); it != courses.end(); it++)
    {
      it->printCourse();
    }
  }
  else
  {
    std::cout << year << "학년 " << semester << "학기에는 조회할 과목이 없습니다" << std::endl;
  }
}

// 과목 추가
void Semester::addCourses()
{
  std::string courseName; // 과목명
  int credits;            // 이수학점
  double grade;           // 최종점수
  int category;           // 전공분류

  // 과목명 입력
  while (true)
  {
    try
    {
      std::cout << "과목명을 입력하세요 ex) 컴퓨터구조 >>> ";
      std::cin >> courseName;
    
      if (std::cin.fail()) 
      {
        std::cin.clear(); // 입력 스트림의 상태를 초기화
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
        throw std::invalid_argument("입력값이 이상합니다!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  };

  // 이수학점 정수 입력
  while (true)
  {
    try
    {
      std::cout << "이수학점을 입력하세요 ex) 3 >>> ";
      std::cin >> credits;

      if (std::cin.fail()) 
      {
        std::cin.clear(); // 입력 스트림의 상태를 초기화
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
        throw std::invalid_argument("숫자를 입력해주세요!");
      }
      else if (credits < 0 || credits > 3)
      {
      throw std::invalid_argument("0~3학점 범위 내에서 입력해주세요!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  };

    
  // 최종점수 실수 입력
  while (true)
  {
    try
    {
      std::cout << "최종점수를 입력하세요 ex) 1.0, 4.5 >>> ";
      std::cin >> grade;

      if (std::cin.fail()) 
      {
        std::cin.clear(); // 입력 스트림의 상태를 초기화
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
        throw std::invalid_argument("실수를 입력해주세요!");
      }
      else if (grade < 0.0 || grade > 4.5)
      {
      throw std::invalid_argument("0.0~4.5점수 범위 내에서 입력해주세요!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  };

    
  // 전공분류 정수 입력
  while (true)
  {
    try
    {
      std::cout << "전송분류를 선택하세요" << std::endl;
      std::cout << "1.전공선택  2.복수전공  3.부전공  4.계열  5.교양  6.자선  7.타전공\n>>> ";
      std::cin >> category;

      if (std::cin.fail()) 
      {
        std::cin.clear(); // 입력 스트림의 상태를 초기화
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 버퍼 비우기
        throw std::invalid_argument("숫자를 입력해주세요!");
      }
      else if (category <= 0 || category >=8)
      {
      throw std::invalid_argument("1~7숫자 범위 내에서 입력해주세요!");
      }
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    break;
  };

  Course c = {courseName, credits, grade, --category};
  courses.push_back(c);
  std::cout << "[" << courses.back().courseName << "] 과목을 성공적으로 추가하였습니다" << std::endl;
}

// 과목 제거
void Semester::removeCourses()
{
  if (courses.size() <= 0)
  {
    std::cout << "제거할 과목이 없습니다" << '\n';
  }
  else
  {
    int choiceRemoveCourse;

    std::cout << "----------제거할 과목을 선택하세요----------" << '\n'; 
    int i=1;
    for (const Course& c : courses)
    {
      std::cout << i << ". " << c.courseName << '\n';
    }

    // 제거할 과목 선택
    while (true)
    {
      try
      {
        std::cin >> choiceRemoveCourse;

        if (std::cin.fail()) 
        {
          std::cin.clear(); 
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          throw std::invalid_argument("숫자를 입력해주세요");
        }
        else if (choiceRemoveCourse < 1 || choiceRemoveCourse > courses.size())
        {
          throw std::invalid_argument("제시된 범위에서 선택하세요");
        }
      }
      catch(const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        continue;
      }
      break;
    };

    --choiceRemoveCourse;
    std::string removeName = (courses.begin() + choiceRemoveCourse)->courseName;
    courses.erase(courses.begin() + choiceRemoveCourse);
    std::cout << "[" << removeName << "] 과목이 성공적으로 제거되었습니다" << '\n';
  }
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


  int choice;
  while (true)
  {
    std::cout << "선택 1. 조회, 2. 추가, 3. 제거" << '\n' << ">>> ";
    std::cin >> choice;
    
    switch (choice)
    {
    case 1:
      semesters[0].printCourses();
      break;

    case 2:
      semesters[0].addCourses();
      break;
    case 3:

      semesters[0].removeCourses();
      break;
    
    default:
      break;
    }
  }
  
  semesters[0].addCourses();
  semesters[0].printCourses();

  

  Course c1 = {"컴퓨터구조", 3, 1.0, MajorRequired};
  c1.printCourse();

  return 0;
}


