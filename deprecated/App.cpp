#include "App.h"
#include "imgui.h"                 // ImGui 핵심 API: 즉시 모드 GUI 라이브러리의 기능을 제공하는 헤더
#include "imgui_impl_win32.h"      // Win32 플랫폼 백엔드: 윈도우 이벤트(마우스, 키보드) 입력을 ImGui로 전달
#include "imgui_impl_dx11.h"       // DirectX11 렌더러 백엔드: ImGui가 생성한 드로우 데이터를 DX11로 렌더링
#include "Course.h"

#include <d3d11.h>                 // DirectX11 장치/API: GPU 생성과 명령 전송을 위한 API
#include <tchar.h>                 // 유니코드/멀티바이트 문자열 지원: Win32 API 호출 시 문자 집합 관리
#include <vector>
#include <string>
#include <iostream>
#include <functional>              // std::function을 사용하기 위해 헤더 추가
#include <Windows.h>

// — 전역 변수 —
// DirectX11 장치 및 렌더 타겟 관련 객체를 전역으로 보관하여
// 어디서나 접근 가능하게 함. 애플리케이션 수명 동안 유지되어야 함.
static ID3D11Device*           g_pd3dDevice = nullptr;           // GPU 연산과 리소스 생성을 담당하는 핵심 객체
static ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;    // GPU에 렌더링 명령을 발행하는 컨텍스트
static IDXGISwapChain*         g_pSwapChain = nullptr;           // 화면에 이미지를 표시하기 위한 전/후면 버퍼 관리 객체
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr; // 백 버퍼를 렌더링 대상으로 바인딩할 뷰

static const char* creditsitems[] = { "0", "1", "2", "3" };
static const char* gradeItems[] = { "NP", "P", "F", "D", "D+", "C", "C+", "B", "B+", "A", "A+" };
static const char* categoryitems[] = { "전공선택", "복수전공", "기초(필수)", "일반(선택)", "균형교양", "계열교양", "타전공" };

int GradeApp::start()
{

    //FreeConsole();

    // 1) Win32 창 등록 & 생성
    // WNDCLASSEXW: 윈도우 클래스를 정의하는 구조체
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
                        GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                        L"MinimalImGui", nullptr };
    
    // 윈도우 스타일 조합
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    ::RegisterClassExW(&wc); // 윈도우 클래스를 OS에 등록하여 CreateWindow 호출 준비
    HWND hwnd = ::CreateWindowW(
        wc.lpszClassName,           // 등록된 클래스 이름으로 창 생성
        L"ImGui - Grade Manager",         // 창 제목
        dwStyle,        // 기본적인 타이틀 바 + 크기 조절 가능한 윈도우 스타일
        100, 100, 1200, 800,         // 위치(100,100) 크기(800x600)
        nullptr, nullptr, wc.hInstance, nullptr);

    // 2) DirectX11 초기화 (SwapChain, Device, Context, RenderTarget 생성)
    if (!CreateDeviceD3D(hwnd))  // 장치 생성 실패 시 리턴
        return 1;
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);  // 만든 창을 화면에 표시
    ::UpdateWindow(hwnd);                // 윈도우의 클라이언트 영역을 바로 그리도록 업데이트

    // 3) ImGui 컨텍스트 생성 (입력·렌더링을 위한 전제)
    IMGUI_CHECKVERSION();                  // 헤더와 라이브러리 버전 불일치 방지
    ImGui::CreateContext();                // ImGui 내부 상태를 담을 컨텍스트 생성
    ImGuiIO& io = ImGui::GetIO();          // 입력/출력 설정에 접근
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 내비게이션 활성화

    // 한글 폰트 로드: 윈도우 기본 글꼴 gulim.ttc를 TTF처럼 로드
    // GetGlyphRangesKorean()로 한글 글리프 범위만 텍스처에 올려 메모리 절약
    ImFont* fontKorean = io.Fonts->AddFontFromFileTTF(
        "C:\\Windows\\Fonts\\MalangmalangB.ttf", 16.0f, nullptr,
        io.Fonts->GetGlyphRangesKorean()
    );
    io.FontDefault = fontKorean;  // 이후 PushFont 없이도 전체 UI에 적용
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();              // 다크 테마 스타일 적용

    // 4) 플랫폼/렌더러 백엔드 초기화
    ImGui_ImplWin32_Init(hwnd);            // Win32 이벤트(키/마우스)를 ImGui로 전달 설정
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext); // DX11에 드로우 데이터를 렌더링하도록 설정


    // 5) 메인 루프
    MSG msg;
    bool done = false;
    run(msg, done, hwnd);

    // 6) 정리
    ImGui_ImplDX11_Shutdown();         // DX11 백엔드 정리
    ImGui_ImplWin32_Shutdown();        // Win32 백엔드 정리
    ImGui::DestroyContext();           // ImGui 컨텍스트 제거
    CleanupDeviceD3D();                // DirectX 리소스 해제
    ::DestroyWindow(hwnd);             // Win32 창 제거
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance); // 클래스 등록 해제
    return 0;
}


