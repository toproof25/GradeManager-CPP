# 프로젝트 기본 설정
cmake_minimum_required(VERSION 4.1.0 )
project(GradeManager CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 모든 핵심 소스 코드를 포함하는 라이브러리 타겟을 정의
# main cpp을 제외하고 나머지 .cpp 파일을 모두 라이브러리로 지정
  # src/ 내 모든 cpp을 SRC 변수에 저장
  file(GLOB SRC 
      "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  )

  # 위에서 만든 .cpp이 있는 SRC 변수에서 main함수가 포함된 grade_manager.cpp을 제거
  list(REMOVE_ITEM SRC
      "${CMAKE_CURRENT_SOURCE_DIR}/src/grade_manager.cpp"
  )

  # 최종적으로 src/ 에 grade_manager.cpp을 제외하고 모두 라이브러리에 등록
  add_library(GradeManagerLib
      ${SRC}
  )

# 2. 라이브러리에 필요한 헤더 파일 경로를 지정합니다.
# 1번에서 .cpp 에 사용되는 모든 .h, .hpp 파일들의 경로를 지정
# 즉 1번 .cpp을 컴파일하는 과정에서 만나는 #include 헤더 파일들을 아래에 지정한 경로에서 찾는다
target_include_directories(GradeManagerLib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)   

# 3. main cpp 파일을 실행 파일로 만드는 과정
# add_executable(실행파일명 .cpp 파일 경로)   
add_executable(GradeManagerApp src/grade_manager.cpp)       

# 4. 실행 파일에 위에서 만든 라이브러리를 연결(link)합니다.
# GradeManagerLib의 소스코드와 헤더 경로 설정이 GradeManagerApp에 전달됩니다.
target_link_libraries(GradeManagerApp PRIVATE GradeManagerLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME}) # 실행파일명
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) # 실행파일버전
include(CPack) # 기본적으로 build 경로에 실행파일을 생성함




# ============================ GoogleTest 추가 =============================
include(FetchContent)

# GoogleTest v1.17.0 FetchContent 선언 (URL 방식)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# Windows CRT 런타임 충돌 방지
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# 실제로 다운로드 및 컴파일 시 포함
FetchContent_MakeAvailable(googletest)

# CTest 기능 활성화
enable_testing()

# 테스트 실행 파일 추가
add_executable(SemesterTest tests/Semester.test.cpp)
add_executable(CourseTest tests/Course.test.cpp)

# 테스트에서 인클루드할 헤더 경로 지정 (inc/)
target_include_directories(SemesterTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(CourseTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 테스트용 바이너리에 GTest 및 프로젝트 라이브러리 링크
target_link_libraries(SemesterTest PRIVATE
    GTest::gtest_main
    GradeManagerLib
)
target_link_libraries(CourseTest PRIVATE
    GTest::gtest_main
    GradeManagerLib
)

# 테스트 자동 등록
include(GoogleTest)
gtest_discover_tests(SemesterTest)
gtest_discover_tests(CourseTest)