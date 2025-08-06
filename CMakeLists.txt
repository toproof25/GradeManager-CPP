# 프로젝트 기본 설정
cmake_minimum_required(VERSION 4.1.0 )
project(GradeManager CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 모든 핵심 소스 코드를 포함하는 cpp파일들을 라이브러리 타겟으로 정의

    # src/ 내 모든 cpp을 SRC 변수에 저장
    file(GLOB SRC 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/*.cpp"
    )

    # 위에서 만든 SRC 변수에서 main함수가 포함된 grade_manager.cpp을 제거 - 테스트 코드도 제거
    list(REMOVE_ITEM SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/grade_manager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test_imgui.cpp"
    )

    # 최종적으로 src/ 에 grade_manager.cpp을 제외하고 모두 라이브러리에 등록
    add_library(GradeManagerLib
        ${SRC}
    )


# 라이브러리에 필요한 헤더 파일 경로를 지정

    # .cpp을 컴파일하는 과정에서 만나는 #include 헤더 파일들을 아래에 지정한 경로에서 찾는다
    target_include_directories(GradeManagerLib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui
    )   

# main cpp 파일을 실행 파일로 만드는 과정
    # add_executable(실행파일명 .cpp 파일 경로)   
    add_executable(GradeManagerApp src/grade_manager.cpp)       


# 실행 파일에 위에서 만든 라이브러리를 링크
    # GradeManagerLib의 소스코드와 헤더 경로 설정이 GradeManagerApp에 전달됩니다.
    target_link_libraries(GradeManagerApp PRIVATE 
        GradeManagerLib
        d3d11        # Direct3D 11 라이브러리
        d3dcompiler  # 셰이더 컴파일
        dxgi         # DXGI 라이브러리 
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME}) # 실행파일명
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) # 실행파일버전
include(CPack) # 기본적으로 build 경로에 실행파일을 생성함
