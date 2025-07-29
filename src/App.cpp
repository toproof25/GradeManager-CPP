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

// 과목 이름 입력 버퍼
char courseNameBuffer[256] = "";

// 이수학점 인덱스, 항목 목록
int creditsItem = 0;
const char* creditsitems[] = { "0", "1", "2", "3" };

// 받은 점수 인덱스, 항목 목록
int gradeItem = 0;
const char* gradeItems[] = { "NP", "P", "F", "D", "D+", "C", "C+", "B", "B+", "A", "A+" };

// 전공분류 인덱스, 항목 목록
int categoryItem = 0;
const char* categoryitems[] = { "전공선택", "복수전공", "부전공", "계열교양", "균형교양", "일반교양", "타전공" };


int GradeApp::start()
{
    // 1) Win32 창 등록 & 생성
    // WNDCLASSEXW: 윈도우 클래스를 정의하는 구조체
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0, 0,
                        GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                        L"MinimalImGui", nullptr };
    ::RegisterClassExW(&wc); // 윈도우 클래스를 OS에 등록하여 CreateWindow 호출 준비
    HWND hwnd = ::CreateWindowW(
        wc.lpszClassName,           // 등록된 클래스 이름으로 창 생성
        L"ImGui - Grade Manager",         // 창 제목
        WS_OVERLAPPEDWINDOW,        // 기본적인 타이틀 바 + 크기 조절 가능한 윈도우 스타일
        100, 100, 800, 600,         // 위치(100,100) 크기(800x600)
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

    ImGui::StyleColorsDark();              // 다크 테마 스타일 적용

    // 4) 플랫폼/렌더러 백엔드 초기화
    ImGui_ImplWin32_Init(hwnd);            // Win32 이벤트(키/마우스)를 ImGui로 전달 설정
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext); // DX11에 드로우 데이터를 렌더링하도록 설정

    // — 앱 상태 변수 —
    bool show_window = true;  // 체크 박스로 윈도우 On/Off 제어
    bool show_window2 = false;  // 체크 박스로 윈도우 On/Off 제어
    int  counter     = 0;     // 버튼 클릭 횟수를 저장하는 변수


    // 5) 메인 루프
    MSG msg;
    bool done = false;
    run(msg, done);

    // 6) 정리
    ImGui_ImplDX11_Shutdown();         // DX11 백엔드 정리
    ImGui_ImplWin32_Shutdown();        // Win32 백엔드 정리
    ImGui::DestroyContext();           // ImGui 컨텍스트 제거
    CleanupDeviceD3D();                // DirectX 리소스 해제
    ::DestroyWindow(hwnd);             // Win32 창 제거
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance); // 클래스 등록 해제
    return 0;
}


void GradeApp::run(MSG& msg, bool& done)
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



        /* ------------------------- UI 렌더링 부분 ------------------------- */
        
        displaySemestersWindow(semesters);

        if (coursesListWindow)
            displayCoursesWindow(semester->getYear(), semester->getSemester(), semester->getCourses());

        if (courseReadWindow)
            displayInfomationCourseWindow( *course );

        if (courseFixWindow)
            displayFixValueCourseWindow( *fixCurse );

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

