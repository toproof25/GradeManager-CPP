#include "gtest/gtest.h" // Google Test 헤더 파일
#include "json.hpp"
#include "SemesterJSON.h"
#include "Semester.h"     
#include "Course.h"   

#include <array>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class SemesterJsonFileTest : public ::testing::Test {
protected:
    const std::string test_filename = "test_json.json";
    SemesterJSON* semesterJson;

    void SetUp() override {
      semesterJson = new SemesterJSON(test_filename);
    }
    void TearDown() override {
      delete semesterJson;
      semesterJson = nullptr;
      if (fs::exists(test_filename)) 
      {
        fs::remove(test_filename);
      }
    }

};

/*
Arrange : ifstream으로 파일 입력
Act     : is_open의 상태를 확인
Assert  : 
    - 올바르게 test_filename이 생성됐다면 성공
*/
TEST_F(SemesterJsonFileTest, CreateBasicJson_CheckCreateFile)
{
  semesterJson->createBasicJson();
  std::ifstream input_file(test_filename);
  ASSERT_TRUE(input_file.is_open());
  input_file.close();
}


/*
Arrange : semesters array 변수
Act     : loadJson을 하여 array를 반환받는다
Assert  : 
    - 사이즈가 올바르게 8개로 나타난다면 불러와진 것
    - 각 학기의 학년/학기/과목의 사이즈가 일치하는지 확인
*/
TEST_F(SemesterJsonFileTest, LoadJson_VerifiesSemesterArraySizeAndContent)
{
  std::array<Semester, 8> semesters = semesterJson->loadJson();
  ASSERT_EQ(8, semesters.size());

  ASSERT_EQ(1, semesters[0].getYear()); ASSERT_EQ(1, semesters[0].getSemester()); ASSERT_EQ(0, semesters[0].getCourses().size());
  ASSERT_EQ(1, semesters[1].getYear()); ASSERT_EQ(2, semesters[1].getSemester()); ASSERT_EQ(0, semesters[1].getCourses().size());

  ASSERT_EQ(2, semesters[2].getYear()); ASSERT_EQ(1, semesters[2].getSemester()); ASSERT_EQ(0, semesters[2].getCourses().size());
  ASSERT_EQ(2, semesters[3].getYear()); ASSERT_EQ(2, semesters[3].getSemester()); ASSERT_EQ(0, semesters[3].getCourses().size());

  ASSERT_EQ(3, semesters[4].getYear()); ASSERT_EQ(1, semesters[4].getSemester()); ASSERT_EQ(0, semesters[4].getCourses().size());
  ASSERT_EQ(3, semesters[5].getYear()); ASSERT_EQ(2, semesters[5].getSemester()); ASSERT_EQ(0, semesters[5].getCourses().size());

  ASSERT_EQ(4, semesters[6].getYear()); ASSERT_EQ(1, semesters[6].getSemester()); ASSERT_EQ(0, semesters[6].getCourses().size());
  ASSERT_EQ(4, semesters[7].getYear()); ASSERT_EQ(2, semesters[7].getSemester()); ASSERT_EQ(0, semesters[7].getCourses().size());
  
}


/*
Arrange : index번호와 Course객체
Act     : Course를 생성 후 createJsonData함수로 실제 json파일에 값을 추가한다
Assert  : 
    - 추가된 학기의 과목 사이즈가 늘어났는지 확인
    - 해당 과목이 추가한 과목의 데이터와 일치하는지 확인
*/
TEST_F(SemesterJsonFileTest, CreateJson_VerifiesAddedCourseSizeAndValue)
{
  int test_index = 3;
  Course::Course c = {"추가할 과목", 3, 3.5, 1};

  // 빈 json파일 생성
  semesterJson->createBasicJson();

  // 과목을 추가
  semesterJson->createJsonData(test_index, c);

  // 추가 후 다시 새롭게 json파일을 읽어와서 확인
  std::array<Semester, 8> result_semesters = semesterJson->loadJson();
  Semester& s = result_semesters[test_index];
  Course::Course& added_c = s.getCourses().back();

  ASSERT_EQ(1, s.getCourses().size());
  ASSERT_EQ("추가할 과목", added_c.courseName);
  ASSERT_EQ(3, added_c.credits);
  ASSERT_EQ(3.5, added_c.grade);
  ASSERT_EQ(1, added_c.category);
}


/*
Arrange : 제거할 index번호와 Course객체
Act     : 먼저 제거할 과목을 추가 후 제거, JSON파일을 다시 불러와서 확인 (4개 과목 추가 후 1개 제거)
Assert  : 
    - 올바르게 제거가 되었다면 사이즈가 0으로 나타남
    - 해당 과목이 추가한 과목의 데이터와 일치하는지 확인
*/
TEST_F(SemesterJsonFileTest, DeleteJson_VerifiesAddedCourseSizeAndValue)
{
  int test_index = 5;
  Course::Course c = {"제거할 과목", 1, 1.5, 6};

  // 빈 json 데이터 생성
  semesterJson->createBasicJson();
  
  // 먼저 제거하기 위한 4개 과목을 추가
  semesterJson->createJsonData(test_index, c);
  semesterJson->createJsonData(test_index, c);
  semesterJson->createJsonData(test_index, c);
  semesterJson->createJsonData(test_index, c);

  // 과목 제거, 2번째 과목을 제거
  semesterJson->deleteJsonData(test_index, 2);

  // 제거 후 다시 새롭게 json파일을 읽어와서 확인
  std::array<Semester, 8> result_semesters = semesterJson->loadJson();
  ASSERT_EQ(3, result_semesters[test_index].getCourses().size());
}



/*
Arrange : 제거할 index번호와 Course객체
Act     : 먼저 제거할 과목을 추가 후 제거, JSON파일을 다시 불러와서 확인 (4개 과목 추가 후 1개 제거)
Assert  : 
    - 올바르게 제거가 되었다면 사이즈가 0으로 나타남
    - 해당 과목이 추가한 과목의 데이터와 일치하는지 확인
*/
TEST_F(SemesterJsonFileTest, UpdateJson_VerifiesUpdatedCourseValue)
{
  int semesters_index = 5;
  int update_index = 3;
  Course::Course c = {"수정할 과목", 1, 1.5, 6};

  std::string update_name = "수정된 과목";
  int update_credits = 2;
  double update_grage = 4.5;
  int update_category = 1;

  // 빈 json 데이터 생성
  semesterJson->createBasicJson();
  
  // 먼저 수정하기 위한 7개 과목을 추가
  semesterJson->createJsonData(semesters_index, c);
  semesterJson->createJsonData(semesters_index, c);
  semesterJson->createJsonData(semesters_index, c);
  semesterJson->createJsonData(semesters_index, c);

  // 과목 내용 수정
  semesterJson->updateJsonData(semesters_index, update_index, "courseName", update_name);
  semesterJson->updateJsonData(semesters_index, update_index, "credits", update_credits);
  semesterJson->updateJsonData(semesters_index, update_index, "grade", update_grage);
  semesterJson->updateJsonData(semesters_index, update_index, "category", update_category);

  // 수정 후 다시 새롭게 json파일을 읽어와서 확인
  std::array<Semester, 8> result_semesters = semesterJson->loadJson();
  Course::Course& update_c = result_semesters[semesters_index].getCourses()[update_index];
  
  ASSERT_EQ(update_name    , update_c.courseName);
  ASSERT_EQ(update_credits , update_c.credits);
  ASSERT_EQ(update_grage   , update_c.grade);
  ASSERT_EQ(update_category, update_c.category);
}