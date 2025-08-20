#pragma once
#include "imgui.h"
#include "Course.h"
#include "Semester.h"
#include "GradeManager.h"

#include <vector>
#include <array>
#include <string>
#include <Windows.h>




// UIManager.h
class UIManager {

private:
    GradeManager& gm;

    bool isInit = false;

    std::string toastMessege;

    // --- Toast Messege 프레임 계산 ---
    bool m_showToastMessege = false;
    int start_frame = 0; // 알림이 시작된 프레임 번호
    const int VISIBLE_FRAMES = 180;  // 완전히 보이는 프레임 수 (약 1초)
    const int FADE_FRAMES = 240;    // 사라지는 데 걸리는 프레임 수 (약 2초)

public:
    UIManager(GradeManager& manager);
    void render(MSG& msg, HWND& hwnd);


private:
    void displaySemestersWindow();  // 모든 학기 윈도우

    void displayCoursesWindow(); // 한 학기의 과목 출력
    void displayInfomationCourseWindow(const Course::Course& c);                             // 한 과목의 정보 출력

    void promptValueCourseWindow(
      bool isInit, 
      const std::function<void(Course::Course&)>& onClickSave,
      bool& m_showWindow    
    );

    void displayToastMessege(std::string messege);
    void displayToastMessege_(const char* messege);

    void displayOptionBar(HWND& hwnd);

    void displayTotalGradeGraph();

    void alignCenter(const char* text);
    bool centerAlignButton(const char* label);
};