void GradeApp::run(MSG& msg, bool& done, HWND& hwnd)
{
    // semester 초기값 설정
    std::array<Semester, 8>& semesters = gm.getSemesters();
    semester = &(semesters.at(0));
    course = &((semester->getCourses()).at(0));

    while (!done)
    {
        // — Win32 메시지 처리 —
        while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg); // 가상 키 메시지를 문자 메시지로 변환
            ::DispatchMessage(&msg);  // WndProc으로 실제 메시지 전달
            if (msg.message == WM_QUIT)
                done = true;          // WM_QUIT 수신 시 루프 탈출 예약
        }
        if (done) break;

        // — ImGui 새 프레임 시작 —
        ImGui_ImplDX11_NewFrame();   // DX11 관련 내부 타이밍/입력 처리
        ImGui_ImplWin32_NewFrame();  // Win32 메시지를 ImGui에 업데이트
        ImGui::NewFrame();           // ImGui 내부에서 위젯 준비 시작



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
        displaySemestersWindow(semesters);

        // 각 학기 내 과목 윈도우
        displayCoursesWindow(semester->getYear(), semester->getSemester(), semester->getCourses());

        // 과정 정보 윈도우
        displayTotalGradeGraph();
        //displayInfomationCourseWindow( *course );


        // 과목 수정 윈도우 
        if (m_showEditWindow)
        {
            auto editHandler = [&](Course::Course& editedCourse) {
                gm.handleFixCourse(*this->semester, *course, editedCourse);
                displayToastMessege(editedCourse.courseName + " 과목을 수정했습니다");
            };
            
            promptValueCourseWindow( *course, this->isInit, editHandler, m_showEditWindow);
        }
        // 과목 추가 윈도우
        if (m_showAddWindow)
        {
            auto addHandler = [&](Course::Course& addedCourse) {
                gm.handleAddCourse(*this->semester, addedCourse);
                displayToastMessege(addedCourse.courseName + " 과목을 추가했습니다");
            };

            Course::Course newCourse = {"", 0, 0.0, 0};
            promptValueCourseWindow( newCourse, this->isInit, addHandler, m_showAddWindow);
        }

        // Dockspace를 위한 ImGui::End()
        ImGui::End();

        // — 렌더링 단계 —
        ImGui::Render();                                  // 위젯 호출 기록으로 렌더 데이터를 생성
        const float clear_col[4] = {0.5f, 0.5f, 0.5f, 1.0f};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr); // 렌더 타겟 바인딩
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_col); // 화면 클리어
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // 렌더 데이터를 GPU에 제출

        // — 화면 출력(VSync On) —
        g_pSwapChain->Present(1, 0);  // 스왑 체인 Present: 화면에 렌더 결과 표시
    }
}


void GradeApp::displayTotalGradeGraph()
{
    std::array<Semester, 8>& semesters = gm.getSemesters();

    static float totalGrade[8] = {0};
    int index = 0;
    for (Semester& s : semesters)
    {
        float gpa = calculateGPA(s.getCourses());
        totalGrade[index++] = gpa;
    }

    /*
    // 1. 프로그램 창의 전체 크기를 가져옵니다.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = main_viewport->GetCenter();
    ImVec2 window_size = main_viewport->WorkSize;
    window_size.x *= 0.5f; window_size.y *= 0.5f;

    // 3. 창 이동, 크기 조절, 접기 등 모든 상호작용을 비활성화하는 플래그
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoFocusOnAppearing;


    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);      // 창 실행 시 위치
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always); // 창 실행 시 크기
    */

    ImGui::Begin("Graph"/*, nullptr, window_flags*/);

    ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); 
    ImGui::PlotLines("총 학점", totalGrade, 8, 0, "1학기~8학기 점수 그래프", 0.0f, 5.5f, ImVec2(0, 300));
    ImGui::PopStyleColor(); 

    ImGui::End();

}

