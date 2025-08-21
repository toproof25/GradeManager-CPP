#include "gtest/gtest.h" // Google Test 헤더 파일
#include "Semester.h"     
#include "Course.h"   

#include <string>
#include <algorithm>


/*
Arrange : Course 객체 생성
Act     : 생성된 객체의 데이터가 일치하는지 확인
Assert  : 
    - Course 생성 시 넣은 값이 일치하는지 검증
*/
TEST(CourseValueTest, ConstructorInitializesCorrectly)
{
  Course::Course c = {"test", 3, 4.0, 7};
  ASSERT_EQ("test", c.courseName);
  ASSERT_EQ(3, c.credits);
  ASSERT_DOUBLE_EQ(4.0, c.grade);
  ASSERT_EQ(7, c.category);
}



/*
Arrange : Course 객체 생성
Act     : 생성된 Course의 각 setter함수로 데이터 변경
Assert  : 
    - setter함수로 데이터가 잘 변경되었는지 확인
*/
TEST(CourseSetterTest, Setter_CorrectValueWhenChangingData)
{
  Course::Course c = {"test", 3, 4.0, 7};
  
  c.setCourseName("changeName");
  c.setCredits(1);
  c.setGrade(0.5);
  c.setCategory(1);

  ASSERT_EQ("changeName", c.courseName);
  ASSERT_EQ(1, c.credits);
  ASSERT_DOUBLE_EQ(0.5, c.grade);
  ASSERT_EQ(1, c.category);
}


/*
Arrange : 
Act     : doubleToGradeString함수로 점수 변환
Assert  : 
    - 각 점수에 대해 올바르게 변환하는지 검증
*/
TEST(CourseConvertTest, doubleToGradeString_CorrectValueForScoreConversion)
{
  ASSERT_EQ("OVER", Course::doubleToGradeString(4.6));
  ASSERT_EQ("A+",   Course::doubleToGradeString(4.5));
  ASSERT_EQ("A",    Course::doubleToGradeString(4.0));
  ASSERT_EQ("B+",   Course::doubleToGradeString(3.5));
  ASSERT_EQ("B",    Course::doubleToGradeString(3.0));
  ASSERT_EQ("C+",   Course::doubleToGradeString(2.5));
  ASSERT_EQ("C",    Course::doubleToGradeString(2.0));
  ASSERT_EQ("D+",   Course::doubleToGradeString(1.5));
  ASSERT_EQ("D",    Course::doubleToGradeString(1.0));
  ASSERT_EQ("P",    Course::doubleToGradeString(0.0));
  ASSERT_EQ("NP",   Course::doubleToGradeString(-1.0));
  ASSERT_EQ("F",    Course::doubleToGradeString(-2.0));
  ASSERT_EQ("ERROR",Course::doubleToGradeString(-10.0));
}

/*
Arrange : 
Act     : stringToGradeDouble함수로 문자열 입력 시 점수로 변환되는지 확인
Assert  : 
    - 각 점수 문자열에 대해 올바르게 변환하는지 검증
*/
TEST(CourseConvertTest, stringToGradeDouble_CorrectValueForScoreConversion)
{
  ASSERT_DOUBLE_EQ(4.5,  Course::stringToGradeDouble("A+"));
  ASSERT_DOUBLE_EQ(4.0,  Course::stringToGradeDouble("A"));
  ASSERT_DOUBLE_EQ(3.5,  Course::stringToGradeDouble("B+"));
  ASSERT_DOUBLE_EQ(3.0,  Course::stringToGradeDouble("B"));
  ASSERT_DOUBLE_EQ(2.5,  Course::stringToGradeDouble("C+"));
  ASSERT_DOUBLE_EQ(2.0,  Course::stringToGradeDouble("C"));
  ASSERT_DOUBLE_EQ(1.5,  Course::stringToGradeDouble("D+"));
  ASSERT_DOUBLE_EQ(1.0,  Course::stringToGradeDouble("D"));
  ASSERT_DOUBLE_EQ(0.0,  Course::stringToGradeDouble("P"));
  ASSERT_DOUBLE_EQ(-1.0, Course::stringToGradeDouble("NP"));
  ASSERT_DOUBLE_EQ(-2.0, Course::stringToGradeDouble("F"));
  ASSERT_DOUBLE_EQ(0.0,  Course::stringToGradeDouble("ERROR"));
}


/*
Arrange : 
Act     : categoryToString함수로 전공분류를 문자열로 변환
Assert  : 
    - 올바르게 변환하는지 검증
*/
TEST(CourseConvertTest, categoryToString_CorrectValueForScoreConversion)
{
  ASSERT_EQ("기초",     Course::categoryToString(Course::BasicRequired));
  ASSERT_EQ("일반",     Course::categoryToString(Course::GeneralElective));
  ASSERT_EQ("균형",     Course::categoryToString(Course::Balance));
  ASSERT_EQ("계열",     Course::categoryToString(Course::Division));
  ASSERT_EQ("학부기초", Course::categoryToString(Course::DepartmentBasic));
  ASSERT_EQ("전공필수", Course::categoryToString(Course::MajorRequired));
  ASSERT_EQ("전공선택", Course::categoryToString(Course::MajorElective));
  ASSERT_EQ("복전선택", Course::categoryToString(Course::DoubleMajorElective));
  ASSERT_EQ("부전기초", Course::categoryToString(Course::MinorBasic));
  ASSERT_EQ("부전선택", Course::categoryToString(Course::MinorElective));
  ASSERT_EQ("자선",     Course::categoryToString(Course::FreeElective));
  ASSERT_EQ("타전",     Course::categoryToString(Course::OtherMajor));
  ASSERT_EQ("ERROR",    Course::categoryToString(99));
}

