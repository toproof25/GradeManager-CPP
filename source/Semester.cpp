#include <vector>
#include "Course.h"
#include "Semester.h"

Semester::Semester(int year, int semester) : year(year), semester(semester) {}

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









