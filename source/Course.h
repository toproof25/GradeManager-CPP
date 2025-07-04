#pragma once // 헤더 중복되지 않게 하는 설정
#include <string>

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

// Course는 과목에 대한 정보를 관리하는 책임을 진다 (저장, 제공)
struct Course
{
  std::string courseName; // 컴퓨터구조
  int credits;            // 3 (학점)
  double grade;           // 4.5 (A+)
  int category;           // 0 (전공선택)

	bool operator<(const Course& otherCourse) const 
  {
	    return grade < otherCourse.grade;
	}
};


void printCourse(const Course& c);
std::string convertToGrade(int grade);
std::string convertToCategory(int category);
bool courseCompare(const Course& lhs, const Course& rhs);


