#include "imgui.h"                 // ImGui 핵심 API: 즉시 모드 GUI 라이브러리의 기능을 제공하는 헤더
#include "Course.h"
#include "UIManager.h"
#include "GradeManager.h"

#include <vector>
#include <string>
#include <functional>              // std::function을 사용하기 위해 헤더 추가

const char* creditsitems[] = { "0", "1", "2", "3" };
const char* gradeItems[] = { "NP", "P", "F", "D", "D+", "C", "C+", "B", "B+", "A", "A+" };
const char* categoryitems[] = { "전공선택", "복수전공", "기초(필수)", "일반(선택)", "균형교양", "계열교양", "타전공" };

UIManager::UIManager(GradeManager& manager) : gm(manager) {}

// 버튼을 셀 중앙에 배치하는 헬퍼 람다 함수
bool UIManager::centerAlignButton(const char* label) {
    float cellWidth = ImGui::GetColumnWidth();
    float buttonWidth = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float newCursorPosX = ImGui::GetCursorPosX() + (cellWidth - buttonWidth) * 0.5f;
    
    // 커서가 셀의 시작 위치보다 왼쪽으로 가지 않도록 방지
    if (newCursorPosX > ImGui::GetCursorPosX()) {
        ImGui::SetCursorPosX(newCursorPosX);
    }
    return ImGui::Button(label);
};

void UIManager::alignCenter(const char* text) {
    float columnWidth = ImGui::GetColumnWidth();
    float textWidth = ImGui::CalcTextSize(text).x;
    // 커서를 (열 너비 - 텍스트 너비)의 절반만큼 이동시켜 가운데 정렬 효과를 냅니다.
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
    ImGui::TextUnformatted(text);
};  


void UIManager::render(MSG& msg, HWND& hwnd)
{
  // --- 1. 전체 화면을 덮는 보이지 않는 창을 만들고, 그 안을 도킹 공간으로 설정합니다. ---
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::Begin("DockSpaceDemo", nullptr, window_flags);
  ImGui::PopStyleVar(2);

  // 도킹 공간 생성!
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);


  /* ------------------------- UI 렌더링 부분 ------------------------- */
  displayOptionBar(hwnd);

  // 토스트 메시지
  if (m_showToastMessege)
  {
      displayToastMessege_(this->toastMessege.c_str());
  }

  // 학기 윈도우
  displaySemestersWindow();

  // 각 학기 내 과목 윈도우
  displayCoursesWindow();

  // 과정 정보 윈도우
  displayTotalGradeGraph();

  // 과목 수정 윈도우 
  if (m_showEditWindow)
  {
      auto editHandler = [&](Course::Course& editedCourse) {
        // 핸들러만 호출하기
          gm.handleFixCourse(editedCourse);
          displayToastMessege(editedCourse.courseName + " 과목을 수정했습니다");
      };
      
      promptValueCourseWindow(this->isInit, editHandler, m_showEditWindow);
  }
  // 과목 추가 윈도우
  if (m_showAddWindow)
  {
      auto addHandler = [&](Course::Course& addedCourse) {
          gm.handleAddCourse(addedCourse);
          displayToastMessege(addedCourse.courseName + " 과목을 추가했습니다");
      };

      promptValueCourseWindow(false, addHandler, m_showAddWindow);
  }

  // Dockspace를 위한 ImGui::End()
  ImGui::End();
}


void UIManager::displayTotalGradeGraph()
{
    std::array<Semester, 8>& semesters = gm.getSemesters();

    static float totalGrade[8] = {0};
    int index = 0;
    for (Semester& s : semesters)
    {
        float gpa = calculateGPA(s.getCourses());
        totalGrade[index++] = gpa;
    }

    ImGui::Begin("Graph");

    ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); 
    ImGui::PlotLines("총 학점", totalGrade, 8, 0, "1학기~8학기 점수 그래프", 0.0f, 5.5f, ImVec2(0, 300));
    ImGui::PopStyleColor(); 

    ImGui::End();

}

