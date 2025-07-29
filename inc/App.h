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
static ID3D11Device*           g_pd3dDevice = nullptr;           // GPU 연산과 리소스 생성을 담당하는 핵심 객체
static ID3D11DeviceContext*    g_pd3dDeviceContext = nullptr;    // GPU에 렌더링 명령을 발행하는 컨텍스트
static IDXGISwapChain*         g_pSwapChain = nullptr;           // 화면에 이미지를 표시하기 위한 전/후면 버퍼 관리 객체
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr; // 백 버퍼를 렌더링 대상으로 바인딩할 뷰

// 1) 상태 변수 (프레임 루프 밖에 static으로 두면 좋습니다)
static char   inputBuf[256] = "";           // 입력 텍스트 버퍼 (최대 255자)
static std::vector<std::string> items;      // 추가된 문자열을 저장할 리스트

// 함수 전방 선언
// 구현부를 아래로 내려서 가독성을 높이기 위해 미리 선언함.
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);


class GradeApp
{
private:
  GradeManager gm;

  // — 앱 상태 변수 —
  bool courseReadWindow = false;
  
  Semester* semester = nullptr;
  Course::Course* course = nullptr;
  
  bool show_window = true;    // 체크 박스로 윈도우 On/Off 제어
  bool show_window2 = false;  // 체크 박스로 윈도우 On/Off 제어
  int  counter     = 0;       // 버튼 클릭 횟수를 저장하는 변수

  // 메뉴1 - 모든 학기 보기
  // 메뉴2 - 선택한 학기 정보 보기
  // 메뉴3 - 성적 보기

public:
  int start();
  void run(MSG& msg, bool& done);

  // 메인 창 - json불러와서 보여짐 - 1학기~8학기로 나열되어있고, 학기 눌렀을 때 해당 학기 정보(과목, 성적)이 나타남
  // 

  void displaySemesters(std::array<Semester, 8>& semesters);

  void displayCourses(int year, int semester, std::vector<Course::Course>& courses); // 한 학기의 과목 출력
  void displayInfomationCourse(const Course::Course& c);                             // 한 과목의 정보 출력

};