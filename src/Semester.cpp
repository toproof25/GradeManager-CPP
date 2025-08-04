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
void Semester::removeCourses(std::vector<Course::Course>::iterator it)
{
  courses.erase(it);
}

double calculateGPA(const std::vector<Course::Course>& courses)
{
  if (courses.empty())
  {
    return 0.0;
  }

  double credits = 0.0;
  double grade = 0.0;

  for(const Course::Course& c : courses)
  {
    if (c.credits == 0 || c.grade == 0.0)
      continue;
    credits += c.credits;
    grade += (c.grade*c.credits);
  }

  if (grade == 0.0 || credits == 0.0)
    return 0.0;
  return grade / credits;
}








