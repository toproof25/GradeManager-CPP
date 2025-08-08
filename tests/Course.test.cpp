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
TEST(CourseTest, ConstructorInitializesCorrectly)
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
TEST(CourseTest, Setter_CorrectValueWhenChangingData)
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
Act     : convertToGrade함수로 점수 변환
Assert  : 
    - 각 점수에 대해 올바르게 변환하는지 검즘
*/
TEST(CourseTest, ConvertToGrade_CorrectValueForScoreConversion)
{
  ASSERT_EQ("A+", Course::convertToGrade(4.5));
  ASSERT_EQ("D", Course::convertToGrade(1.0));
  ASSERT_EQ("P", Course::convertToGrade(0.0));
  ASSERT_EQ("X", Course::convertToGrade(10.0));
}

/*
Arrange : 
Act     : convertToCategory함수로 전공분류를 문자열로 변환
Assert  : 
    - 올바르게 변환하는지 검증
*/
TEST(CourseTest, ConvertToCategory_CorrectValueForScoreConversion)
{
  ASSERT_EQ("전공선택", Course::convertToCategory(0));
  ASSERT_EQ("일반(선택)", Course::convertToCategory(3));
  ASSERT_EQ("ERROR", Course::convertToCategory(8));
}




// 테스트 픽스처 클래스 정의
// 다양한 테스트에서 같은 정의/준비가 필요할 때 사용할 수 있다
// 4개의 정렬을 검사하기 위해 같은 데이터를 준비
class CourseSortTest : public ::testing::Test {
protected:
    std::vector<Course::Course> courses;
    std::vector<Course::Course> correctCourses;
    void SetUp() override {
      courses = { 
        {"f", 3, 3.5, 7}, {"e", 2, 4.0, 6}, {"z", 1, 1.0, 4}, {"s", 0, 1.5, 2}, 
        {"가", 1, 2.0, 1}, {"a", 2, 4.5, 3}, {"b", 3, 2.5, 5}, {"c", 2, 0.0, 0}
      };

      // 모든 데이터가 정렬된 순서로 있는 정답변수
      correctCourses = {
        {"a", 0, 0.0, 0}, {"b", 1, 1.0, 1}, {"c", 1, 1.5, 2}, {"e", 2, 2.0, 3},
        {"f", 2, 2.5, 4}, {"s", 2, 3.5, 5}, {"z", 3, 4.0, 6}, {"가", 3, 4.5, 7}
      };
    }
};

/*
Arrange : 정렬이 안된 Course 객체와 정렬이 된 Course객체
Act     : 정렬이 안된 Course 객체를 사용자 정의 규칙으로 이름, 이수학점, 점수, 분류에 대해서 각 정렬 수행
Assert  : 
    - 기대하는 정렬 (오름차순)으로 정렬이 되는 지 확인
*/
TEST_F(CourseSortTest, NameSort_SortsCoursesByNameAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseNameCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctCourses[i].courseName, courses[i].courseName);
  }
}
TEST_F(CourseSortTest, CreditsSort_SortsCoursesByCreditsAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseCreditsCompareAscendingOrder);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctCourses[i].credits, courses[i].credits);
  }
}
TEST_F(CourseSortTest, GradeSort_SortsCoursesByGradeAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseGradeCompare);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_DOUBLE_EQ(correctCourses[i].grade, courses[i].grade);
  }
}
TEST_F(CourseSortTest, CategorySort_SortsCoursesByCategoryAscendingly) {
  std::sort(courses.begin(), courses.end(), Course::courseCategoryCompare);

  int size = courses.size();
  for (int i=0; i<size; ++i)
  {
    ASSERT_EQ(correctCourses[i].category, courses[i].category);
  }
}



// TDD - 이후 정렬 후 이진탐색이 올바른지 검증하는 테스트 코드 작성