void GradeApp::handleLoadJsonFile(HWND& hwnd)
{
    OPENFILENAME ofn;       // 공용 대화상자 구조체
    char szFile[260] = { 0, }; // 선택된 파일 경로를 저장할 버퍼

    // 구조체 초기화
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd; // 부모 윈도우 핸들
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);

    // JSON 파일만
    ofn.lpstrFilter = 
        "JSON 파일만 가능 (*.json)\0"   // 드롭다운에서 보여질 텍스트
        "*.json\0"                      // 실제로 매칭할 패턴
            "\0";                    
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST  | OFN_NOCHANGEDIR;

    // GetOpenFileNameA 함수는 ANSI 버전입니다.
    std::string filePath = "";
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        filePath = std::string(ofn.lpstrFile);
        displayToastMessege(filePath);
    }
    gm.handleLoadJson(filePath);

    // semester 초기값 설정
    std::array<Semester, 8>& semesters = gm.getSemesters();
    semester = &(semesters.at(0));
    course = &((semester->getCourses()).at(0));
    
    displayToastMessege("학기 데이터를 불러왔습니다");
}

void GradeApp::displayOptionBar(HWND& hwnd)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("파일"))
        {
            if (ImGui::MenuItem("학기 파일 불러오기"))
            {
                handleLoadJsonFile(hwnd);
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

void GradeApp::displayToastMessege(std::string messege)
{
    m_showToastMessege = true;
    start_frame = ImGui::GetFrameCount(); // 시작 프레임 기록
    toastMessege = messege;
}

void GradeApp::displayToastMessege_(const char* messege)
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

    // 가운데 정렬
    auto alignCenter = [](const char* text) {
        float columnWidth = ImGui::GetColumnWidth();
        float textWidth = ImGui::CalcTextSize(text).x;
        // 커서를 (열 너비 - 텍스트 너비)의 절반만큼 이동시켜 가운데 정렬 효과를 냅니다.
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
        ImGui::TextUnformatted(text);
    };

    ImGui::SetNextWindowBgAlpha(alpha);
    ImGui::Begin("ToastMessege", nullptr, window_flags);
    alignCenter(messege);
    ImGui::End();
}

// 모든 학기 윈도우
void GradeApp::displaySemestersWindow(std::array<Semester, 8>& semesters)
{
    /*
    // 1. 프로그램 창의 전체 크기를 가져옵니다.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = main_viewport->WorkPos;
    ImVec2 window_size = main_viewport->WorkSize;
    window_size.x *= 0.3f;

    // 2. ImGui 창의 위치와 크기를 전체 창 크기에 맞게 고정시킵니다.
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always );
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always );

    // 3. 창 이동, 크기 조절, 접기 등 모든 상호작용을 비활성화하는 플래그
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoFocusOnAppearing;
    */


    // 가운데 정렬
    auto alignCenter = [](const char* text) {
        float columnWidth = ImGui::GetColumnWidth();
        float textWidth = ImGui::CalcTextSize(text).x;
        // 커서를 (열 너비 - 텍스트 너비)의 절반만큼 이동시켜 가운데 정렬 효과를 냅니다.
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
        ImGui::TextUnformatted(text);
    };         

    std::string title = "학기 조회"; 
    ImGui::Begin(title.c_str() /*, nullptr, window_flags*/);

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
        //    가로(-1.0f): 사용 가능한 공간을 꽉 채우라는 의미입니다.
        //    세로(0): 텍스트 크기에 맞는 기본 높이를 사용합니다. (예: 30.0f로 고정해도 좋습니다)
        ImVec2 button_size = ImVec2(-1.0f, 50.0f);

        // 2. 가로로 긴 버튼을 생성합니다.
        //    버튼은 자동으로 한 줄씩 차지하며 나타납니다.
        if (ImGui::Button(semesterTitle.c_str(), button_size))
        {
            semester = &s;
        }
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::PopID();
    }

    ImGui::End();   
}

