#include "gtest/gtest.h" // Google Test 헤더 파일 포함
#include "Semester.h"     
#include "Course.h"    


/*
Arrange : Semester 객체와 학년/학기 데이터
Act     : 학년/학기 Semester 데이터로 생성
Assert  : Semester 생성자로 넣어 생성된 Semester 데이터 초기값이 일치하는지 확인
*/
TEST(SemesterTest, CreateSemesterClassShouldEquleValue) {

    int year = 3;
    int semester = 2;

    Semester s(year, semester);
    int courseVectorSize = s.getCourses().size();

    ASSERT_EQ(3, s.getYear());
    ASSERT_EQ(2, s.getSemester());
    ASSERT_EQ(0, s.getCourses().size());
}




/*
Arrange : Semester 객체와 Course 객체를 준비
Act     : Semester.addCourse(Course) - Course를 Semester 멤버 변수 vector<Course> courses에 push하는 함수 실행
Assert  : 
    - 빈 vector에 추가했을 때 사이즈가 1이 된다면 정상적으로 추가됨을 검증
    - 테스트로 생성한 Course와 추가된 Course의 정보가 일치하는지 검증
*/
TEST(SemesterTest, AddCourse_ShouldIncreaseSizeAndStoreCorrectData) {

    Semester s(1, 1);
    Course::Course c = {"test", 3, 4.5, 1};
    int preSize = s.getCourses().size();
    s.addCourses(c);

    // 1) courses 벡터의 사이즈가 이전 사이즈보다 +1 이어야 함
    ASSERT_EQ(preSize+1, s.getCourses().size());

    // 2) 벡터의 마지막 원소 course가 추가한 Course와 데이터가 일치해야 함
    const Course::Course& course = s.getCourses().back();
    EXPECT_EQ(c.courseName, course.courseName);
    EXPECT_EQ(c.credits, course.credits);
    EXPECT_DOUBLE_EQ(c.grade, course.grade);
    EXPECT_EQ(c.category, course.category);
}