/*
Arrange : 
Act     : categoryToString함수 전공분류 문자열 입력 시 올바른 정수로 변환되는지 검증
Assert  : 
    - 올바르게 변환하는지 검증
*/
TEST(CourseConvertTest, stringToCategory_CorrectValueForScoreConversion)
{
  ASSERT_EQ(Course::BasicRequired,       Course::stringToCategory("기초"));
  ASSERT_EQ(Course::GeneralElective,     Course::stringToCategory("일반"));
  ASSERT_EQ(Course::Balance,             Course::stringToCategory("균형"));
  ASSERT_EQ(Course::Division,            Course::stringToCategory("계열"));
  ASSERT_EQ(Course::DepartmentBasic,     Course::stringToCategory("학부기초"));
  ASSERT_EQ(Course::MajorRequired,       Course::stringToCategory("전공필수"));
  ASSERT_EQ(Course::MajorElective,       Course::stringToCategory("전공선택"));
  ASSERT_EQ(Course::DoubleMajorElective, Course::stringToCategory("복전선택"));
  ASSERT_EQ(Course::MinorBasic,          Course::stringToCategory("부전기초"));
  ASSERT_EQ(Course::MinorElective,       Course::stringToCategory("부전선택"));
  ASSERT_EQ(Course::FreeElective,       Course::stringToCategory("자선"));
  ASSERT_EQ(Course::OtherMajor,       Course::stringToCategory("타전"));
  ASSERT_EQ(-1,                          Course::stringToCategory("ERROR"));
}




// 테스트 픽스처 클래스 정의
// 다양한 테스트에서 같은 정의/준비가 필요할 때 사용할 수 있다
// 4개의 정렬을 검사하기 위해 같은 데이터를 준비
class CourseSortTest : public ::testing::Test {
protected:
    std::vector<Course::Course> courses;
    std::vector<Course::Course> correctAscCourses;
    std::vector<Course::Course> correctDesCourses;
    void SetUp() override {
      courses = { 
        {"f", 3, 3.5, 7}, {"e", 2, 4.0, 6}, {"z", 1, 1.0, 4}, {"s", 0, 1.5, 2}, 
        {"가", 1, 2.0, 1}, {"a", 2, 4.5, 3}, {"b", 3, 2.5, 5}, {"c", 2, 0.0, 0}
      };

      // 모든 데이터가 정렬된 순서로 있는 정답변수
      correctAscCourses = {
        {"a", 0, 0.0, 0}, {"b", 1, 1.0, 1}, {"c", 1, 1.5, 2}, {"e", 2, 2.0, 3},
        {"f", 2, 2.5, 4}, {"s", 2, 3.5, 5}, {"z", 3, 4.0, 6}, {"가", 3, 4.5, 7}
      };
      correctDesCourses = {
        {"가", 3, 4.5, 7}, {"z", 3, 4.0, 6}, {"s", 2, 3.5, 5}, {"f", 2, 2.5, 4},
        {"e", 2, 2.0, 3}, {"c", 1, 1.5, 2}, {"b", 1, 1.0, 1}, {"a", 0, 0.0, 0}
      };
    }
};

/*
Arrange : 정렬이 안된 Course 객체와 정렬이 된 Course객체
Act     : 정렬이 안된 Course 객체를 사용자 정의 규칙으로 이름, 이수학점, 점수, 분류에 대해서 각 정렬 수행
Assert  : 
    - 기대하는 정렬 (오름차순, 내림차순)으로 정렬이 되는 지 확인
*/
TEST_F(CourseSortTest, NameSort_SortsCoursesByNameAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseNameCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctAscCourses[i].courseName, courses[i].courseName);
  }
}
TEST_F(CourseSortTest, CreditsSort_SortsCoursesByCreditsAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseCreditsCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctAscCourses[i].credits, courses[i].credits);
  }
}
TEST_F(CourseSortTest, GradeSort_SortsCoursesByGradeAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseGradeCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_DOUBLE_EQ(correctAscCourses[i].grade, courses[i].grade);
  }
}
TEST_F(CourseSortTest, CategorySort_SortsCoursesByCategoryAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseCategoryCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctAscCourses[i].category, courses[i].category);
  }
}

// 내림차순 정렬 검사
TEST_F(CourseSortTest, NameSort_SortsCoursesByNameDes) {
  std::sort(courses.begin(), courses.end(), Course::courseNameCompareDescendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctDesCourses[i].courseName, courses[i].courseName);
  }
}
TEST_F(CourseSortTest, CreditsSort_SortsCoursesByCreditsDes) {
  std::sort(courses.begin(), courses.end(), Course::courseCreditsCompareDescendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctDesCourses[i].credits, courses[i].credits);
  }
}
TEST_F(CourseSortTest, GradeSort_SortsCoursesByGradeDes) {
  std::sort(courses.begin(), courses.end(), Course::courseGradeCompareDescendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_DOUBLE_EQ(correctDesCourses[i].grade, courses[i].grade);
  }
}
TEST_F(CourseSortTest, CategorySort_SortsCoursesByCategoryDes) {
  std::sort(courses.begin(), courses.end(), Course::courseCategoryCompareDescendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctDesCourses[i].category, courses[i].category);
  }
}




// TDD - 이후 정렬 후 이진탐색이 올바른지 검증하는 테스트 코드 작성