// 모든 학기 윈도우
void GradeApp::displaySemestersWindow(std::array<Semester, 8>& semesters)
{
    std::string title = "학기 조회"; 
    ImGui::Begin(title.c_str());

    if (ImGui::BeginTable("courseInfoTable", 1, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
    {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);

        int buttonId = 10;
        for (Semester& s : semesters)
        {
            ImGui::PushID(buttonId++); 

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            std::string semesterTitle = std::to_string(s.getYear()) + "학년 " + std::to_string(s.getSemester()) + "학기";

            if (ImGui::Button(semesterTitle.c_str()))
            {
                semester = &s;
                coursesListWindow = true;
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();   
}

// 학기 내 과목 윈도우
void GradeApp::displayCoursesWindow(int year, int semesterNumber, std::vector<Course::Course>& courses)
{
    std::string title = std::to_string(year) + "학년 " + std::to_string(semesterNumber) + "학기 과목 조회"; 
    ImGui::Begin(title.c_str(), &coursesListWindow);

    if (ImGui::BeginTable("courseInfoTable", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
    {
        // 열 너비 설정: 첫 열은 고정, 두 번째 열은 나머지 공간 차지
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 350.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 50.f);

        int buttonId = 0;
        for (Course::Course& c : courses)
        {
            ImGui::PushID(buttonId++); 

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            std::string courseTitle = c.courseName + " / " + std::to_string(c.credits) + " / " + Course::convertToGrade(c.grade) + " / " + Course::convertToCategory(c.category);

            if (ImGui::Button(courseTitle.c_str()))
            {
                courseReadWindow = true;
                course = &c;
            }

            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("수정"))
            {
                courseFixWindow = true;
                fixCurse = &c;
            }

            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("제거")) 
            {
                gm.handleRemoveCourse(*this->semester, c);
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();   
}

// 한 과목의 정보 출력 윈도우
void GradeApp::displayInfomationCourseWindow(const Course::Course& c)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);      // 창 실행 시 위치
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_Appearing); // 창 실행 시 크기
    ImGui::Begin(('[' + c.courseName + "] 정보 조회").c_str(), &courseReadWindow);

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
        std::string categoryStr = Course::convertToCategory(c.category);
        alignLeft(categoryStr.c_str());

        ImGui::EndTable();
    }

    ImGui::End();                            
}


void GradeApp::displayFixValueCourseWindow(Course::Course& fixCourse)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);      // 창 실행 시 위치
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_Appearing); // 창 실행 시 크기
    ImGui::Begin(('[' + fixCourse.courseName + "] 과목 정보 수정").c_str(), &courseFixWindow);

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



    // 1. 과목명 버퍼 초기화
    // 기존 버퍼를 0으로 깨끗하게 지우고, fixCourse의 과목명을 안전하게 복사합니다.
    memset(courseNameBuffer, 0, sizeof(courseNameBuffer));
    strncpy(courseNameBuffer, fixCourse.courseName.c_str(), sizeof(courseNameBuffer) - 1);

    // 2. 이수학점 콤보박스 인덱스 초기화
    // fixCourse.credits(int)를 creditsitems(const char* [])의 인덱스로 직접 사용합니다.
    // (creditsitems가 "0", "1", "2", "3" 순서로 되어있으므로 가능)
    if (fixCourse.credits >= 0 && fixCourse.credits < IM_ARRAYSIZE(creditsitems)) 
    {
        creditsItem = fixCourse.credits;
    }

    // 3. 받은 점수 콤보박스 인덱스 초기화
    // Course::convertToGrade 함수를 사용해 double 점수를 "A+" 같은 문자열로 변환
    std::string gradeStr = Course::convertToGrade(fixCourse.grade);
    // 변환된 문자열과 일치하는 항목을 gradeItems 배열에서 찾아 인덱스를 설정
    for (int i = 0; i < IM_ARRAYSIZE(gradeItems); ++i) 
    {
        if (gradeItems[i] == gradeStr) {
            gradeItem = i;
            break;
        }
    }
    
    // 4. 전공분류 콤보박스 인덱스 초기화
    std::string categoryStr = Course::convertToCategory(fixCourse.category);
    // 변환된 문자열과 일치하는 항목을 categoryitems 배열에서 찾아 인덱스를 설정
    for (int i = 0; i < IM_ARRAYSIZE(categoryitems); ++i) 
    {
        if (categoryitems[i] == categoryStr) {
            categoryItem = i;
            break;
        }
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
        ImGui::InputText("##CourseName", courseNameBuffer, IM_ARRAYSIZE(courseNameBuffer));
        ImGui::PopItemWidth();

        // — 2행 이수학점 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("이수학점");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Credits", &creditsItem, creditsitems, IM_ARRAYSIZE(creditsitems));

        // — 3행 받은 점수 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("받은 점수");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Grade", &gradeItem, gradeItems, IM_ARRAYSIZE(gradeItems));

        // — 4행 전공분류 —
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("전공분류");
        ImGui::TableSetColumnIndex(1);
        ImGui::Combo("##Category", &categoryItem, categoryitems, IM_ARRAYSIZE(categoryitems));

        ImGui::EndTable();
    }

    // 저장 버튼 중앙 배치
    const char* label = "저장";
    float region = ImGui::GetContentRegionAvail().x;
    float txtW = ImGui::CalcTextSize(label).x;
    ImGui::SetCursorPosX((region - txtW) * 0.5f);
    if (ImGui::Button(label))
    {
        int credit = std::stoi(creditsitems[creditsItem]);
        double grade = Course::gradeToConvert(gradeItems[gradeItem]);
        int category = Course::categoryToConvert(categoryitems[categoryItem]);

        Course::Course fixedCourse = { courseNameBuffer, credit, grade, category };

        //gm.handleFixCourse(*this->semester, *this->course, fixCourse);

        std::cout << "입력된 과목명: " << fixedCourse.courseName
                  << ", 선택 학점: " << fixedCourse.credits
                  << ", 반은 점수: " << fixedCourse.grade
                  << ", 전공 분류: " << fixedCourse.category
                  << std::endl;
        
        courseFixWindow = false;
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
