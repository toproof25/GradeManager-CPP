#include "Semester.h"
#include "Course.h"
#include "ConsoleUIManager.h"
#include "GradeManager.h"
#include "SemesterJSON.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>

GradeManager::GradeManager() : semesters(semesterJson.loadJson()) {}
GradeManager::~GradeManager()
{
  semesterJson.saveJson(getSemesters());
}

void GradeManager::handleLoadJson(std::string& filePath)
{
  if (filePath.empty())
  {
    setSemesters(semesterJson.loadJson());
  }
  else
  {
    setSemesters(semesterJson.loadJson(filePath));
  }
}

void GradeManager::handleSaveJson() 
{
  semesterJson.saveJson(getSemesters());
}

void GradeManager::setSemesters(std::array<Semester, 8> semesters)
{
  this->semesters = semesters;
}

// 정렬할 과목 벡터와 정렬 번호를 통해 정렬
void GradeManager::sortCourse(std::vector<Course::Course>& courses, int choiceSort)
{
  switch(choiceSort)
  {
    case 1:
      if(isNameAscending) std::sort(courses.begin(), courses.end(), Course::courseNameCompareAscendingOrder);
      else std::sort(courses.begin(), courses.end(), Course::courseNameCompareDescendingOrder);
      isNameAscending = !isNameAscending;
      break;

    case 2:
      if(isCreditAscending) std::sort(courses.begin(), courses.end(), Course::courseCreditsCompareAscendingOrder);
      else std::sort(courses.begin(), courses.end(), Course::courseCreditsCompareDescendingOrder);
      isCreditAscending = !isCreditAscending;
      break;

    case 3:
      if(isGradeAscending) std::sort(courses.begin(), courses.end(), Course::courseGradeCompareAscendingOrder);
      else std::sort(courses.begin(), courses.end(), Course::courseGradeCompareDescendingOrder);
      isGradeAscending = !isGradeAscending;
      break;

    case 4:
      if(isCategoryAscending) std::sort(courses.begin(), courses.end(), Course::courseCategoryCompareAscendingOrder);
      else std::sort(courses.begin(), courses.end(), Course::courseCategoryCompareDescendingOrder);
      isCategoryAscending = !isCategoryAscending;
      break;
  }
}


void GradeManager::handleSelectSemesters()
{
  choiceSemester = consoleUIManager.displaySemesterChoice(semesters);
  if (choiceSemester == -1)
    menu = Menu::CourseSort;
  else if (choiceSemester == 8)
    menu = Menu::TotalGPA;
  else
    menu = Menu::CourseChoise;
}
void GradeManager::handleSelectCourse()
{
  /* 
  choiceCourse = consoleUIManager.displayCourseChoice(semesters[choiceSemester].getYear(), semesters[choiceSemester].getSemester());
  // 학기 선택으로 돌아가기
  if (choiceCourse == 0)
    menu = Menu::SemesterChoise;
  // 과목 조회
  else if (choiceCourse == 1) 
    consoleUIManager.displayCoursesInSemester(semesters.at(choiceSemester));
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
  // 총 학점 보기
  else if (choiceCourse == 6)
    handleGpaSemester();
  */
}
void GradeManager::handleSortAllCourse()
{
  // 모든 과목을 담을 벡터 생성 후 가져오기
  std::vector<Course::Course> allCourse = getAllCourseVector();

  consoleUIManager.displayMessage("*----------1학년 1학기 ~ 4학년 2학기 모든 과목 정렬전----------*\n");
  for (Course::Course& c : allCourse)
    consoleUIManager.displayCourse(c);

  int choiceSort = consoleUIManager.displaySortChoice();;
  sortCourse(allCourse, choiceSort);

  consoleUIManager.displayMessage("*----------1학년 1학기 ~ 4학년 2학기 모든 과목 정렬후----------*\n");
  for (Course::Course& c : allCourse)
    consoleUIManager.displayCourse(c);

  // 메인 메뉴로 돌아가기
  menu = Menu::SemesterChoise;
}


