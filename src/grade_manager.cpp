#include "Application.h" 
#include <Windows.h> 

// 콘솔을 완전히 지우기 위해 윈도우 GUI 애플리케이션으로 설정
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  Application app;
  app.start();
  return 0;
}

/*
#include "Application.h" 

int main()
{
  Application app;
  app.start();
}
*/


