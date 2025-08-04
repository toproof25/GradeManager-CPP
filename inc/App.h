#pragma once
#include "imgui.h"                 // ImGui 핵심 API: 즉시 모드 GUI 라이브러리의 기능을 제공하는 헤더
#include "imgui_impl_win32.h"      // Win32 플랫폼 백엔드: 윈도우 이벤트(마우스, 키보드) 입력을 ImGui로 전달
#include "imgui_impl_dx11.h"       // DirectX11 렌더러 백엔드: ImGui가 생성한 드로우 데이터를 DX11로 렌더링
#include "Course.h"
#include "Semester.h"
#include "GradeManager.h"

#include <d3d11.h>                 // DirectX11 장치/API: GPU 생성과 명령 전송을 위한 API
#include <tchar.h>                 // 유니코드/멀티바이트 문자열 지원: Win32 API 호출 시 문자 집합 관리
#include <vector>
#include <array>
#include <string>

// — 전역 변수 —
// DirectX11 장치 및 렌더 타겟 관련 객체를 전역으로 보관하여
// 어디서나 접근 가능하게 함. 애플리케이션 수명 동안 유지되어야 함.
extern ID3D11Device*           g_pd3dDevice;           // GPU 연산과 리소스 생성을 담당하는 핵심 객체
extern ID3D11DeviceContext*    g_pd3dDeviceContext;    // GPU에 렌더링 명령을 발행하는 컨텍스트
extern IDXGISwapChain*         g_pSwapChain;           // 화면에 이미지를 표시하기 위한 전/후면 버퍼 관리 객체
extern ID3D11RenderTargetView* g_mainRenderTargetView; // 백 버퍼를 렌더링 대상으로 바인딩할 뷰

// 과목 이름 입력 버퍼
extern char courseNameBuffer[256];

// 이수학점 인덱스, 항목 목록
extern int creditsItem;
extern const char* creditsitems[];

// 받은 점수 인덱스, 항목 목록
extern int gradeItem;
extern const char* gradeItems[];


// 전공분류 인덱스, 항목 목록
extern int categoryItem;
extern const char* categoryitems[];


class GradeApp
{
private:
  GradeManager gm;

  // — 앱 상태 변수 —
  bool m_showAddWindow = false;
  bool m_showEditWindow = false;

  bool m_courseReadWindow = false;

  bool isInit = false;
  bool courseFixWindow = false;

  Semester* semester = nullptr;
  Course::Course* course = nullptr;

  std::string toastMessege;

  // --- 프레임 계산 ---
  bool m_showToastMessege = false;
  int start_frame = 0; // 알림이 시작된 프레임 번호
  const int VISIBLE_FRAMES = 180;  // 완전히 보이는 프레임 수 (약 1초)
  const int FADE_FRAMES = 240;    // 사라지는 데 걸리는 프레임 수 (약 2초)

  void displayToastMessege_(const char* messege);


public:
  int start();
  void run(MSG& msg, bool& done);

  // 메인 창 - json불러와서 보여짐 - 1학기~8학기로 나열되어있고, 학기 눌렀을 때 해당 학기 정보(과목, 성적)이 나타남
  // 

  void displaySemestersWindow(std::array<Semester, 8>& semesters);  // 모든 학기 윈도우

  void displayCoursesWindow(int year, int semester, std::vector<Course::Course>& courses); // 한 학기의 과목 출력
  void displayInfomationCourseWindow(const Course::Course& c);                             // 한 과목의 정보 출력

  void promptValueCourseWindow(
    Course::Course& fixCourse, 
    bool isInit, 
    const std::function<void(Course::Course&)>& onClickSave,
    bool& m_showWindow    
  );


  void displayToastMessege(std::string messege);

};


// ImGui에 필요한 함수들
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);