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


void GradeManager::handleSelectSemesters()
{
  choiceSemester = consoleUIManager.displaySemesterChoice(semesters);
  if (choiceSemester == -1)
    menu = Menu::CourseSort;
  else
    menu = Menu::CourseChoise;
}
void GradeManager::handleSelectCourse()
{
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
}
void GradeManager::handleSortAllCourse()
{
  // 모든 과목을 담을 벡터 생성 후 가져오기
  std::vector<Course::Course> allCourse = allCourseVector();

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


void GradeManager::handleAddCourse() 
{
  Course::Course c;
  try
  {
    c = consoleUIManager.createAddCourse();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return;
  }
  semesters.at(choiceSemester).addCourses(c);
  semesterJson.createJsonData(choiceSemester, c);
  consoleUIManager.displayMessage("✅ [" + c.courseName + "] 과목이 성공적으로 추가되었습니다! ✅");
}
void GradeManager::handleRemoveCourse()  
{
  Semester& s = semesters.at(choiceSemester);
  const std::vector<Course::Course>& courses = s.getCourses();

  if (courses.size() <= 0)
  {
    consoleUIManager.displayMessage("\n❌ 제거할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌");
  }
  else
  {
    consoleUIManager.displayMessage("\n--- 제거할 과목을 선택하세요 ---");
    int removeIndex = consoleUIManager.promptChoiceCourseIndex(courses);
    std::string removeName = (courses.begin() + removeIndex)->courseName;
    semesters.at(choiceSemester).removeCourses(removeIndex); // 실제 제거 부분
    semesterJson.deleteJsonData(choiceSemester, removeIndex);
    consoleUIManager.displayMessage("\n✅ [" + removeName + "] 과목이 성공적으로 제거되었습니다! ✅");
  }
}
void GradeManager::handleFixCourse()  
{
  Semester& s = semesters.at(choiceSemester);
  std::vector<Course::Course>& courses = s.getCourses();

  if (courses.size() <= 0)
  {
    consoleUIManager.displayMessage("\n❌ 수정할 과목이 없습니다. 과목을 먼저 추가해주세요. ❌");
  }
  else
  {
    consoleUIManager.displayMessage("\n--- 수정할 과목을 선택하세요 ---");

    // 수정할 과목 Index를 설정
    int fixIndex = consoleUIManager.promptChoiceCourseIndex(courses);
    Course::Course& fixC = courses.at(fixIndex);

    // 수정할 과목 value를 설정
    int fixValue = consoleUIManager.promptFixValue(fixC);

    // 각 데이터 (과목명, 이수학점, 점수, 전공분류)에 맞는 타입을 수정
    if (fixValue == 1)
    {
      std::string fixName;
      fixName = consoleUIManager.promptFixString("🏷️ 변경할 과목명을 입력하세요 (예: 컴퓨터구조)");
      if (fixName == "") return;
      fixC.setCourseName(fixName);
      semesterJson.updateJsonData(choiceSemester, fixIndex, "courseName", fixName);
    }
    else if (fixValue == 2)
    {
      int fixCredits;
      fixCredits = consoleUIManager.promptFixInt("🔢 변경할 이수학점을 입력하세요 (예: 3)", 0, 3);
      if (fixCredits == -1) return;
      fixC.setCredits(fixCredits);
      semesterJson.updateJsonData(choiceSemester, fixIndex, "credits", fixCredits);
    }
    else if (fixValue == 3)
    {
      double fixGrade;
      fixGrade = consoleUIManager.promptFixDouble("💯 변경할 최종점수를 입력하세요 (예: 1.0, 4.5)", 0.0, 4.5);
      if (fixGrade == -1) return;
      fixC.setGrade(fixGrade);
      semesterJson.updateJsonData(choiceSemester, fixIndex, "grade", fixGrade);
    }
    else if (fixValue == 4)
    {
      int fixCategory;
      fixCategory = consoleUIManager.promptFixInt("📚 변경할 전공 분류를 선택하세요:\n 1.전공 선택  2.복수 전공   3.기초(필수)  4.일반(선택)  5.균형교양  6.계열교양  7.타전공", 1, 7);
      if (fixCategory == -1) return;
      fixC.setCategory(fixCategory);
      semesterJson.updateJsonData(choiceSemester, fixIndex, "category", fixCategory);
    }


    consoleUIManager.displayMessage("\n--- 수정된 과목의 정보 ---");
    consoleUIManager.displayCourse(fixC);
    consoleUIManager.displayMessage("----------------------------------------");
  }
}
void GradeManager::handleSortCourse()  
{
  Semester& s = semesters.at(choiceSemester);
  int choiceSort = consoleUIManager.displaySortChoice();
  sortCourse(s.getCourses(), choiceSort);
  semesterJson.sortJsonData(choiceSemester, choiceSort);
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

        break;
    }
  }
}
