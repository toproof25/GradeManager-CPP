#include "gtest/gtest.h" // Google Test 헤더 파일
#include "Semester.h"     
#include "Course.h"    


/*
Arrange : Semester 객체와 학년/학기 데이터
Act     : 학년/학기 Semester 데이터로 생성
Assert  : 
    - Semester 생성자로 넣어 생성된 Semester 데이터 초기값이 일치하는지 확인 (생성자 초기값 학년/학기, courses 벡터의 크기는 0)
    - 또한 각 멤버 변수에 대한 getter함수가 올바르게 동작하는지 확인
*/
TEST(SemesterTest, ConstructorInitializesCorrectly) {

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


/*
Arrange : Semester 객체와 Course 객체 2개를 정의
Act     : Semester객체에 Course를 제거하는 removeCourses(제거 index)를 실행
Assert  : 
    - 2개의 과목에서 1개를 제거했을 때 벡터 사이즈가 1로 일치하는지 확인
    - 2개 중 첫번째 과목 제거 후 남은 과목 데이터가 일치하는지 확인
*/
TEST(SemesterTest, RemoveCourses_DecreasesSizeAndMaintainsCorrectData) {

    Semester s(4, 2);
    Course::Course c = {"test", 3, 4.5, 1};
    Course::Course c2 = {"test2", 1, 2.0, 5};

    s.addCourses(c);
    s.addCourses(c2);


    std::vector<Course::Course>& courses = s.getCourses();
    std::vector<Course::Course>::iterator it = courses.end()-1;

    // 1) 제거된 상태에서 크기가 제대로 줄어들었는지 확인
    s.removeCourses(it);
    ASSERT_EQ(1, s.getCourses().size());

    // 2) 두개의 과목 중 하나가 제거되고, 남은 과목 데이터가 일치하는지 확인
    const Course::Course& course = s.getCourses().back();
    EXPECT_EQ(c.courseName, course.courseName);
    EXPECT_EQ(c.credits, course.credits);
    EXPECT_DOUBLE_EQ(c.grade, course.grade);
    EXPECT_EQ(c.category, course.category);
}



/*
Arrange : Semester객체와 Course객체 여러개를 준비
Act     : Semester객체에 Course를 추가한 후 calculateGPA (성적 구하기) 함수를 실행
Assert  : 
    - 설정된 점수의 예상한 최종점수가 일치하는지 검증
*/
TEST(SemesterTest, CalculateGPA_GpaCalculatesCorrectlyGPA) {

    Semester s(1, 2);
    Course::Course c =  {"과목1", 3, 4.5, 1};
    Course::Course c2 = {"과목2", 2, 2.0, 3};
    Course::Course c3 = {"과목3", 1, 1.0, 2};
    Course::Course c4 = {"과목4", 3, 3.5, 6};
    Course::Course c5 = {"과목5", 0, 0.0, 7};

    s.addCourses(c);
    s.addCourses(c2);
    s.addCourses(c3);
    s.addCourses(c4);
    s.addCourses(c5);

    // ASSERT_NEAR(예상값, 실행결과, 오차범위) - 오차범위 내라면 True
    ASSERT_NEAR(3.22, calculateGPA(s.getCourses()), 0.009);
}