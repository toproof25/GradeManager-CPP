#include <iostream>
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>
#include <array>
#include <vector>

#include "Course.h" // 과목 구조체
#include "Semester.h" // 과목 구조체

// 출력 결과 std::cout부분의 경우 gemini로 작성 (보다 직관적이로 이쁘게 출력하게 하기 위함)
#include <iomanip>      // <--- 이 부분 추가해야 setw, left 등 사용 가능


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


