cmake_minimum_required(VERSION 3.10.0)
project(GradeManager-CPP VERSION 0.1.0 LANGUAGES C CXX)

# C++ 표준 20 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(grade_manager
    source/cpp/grade_manager.cpp
    source/cpp/GradeManager.cpp
    source/cpp/ConsoleUIManager.cpp
    source/cpp/Semester.cpp
    source/cpp/Course.cpp
    source/cpp/SemesterJSON.cpp
)

# 헤더 파일을 찾을 경로를 지정
target_include_directories(grade_manager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/header/
    ${CMAKE_CURRENT_SOURCE_DIR}/source/lib/
)
