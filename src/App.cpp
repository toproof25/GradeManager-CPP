#include "App.h"
#include "imgui.h"                 // ImGui 핵심 API: 즉시 모드 GUI 라이브러리의 기능을 제공하는 헤더
#include "imgui_impl_win32.h"      // Win32 플랫폼 백엔드: 윈도우 이벤트(마우스, 키보드) 입력을 ImGui로 전달
#include "imgui_impl_dx11.h"       // DirectX11 렌더러 백엔드: ImGui가 생성한 드로우 데이터를 DX11로 렌더링
#include <d3d11.h>                 // DirectX11 장치/API: GPU 생성과 명령 전송을 위한 API
#include <tchar.h>                 // 유니코드/멀티바이트 문자열 지원: Win32 API 호출 시 문자 집합 관리

#include <vector>
#include <string>
#include <iostream>


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


    std::array<Semester, 8>& semesters = gm.getSemesters();
    displayCourses(semesters.at(0).getCourses());


    if (courseReadWindow)
    {
        displayInfomationCourse( *course );
    }


    // — 간단 창 예제 —
    if (show_window)
    {
        ImGui::Begin("간단 창", &show_window); // 창 시작, 닫기 버튼으로 show_window 업데이트
        ImGui::Text("ImGui 기본 입문 예제");     // 화면에 텍스트 출력
        if (ImGui::Button("카운터 증가"))         // 버튼을 렌더링, 클릭 시 true 반환
            counter++;                            // 클릭이 감지되면 카운터 증가
        ImGui::Text("클릭 횟수: %d", counter);   // 현재 카운터 값을 출력
        ImGui::End();                             // 창 닫기 (Begin과 쌍)
    }

    // 키보드 입력 감지 (스페이스바를 누르면)
    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
        counter++;
    }

    // — 간단 창 예제 —
    if (show_window2)
    {
        // 3) 입력창 + 버튼 + 리스트 표시
        ImGui::Begin("텍스트 입력 예제", &show_window2);  

            // 텍스트 입력창: 마우스로 클릭해서 포커스, 키보드 입력 가능
            ImGui::InputText("입력", inputBuf, IM_ARRAYSIZE(inputBuf));
            
            // 같은 줄에 버튼을 배치
            ImGui::SameLine();
            if (ImGui::Button("추가") || ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                // 비어 있지 않은 입력만 처리
                if (inputBuf[0] != '\0')
                {
                    items.emplace_back(inputBuf);  // 리스트에 추가
                    for (char& c : inputBuf)
                        c = '\0';
                }
            }

            ImGui::Separator();  // 구분선

            // 추가된 문자열들을 한 줄씩 표시
            for (size_t i = 0; i < items.size(); i++)
            {
                ImGui::Text("%zu: %s", i + 1, items[i].c_str());
            }

        ImGui::End();
    }


    // — 렌더링 단계 —
    ImGui::Render();                                  // 위젯 호출 기록으로 렌더 데이터를 생성
    const float clear_col[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr); // 렌더 타겟 바인딩
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_col); // 화면 클리어
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // 렌더 데이터를 GPU에 제출

    // — 화면 출력(VSync On) —
    g_pSwapChain->Present(1, 0);  // 스왑 체인 Present: 화면에 렌더 결과 표시
  }
}

void GradeApp::displayCourses(std::vector<Course::Course>& courses)
{
    ImGui::Begin("n학년 n학기 과목 조회");

    if (ImGui::BeginTable("courseInfoTable", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg))
    {
        // 열 너비 설정: 첫 열은 고정, 두 번째 열은 나머지 공간 차지
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 50.f);

        int buttonId = 0;
        for (Course::Course& c : courses)
        {
            ImGui::PushID(buttonId++); 

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            //ImGui::Text(c.courseName.c_str());
            if (ImGui::Button(c.courseName.c_str()))
            {
                courseReadWindow = true;
                course = &c;
            }
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("수정"))
                counter++;
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("제거")) 
                counter++;

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::End();   
}


void GradeApp::displayInfomationCourse(const Course::Course& c)
{
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Once);
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
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
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



// — DirectX11 헬퍼 함수 구현 —

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
