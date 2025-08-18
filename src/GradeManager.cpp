#include "Semester.h"
#include "Course.h"
#include "GradeManager.h"
#include "SemesterJSON.h"

#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>

GradeManager::GradeManager() : semesters(semesterJson.loadJson()) 
{
  // semester 초기값 설정
  std::array<Semester, 8>& semesters = getSemesters();
  semester = &(semesters.at(0));
  course = &((semester->getCourses()).at(0));
}
GradeManager::~GradeManager() { semesterJson.saveJson(getSemesters()); }

void GradeManager::setSemesters(std::array<Semester, 8> semesters){ this->semesters = semesters; }
void GradeManager::setSelectSemester(Semester& semester){ this->semester = &semester; }
void GradeManager::setSelectCourse(Course::Course& course) { this->course = &course; }
Semester& GradeManager::getSelectSemester() { return *semester; }
Course::Course& GradeManager::getSelectCourse() { return *course; }

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

void GradeManager::handleAddCourse(Course::Course& newCourse) 
{
  semester->addCourses(newCourse);
}
void GradeManager::handleRemoveCourse()  
{
  // 기존에는 choiceSemester index번호로 Semester를 가져옴 -> GUI에서 Semester& 와 Course::Course&를 넘겨주어 사용
  std::vector<Course::Course>& courses = semester->getCourses();
  std::vector<Course::Course>::iterator it = std::find(courses.begin(), courses.end(), *course);
  
  std::string removeName = it->courseName;
  semester->removeCourses(it); // 실제 제거 부분

}
void GradeManager::handleFixCourse(Course::Course& fixCourse)  
{
  // c를 바탕으로 수정할 과목의 반복자를 찾음
  std::vector<Course::Course>& courses = semester->getCourses();
  std::vector<Course::Course>::iterator it = std::find(courses.begin(), courses.end(), *course);

  // 수정할 과목 값을 변경
  it->setCourseName(fixCourse.courseName);
  it->setCredits(fixCourse.credits);
  it->setGrade(fixCourse.grade);
  it->setCategory(fixCourse.category);
}
void GradeManager::handleSortCourse(int choiceSort)  
{
  std::vector<Course::Course>& courses = semester->getCourses();
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


std::string GradeManager::handleLoadJsonFile(HWND& hwnd)
{
    OPENFILENAME ofn;       // 공용 대화상자 구조체
    char szFile[260] = { 0, }; // 선택된 파일 경로를 저장할 버퍼

    // 구조체 초기화
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd; // 부모 윈도우 핸들
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);

    // JSON 파일만
    ofn.lpstrFilter = 
        "JSON 파일만 가능 (*.json)\0"   // 드롭다운에서 보여질 텍스트
        "*.json\0"                      // 실제로 매칭할 패턴
            "\0";                    
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST  | OFN_NOCHANGEDIR;

    // GetOpenFileNameA 함수는 ANSI 버전입니다.
    std::string filePath = "";
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        filePath = std::string(ofn.lpstrFile);
    }
    handleLoadJson(filePath);
    
    return "학기 데이터를 불러왔습니다";
}