void UIManager::displayOptionBar(HWND& hwnd)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("파일"))
        {
            if (ImGui::MenuItem("학기 파일 불러오기"))
            {
                displayToastMessege(gm.handleLoadJsonFile(hwnd));
            }

            ImGui::Separator();

            if (ImGui::MenuItem("현재 상태 저장"))
            {
                gm.handleSaveJson();
                displayToastMessege("저장되었습니다");
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void UIManager::displayToastMessege(std::string messege)
{
    m_showToastMessege = true;
    start_frame = ImGui::GetFrameCount(); // 시작 프레임 기록
    toastMessege = messege;
}

void UIManager::displayToastMessege_(const char* messege)
{
    // 1. 프로그램 창의 전체 크기를 가져옵니다.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 center = main_viewport->GetCenter();
    ImVec2 window_size = main_viewport->WorkSize;

    // 2. ImGui 창의 위치와 크기를 전체 창 크기에 맞게 고정시킵니다.
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(300, 50), ImGuiCond_Always);

    // 3. 창 이동, 크기 조절, 접기 등 모든 상호작용을 비활성화하는 플래그
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoTitleBar;

    int current_frame = ImGui::GetFrameCount();
    int elapsed_frames = current_frame - start_frame;

    // 시간이 다 되면 창을 숨김
    if (elapsed_frames > VISIBLE_FRAMES + FADE_FRAMES)
    {
        m_showToastMessege = false;
        return;
    }

    // --- 투명도 계산 ---
    float alpha = 1.0f; // 기본은 불투명
    if (elapsed_frames > VISIBLE_FRAMES)
    {
        // 사라지는 구간에 진입하면, 경과된 프레임을 기준으로 투명도를 계산
        float fade_progress = (float)(elapsed_frames - VISIBLE_FRAMES) / FADE_FRAMES;
        alpha = 1.0f - fade_progress;
    }

    ImGui::SetNextWindowBgAlpha(alpha);
    ImGui::Begin("ToastMessege", nullptr, window_flags);
    alignCenter(messege);
    ImGui::End();
}

// 모든 학기 윈도우
void UIManager::displaySemestersWindow()
{       
    std::array<Semester, 8>& semesters = gm.getSemesters();

    ImGui::Begin("학기 조회");

    alignCenter("강남대학교 ICT융합공학부 가상현실전공");
    alignCenter("201904022 김상옥");
    alignCenter("전체 학기 관리");

    int buttonId = 10;
    for (Semester& s : semesters)
    {
        // 각 버튼이 고유 ID를 갖도록 PushID/PopID를 사용합니다.
        ImGui::PushID(buttonId++); 

        // 버튼에 표시될 텍스트를 만듭니다.
        std::string semesterTitle = std::to_string(s.getYear()) + "학년 " + std::to_string(s.getSemester()) + "학기";

        // 1. 버튼 크기를 설정합니다.
        ImVec2 button_size = ImVec2(-1.0f, 50.0f);

        // 2. 가로로 긴 버튼을 생성합니다.
        if (ImGui::Button(semesterTitle.c_str(), button_size))
        {
            gm.setSelectSemester(s);
        }
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
    }

    ImGui::End();   
}

// 학기 내 과목 윈도우
void UIManager::displayCoursesWindow()
{
    Semester& semester = gm.getSelectSemester();
    int year = semester.getYear();
    int semesterNumber = semester.getSemester();
    std::vector<Course::Course>& courses = semester.getCourses();

    ImGui::Begin("과목 조회");
    std::string semesterTitle = std::to_string(year) + "학년 " + std::to_string(semesterNumber) + "학기 과목\n"; 

    int allCredit = 0;
    for (const Course::Course& c : courses)
    {
        allCredit += c.credits;
    }
    double gpa = calculateGPA(courses);

    std::string  coursesTitle = "이수학점(" + std::to_string(allCredit) + ") / 최종점수(" + std::to_string(gpa) + ")";
    alignCenter(semesterTitle.c_str());
    alignCenter(coursesTitle.c_str());

    if (ImGui::BeginTable("courseInfoTable", 6, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("과목명", ImGuiTableColumnFlags_WidthStretch, 4.0f);
        ImGui::TableSetupColumn("학점", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("점수", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("분류", ImGuiTableColumnFlags_WidthFixed, 50.0f); // 너비 약간 조절
        ImGui::TableSetupColumn("수정", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("제거", ImGuiTableColumnFlags_WidthFixed, 50.0f);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if(ImGui::Button("과목이름"))
        {
            gm.handleSortCourse(1);
        }
        ImGui::TableSetColumnIndex(1);
        if(ImGui::Button("이수학점"))
        {
            gm.handleSortCourse(2);
        }
        ImGui::TableSetColumnIndex(2);
        if(ImGui::Button("받은점수"))
        {
            gm.handleSortCourse(3);
        }
        ImGui::TableSetColumnIndex(3);
        if(ImGui::Button("전공분류"))
        {
            gm.handleSortCourse(4);
        }


        int buttonId = 0;
        for (Course::Course& c : courses)
        {
            ImGui::PushID(buttonId++); 
            ImGui::TableNextRow();


            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button(c.courseName.c_str()))
            {
                m_courseReadWindow = true;
                gm.setSelectCourse(c);
            }


            ImGui::TableSetColumnIndex(1);
            ImGui::Text(std::to_string(c.credits).c_str());
            
            ImGui::TableSetColumnIndex(2);
            std::string grade = Course::convertToGrade(c.grade);
            ImGui::Text(grade.c_str());
            
            ImGui::TableSetColumnIndex(3);
            std::string category = Course::convertToCategory(c.category);
            ImGui::Text(category.c_str());


            ImGui::TableSetColumnIndex(4);
            if (centerAlignButton("수정"))
            {
                gm.setSelectCourse(c);
                m_showEditWindow = true;
                m_showAddWindow = false;
                m_courseReadWindow = false;
                this->isInit = true;
            }

            ImGui::TableSetColumnIndex(5);
            if (centerAlignButton("제거")) 
            {
                std::string cName = c.courseName;
                gm.setSelectCourse(c);
                gm.handleRemoveCourse();
                displayToastMessege(cName + " 과목을 제거했습니다");
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    // 저장 버튼 중앙 배치
    const char* label = "+++새로운 과목 추가+++";
    float region = ImGui::GetContentRegionAvail().x;
    float txtW = ImGui::CalcTextSize(label).x;
    ImGui::SetCursorPosX((region - txtW) * 0.5f);
    if (ImGui::Button(label))
    {
        m_showEditWindow = false;
        m_courseReadWindow = false;
        m_showAddWindow = true;
        this->isInit = false;
    }


    ImGui::End();   
}

// Course::Course& course :  수정할 과목을 참조
// bool isInit : 입력 폼을 course로 초기값을 설정
void UIManager::promptValueCourseWindow(
    bool isInit, 
    const std::function<void(Course::Course&)>& onClickSave,
    bool& m_showWindow    
)
{
    Course::Course& course = gm.getSelectCourse();

    // 해다 함수(창)이 실행 시 한번만 과목의 정보를 입력폼에 넣고, 이후에는 변경해도 상관없도록
    // 현재는 수정을 해도 계속 기존 과목 정보로 초기화가 됨
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing , ImVec2(0.5f, 0.5f));      // 창 실행 시 위치
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Appearing ); // 창 실행 시 크기

    // 과목 정보 입력값
    static char tempCourseNameBuffer[256] = "";
    static int tempCreditsItem = 0;
    static int tempGradeItem = 0;
    static int tempCategoryItem = 0;

    // 수정 시 초기값을 설정
    if (isInit)
    {
        // 과목명 버퍼 초기화
        strncpy(tempCourseNameBuffer, course.courseName.c_str(), sizeof(tempCourseNameBuffer) - 1);

        // 이수학점 콤보박스 인덱스 초기화
        tempCreditsItem = course.credits;

        // 받은 점수 콤보박스 인덱스 초기화
        std::string gradeStr = Course::convertToGrade(course.grade);
        // 변환된 문자열과 일치하는 항목을 gradeItems 배열에서 찾아 인덱스를 설정
        for (int i = 0; i < IM_ARRAYSIZE(gradeItems); ++i) 
        {
            if (gradeItems[i] == gradeStr) {
                tempGradeItem = i;
                break;
            }
        }
        
        // 전공분류 콤보박스 인덱스 초기화
        std::string categoryStr = Course::convertToCategory(course.category);
        for (int i = 0; i < IM_ARRAYSIZE(categoryitems); ++i) 
        {
            if (categoryitems[i] == categoryStr) {
                tempCategoryItem = i;
                break;
            }
        }
        this->isInit = false;
    }

    ImGui::Begin("과목 정보 입력", &m_showWindow);

    // 2열 테이블
    if (ImGui::BeginTable("courseTable", 2))
    {
        // 열 너비 설정
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();

        // — 1행 과목명 —
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("과목명");
        ImGui::TableSetColumnIndex(1);
        ImGui::PushItemWidth(-1);
        ImGui::InputText("##CourseName", tempCourseNameBuffer, IM_ARRAYSIZE(tempCourseNameBuffer));
        ImGui::PopItemWidth();

        // — 2행 이수학점 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("이수학점");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Credits", &tempCreditsItem, creditsitems, IM_ARRAYSIZE(creditsitems));

        // — 3행 받은 점수 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("받은 점수");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Grade", &tempGradeItem, gradeItems, IM_ARRAYSIZE(gradeItems));

        // — 4행 전공분류 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("전공분류");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Category", &tempCategoryItem, categoryitems, IM_ARRAYSIZE(categoryitems));

        ImGui::EndTable();
    }

    // 저장 버튼 중앙 배치
    const char* label = "저장";
    float region = ImGui::GetContentRegionAvail().x;
    float txtW = ImGui::CalcTextSize(label).x;
    ImGui::SetCursorPosX((region - txtW) * 0.5f);
    if (ImGui::Button(label))
    {
        int credit = std::stoi(creditsitems[tempCreditsItem]);
        double grade = Course::gradeToConvert(gradeItems[tempGradeItem]);
        int category = Course::categoryToConvert(categoryitems[tempCategoryItem]);
        Course::Course inputCourse = { tempCourseNameBuffer, credit, grade, category };

        onClickSave(inputCourse);

        strncpy(tempCourseNameBuffer, "\0", sizeof(tempCourseNameBuffer) - 1);
        tempCreditsItem = 0;
        tempGradeItem = 0;
        tempCategoryItem = 0;

        m_showWindow = false;
    }

    ImGui::End();
}




/*
// 한 과목의 정보 출력 윈도우
void UIManager::displayInfomationCourseWindow(const Course::Course& c)
{
    ImGui::Begin(('[' + c.courseName + "] 정보 조회").c_str(), &m_courseReadWindow);

    if (ImGui::BeginTable("courseInfoTable", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
    {
        // 열 너비 설정: 첫 열은 고정, 두 번째 열은 나머지 공간 차지
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

        // --- 과목명 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        // 첫 번째 열의 텍스트도 가운데 정렬
        alignCenter("과목명");
        ImGui::TableSetColumnIndex(1);
        ImGui::TextUnformatted(c.courseName.c_str());

        // --- 이수학점 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("이수학점");
        ImGui::TableSetColumnIndex(1);
        std::string creditsStr = std::to_string(c.credits);
        ImGui::TextUnformatted(creditsStr.c_str());

        // --- 받은점수 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("받은점수");
        ImGui::TableSetColumnIndex(1);
        char gradeBuf[16];
        snprintf(gradeBuf, sizeof(gradeBuf), "%.2f", c.grade);
        ImGui::TextUnformatted(gradeBuf);

        // --- 전공분류 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("전공분류");
        ImGui::TableSetColumnIndex(1);
        std::string categoryStr = Course::convertToCategory(c.category);
        ImGui::TextUnformatted(categoryStr.c_str());

        ImGui::EndTable();
    }

    ImGui::End();                            
}
*/