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

double calculateGPA(const std::vector<Course::Course>& courses)
{
  double credits = 0;
  double grade = 0;

  for(const Course::Course& c : courses)
  {
    credits += c.credits;
    grade += (c.grade*c.credits);
  }
  return grade / credits;
}








