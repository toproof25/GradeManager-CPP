#pragma once
#include "imgui.h"                 // ImGui 핵심 API: 즉시 모드 GUI 라이브러리의 기능을 제공하는 헤더
#include "imgui_impl_win32.h"      // Win32 플랫폼 백엔드: 윈도우 이벤트(마우스, 키보드) 입력을 ImGui로 전달
#include "imgui_impl_dx11.h"       // DirectX11 렌더러 백엔드: ImGui가 생성한 드로우 데이터를 DX11로 렌더링
#include "Course.h"
#include "Semester.h"
#include "GradeManager.h"
#include "UIManager.h"

#include <d3d11.h>                 // DirectX11 장치/API: GPU 생성과 명령 전송을 위한 API
#include <tchar.h>                 // 유니코드/멀티바이트 문자열 지원: Win32 API 호출 시 문자 집합 관리
#include <vector>
#include <array>
#include <string>

class Application
{
  
private:
  ID3D11Device*           g_pd3dDevice           = nullptr;   // GPU 연산과 리소스 생성을 담당하는 핵심 객체
  ID3D11DeviceContext*    g_pd3dDeviceContext    = nullptr;   // GPU에 렌더링 명령을 발행하는 컨텍스트
  IDXGISwapChain*         g_pSwapChain           = nullptr;   // 화면에 이미지를 표시하기 위한 전/후면 버퍼 관리 객체
  ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;   // 백 버퍼를 렌더링 대상으로 바인딩할 뷰
  
  // Composition으로 관리
  GradeManager gradeManager;
  UIManager uiManager;

public:
  Application();
  int start();
  void run(MSG& msg, bool& done, HWND& hwnd);


  // WNDPROC에 등록할 static 함수와 메시지 처리 함수
  static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:
  // ImGui에 필요한 함수들
  bool CreateDeviceD3D(HWND hWnd);
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
};

