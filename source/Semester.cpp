#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept> // 예외가 정의된 헤더파일
#include <limits>

#include "Course.h"
#include "Semester.h"



// 과목 추가
void Semester::addCourses(Course::Course c)
{
  courses.push_back(c);
}

// 과목 제거
void Semester::removeCourses(int removeIndex)
{
  courses.erase(courses.begin() + removeIndex);
}