// 학기 내 과목 윈도우
void GradeApp::displayCoursesWindow(int year, int semesterNumber, std::vector<Course::Course>& courses)
{
    /* 
    // 1. 프로그램 창의 전체 크기를 가져옵니다.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = main_viewport->WorkPos;
    ImVec2 window_size = main_viewport->WorkSize;
    window_pos.x = main_viewport->GetCenter().x;
    window_size.x *= 0.5f; window_size.y *= 0.5f;

    // 2. ImGui 창의 위치와 크기를 전체 창 크기에 맞게 고정시킵니다.
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always );
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always );

    // 3. 창 이동, 크기 조절, 접기 등 모든 상호작용을 비활성화하는 플래그
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoFocusOnAppearing;
    */
    // 가운데 정렬
    auto alignCenter = [](const char* text) {
        float columnWidth = ImGui::GetColumnWidth();
        float textWidth = ImGui::CalcTextSize(text).x;
        // 커서를 (열 너비 - 텍스트 너비)의 절반만큼 이동시켜 가운데 정렬 효과를 냅니다.
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
        ImGui::TextUnformatted(text);
    };

    // 버튼을 셀 중앙에 배치하는 헬퍼 람다 함수
    auto centerAlignButton = [](const char* label) {
        float cellWidth = ImGui::GetColumnWidth();
        float buttonWidth = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
        float newCursorPosX = ImGui::GetCursorPosX() + (cellWidth - buttonWidth) * 0.5f;
        
        // 커서가 셀의 시작 위치보다 왼쪽으로 가지 않도록 방지
        if (newCursorPosX > ImGui::GetCursorPosX()) {
            ImGui::SetCursorPosX(newCursorPosX);
        }
        return ImGui::Button(label);
    };

    ImGui::Begin("과목 조회"/*, nullptr, window_flags*/);
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
            gm.handleSortCourse(*semester, 1);
        }
        ImGui::TableSetColumnIndex(1);
        if(ImGui::Button("이수학점"))
        {
            gm.handleSortCourse(*semester, 2);
        }
        ImGui::TableSetColumnIndex(2);
        if(ImGui::Button("받은점수"))
        {
            gm.handleSortCourse(*semester, 3);
        }
        ImGui::TableSetColumnIndex(3);
        if(ImGui::Button("전공분류"))
        {
            gm.handleSortCourse(*semester, 4);
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
                course = &c;
            }


            ImGui::TableSetColumnIndex(1);
            ImGui::Text(std::to_string(c.credits).c_str());
            
            ImGui::TableSetColumnIndex(2);
            std::string grade = Course::doubleToGradeString(c.grade);
            ImGui::Text(grade.c_str());
            
            ImGui::TableSetColumnIndex(3);
            std::string category = Course::categoryToString(c.category);
            ImGui::Text(category.c_str());


            ImGui::TableSetColumnIndex(4);
            if (centerAlignButton("수정"))
            {
                m_showEditWindow = true;
                m_showAddWindow = false;
                m_courseReadWindow = false;
                this->isInit = true;
                course = &c;
            }

            ImGui::TableSetColumnIndex(5);
            if (centerAlignButton("제거")) 
            {
                std::string cName = c.courseName;
                gm.handleRemoveCourse(*this->semester, c);
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
        this->isInit = true;
    }


    ImGui::End();   
}

// 한 과목의 정보 출력 윈도우
void GradeApp::displayInfomationCourseWindow(const Course::Course& c)
{
    /*
    // 1. 프로그램 창의 전체 크기를 가져옵니다.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = main_viewport->GetCenter();
    ImVec2 window_size = main_viewport->WorkSize;
    window_size.x *= 0.5f; window_size.y *= 0.5f;

    // 3. 창 이동, 크기 조절, 접기 등 모든 상호작용을 비활성화하는 플래그
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoResize | 
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoFocusOnAppearing;


    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);      // 창 실행 시 위치
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always); // 창 실행 시 크기
    */
    ImGui::Begin(('[' + c.courseName + "] 정보 조회").c_str(), &m_courseReadWindow/*, window_flags*/);
    

    // 왼쪽 정렬 (기본)
    auto alignLeft = [](const char* text) {
        // Text/TextUnformatted는 기본적으로 왼쪽 정렬입니다.
        ImGui::TextUnformatted(text);
    };

    // 가운데 정렬
    auto alignCenter = [](const char* text) {
        float columnWidth = ImGui::GetColumnWidth();
        float textWidth = ImGui::CalcTextSize(text).x;
        // 커서를 (열 너비 - 텍스트 너비)의 절반만큼 이동시켜 가운데 정렬 효과를 냅니다.
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);
        ImGui::TextUnformatted(text);
    };

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
        alignLeft(c.courseName.c_str());

        // --- 이수학점 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("이수학점");
        ImGui::TableSetColumnIndex(1);
        std::string creditsStr = std::to_string(c.credits);
        alignLeft(creditsStr.c_str());

        // --- 받은점수 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("받은점수");
        ImGui::TableSetColumnIndex(1);
        char gradeBuf[16];
        snprintf(gradeBuf, sizeof(gradeBuf), "%.2f", c.grade);
        alignLeft(gradeBuf);

        // --- 전공분류 행 ---
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        alignCenter("전공분류");
        ImGui::TableSetColumnIndex(1);
        std::string categoryStr = Course::categoryToString(c.category);
        alignLeft(categoryStr.c_str());

        ImGui::EndTable();
    }

    ImGui::End();                            
}