void GradeManager::handleAddCourse(Semester& s, Course::Course& newCourse) 
{
  std::vector<Course::Course>& courses = s.getCourses();
  s.addCourses(newCourse);
  //semesterJson.saveJson(getSemesters());
  consoleUIManager.displayMessage("✅ [" + newCourse.courseName + "] 과목이 성공적으로 추가되었습니다! ✅");
}

void GradeManager::handleRemoveCourse(Semester& s, Course::Course& c)  
{
  // 기존에는 choiceSemester index번호로 Semester를 가져옴 -> GUI에서 Semester& 와 Course::Course&를 넘겨주어 사용
  std::vector<Course::Course>& courses = s.getCourses();
  std::vector<Course::Course>::iterator it = std::find(courses.begin(), courses.end(), c);
  
  // 기존에는 index번호로 제거 -> GUI에서 반복자로 제거하도록 변경
  consoleUIManager.displayMessage("\n--- 제거할 과목을 선택하세요 ---");
  std::string removeName = it->courseName;
  s.removeCourses(it); // 실제 제거 부분
  //semesterJson.saveJson(getSemesters());
  consoleUIManager.displayMessage("\n✅ [" + removeName + "] 과목이 성공적으로 제거되었습니다! ✅");

}

void GradeManager::handleFixCourse(Semester& s, Course::Course& c, Course::Course& fixCourse)  
{
  // c를 바탕으로 수정할 과목의 반복자를 찾음
  std::vector<Course::Course>& courses = s.getCourses();
  std::vector<Course::Course>::iterator it = std::find(courses.begin(), courses.end(), c);

  if (courses.size() <= 0 || it == courses.end())
  {
    // GUI로 완전 변경 시 [수정] 버튼이 없으므로 해당 예외가 발생하지 않음 (일단 유지)
    consoleUIManager.displayMessage("\n❌ 수정할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌");
  }
  else
  {
    // 수정할 과목 값을 변경
    it->setCourseName(fixCourse.courseName);
    it->setCredits(fixCourse.credits);
    it->setGrade(fixCourse.grade);
    it->setCategory(fixCourse.category);
    
    // 수정된 후에 수정된 데이터를 바탕으로 저장
    //semesterJson.saveJson(getSemesters());

    consoleUIManager.displayMessage("\n✅ [" + it->courseName + "] 과목이 성공적으로 수정되었습니다! ✅");
  }

}
void GradeManager::handleSortCourse(Semester& s, int choiceSort)  
{
  //Semester& s = semesters.at(choiceSemester);
  //int choiceSort = consoleUIManager.displaySortChoice();
  sortCourse(s.getCourses(), choiceSort);
  //semesterJson.sortJsonData(choiceSemester, choiceSort);
}
void GradeManager::handleGpaSemester()
{
  double GPA = calculateGPA(semesters.at(choiceSemester).getCourses());
  consoleUIManager.displayMessage("총 학점은 [" + std::to_string(GPA) + "] 입니다.");
}

// 전체 학기 array에서 모든 과목을 추출하고 저장한 vector를 반환
std::vector<Course::Course> GradeManager::getAllCourseVector()
{
  std::vector<Course::Course> allCourse;
  for (Semester& s : semesters)
  {
    for (Course::Course& c : s.getCourses())
      allCourse.push_back(c);
  }
  return allCourse;
} 

void GradeManager::run()
{
  while (true)
  {
    switch(menu)
    {
      case Menu::SemesterChoise:
        handleSelectSemesters();
        break;

      case Menu::CourseChoise:
        handleSelectCourse();
        break;

      case Menu::CourseSort:
        handleSortAllCourse();

      case Menu::TotalGPA:
        std::vector<Course::Course> c = getAllCourseVector();
        double GPA = calculateGPA(c);
        consoleUIManager.displayMessage("모든 학기 총 학점은 [" + std::to_string(GPA) + "] 입니다.");
        menu = Menu::SemesterChoise;
        break;
    }
  }
}