// Course::Course& course :  수정할 과목을 참조
// bool isInit : 입력 폼을 course로 초기값을 설정
void GradeApp::promptValueCourseWindow(
    Course::Course& course, 
    bool isInit, 
    const std::function<void(Course::Course&)>& onClickSave,
    bool& m_showWindow    
)
{
    // 해다 함수(창)이 실행 시 한번만 과목의 정보를 입력폼에 넣고, 이후에는 변경해도 상관없도록
    // 현재는 수정을 해도 계속 기존 과목 정보로 초기화가 됨
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing , ImVec2(0.5f, 0.5f));      // 창 실행 시 위치
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Appearing ); // 창 실행 시 크기

    std::string title = course.courseName.empty() ? "추가할 과목 정보 입력" : '[' + course.courseName + "] 과목 정보 입력";

    ImGui::Begin(title.c_str(), &m_showWindow);

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
        std::string gradeStr = Course::doubleToGradeString(course.grade);
        // 변환된 문자열과 일치하는 항목을 gradeItems 배열에서 찾아 인덱스를 설정
        for (int i = 0; i < IM_ARRAYSIZE(gradeItems); ++i) 
        {
            if (gradeItems[i] == gradeStr) {
                tempGradeItem = i;
                break;
            }
        }
        
        // 전공분류 콤보박스 인덱스 초기화
        std::string categoryStr = Course::categoryToString(course.category);
        for (int i = 0; i < IM_ARRAYSIZE(categoryitems); ++i) 
        {
            if (categoryitems[i] == categoryStr) {
                tempCategoryItem = i;
                break;
            }
        }
        this->isInit = false;
    }


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
        double grade = Course::stringToGradeDouble(gradeItems[tempGradeItem]);
        int category = Course::stringToCategory(categoryitems[tempCategoryItem]);
        Course::Course inputCourse = { tempCourseNameBuffer, credit, grade, category };

        onClickSave(inputCourse);

        m_showWindow = false;
    }

    ImGui::End();
}

// ------------------ DirectX11 헬퍼 함수 구현 ------------------

// 1) Device & SwapChain 생성, RenderTargetView 생성
bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount            = 2;                                 // 더블 버퍼링: 깜빡임 방지
    sd.BufferDesc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;       // RGBA 각 8비트
    sd.BufferUsage            = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 렌더 타겟으로 사용
    sd.OutputWindow           = hWnd;                              // 렌더링 윈도우 핸들
    sd.SampleDesc.Count       = 1;                                 // 멀티샘플링 비활성
    sd.Windowed               = TRUE;                              // 창 모드
    sd.SwapEffect             = DXGI_SWAP_EFFECT_DISCARD;          // 화면 전환 시 이전 버퍼 버림

    // 하드웨어 가속 디바이스 생성 시도
    if (D3D11CreateDeviceAndSwapChain(
          nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
          nullptr, 0, D3D11_SDK_VERSION, &sd,
          &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget(); // 백 버퍼에 대한 렌더 타겟 뷰 생성
    return true;
}

// 2) RenderTargetView 생성
void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)); // 백 버퍼 텍스처 가져오기
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView); // 뷰 생성
    pBackBuffer->Release();                              // 참조 해제
}

// 3) 리소스 해제
void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
void CleanupDeviceD3D()
{
    CleanupRenderTarget();                                   // 렌더 타겟 뷰 먼저 해제
    if (g_pSwapChain)           { g_pSwapChain->Release();           g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext)    { g_pd3dDeviceContext->Release();    g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice)           { g_pd3dDevice->Release();           g_pd3dDevice = nullptr; }
}


// — Win32 메시지 처리 함수 —
// ImGui_ImplWin32_WndProcHandler를 호출해야
// 키/마우스 입력이 ImGui로 전달됩니다.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // ImGui 내부에서 소비할 수 있는 메시지인지 먼저 확인
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true; // ImGui가 처리했다면 애플리케이션 로직에서는 무시

    switch (msg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0); // 창이 닫히면 메시지 루프에 종료 요청
        return 0;
    
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
            CleanupRenderTarget();
            UINT newW = (UINT)LOWORD(lParam);
            UINT newH = (UINT)HIWORD(lParam);
            g_pSwapChain->ResizeBuffers(0, newW, newH, DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();

            // 📐 Viewport 구성 – GPU에도 새 렌더링 영역 알리기
            D3D11_VIEWPORT vp;
            vp.Width    = (FLOAT)newW;
            vp.Height   = (FLOAT)newH;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0;
            vp.TopLeftY = 0;
            g_pd3dDeviceContext->RSSetViewports(1, &vp);
        }
        return 0;
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam); // 기본 핸들러 호출
}
