// The World's Hardest Game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "The World's Hardest Game.h"
#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THEWORLDSHARDESTGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEWORLDSHARDESTGAME));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THEWORLDSHARDESTGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THEWORLDSHARDESTGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

/// 색상 코드
/// 배경 색 LEVEL 1 ~ 20 : 181, 181, 255 ( 연한 파란색 ) / 적 : 4, 4, 242
/// 배경 색 LEVEL 21 ~ 30 : 209, 188, 245 ( 연한 보라색 ) / 적 : 79, 66, 110
/// 배경 색 LEVEL 31 ~ 40 : 240, 168, 169 ( 연한 빨간색 )  / 적 : 248, 254, 79
/// 부활 지점, 시작 지점, 도착 지점 : 182, 254, 180 ( 연한 초록색 )
/// 코인 : 255, 233, 0 ( 노란색 )

int stageColor[] = { RGB(181, 181, 255), RGB(209, 188, 245), RGB(240, 168, 169) };

/// 배경색의 전체 화면 : FillRect( ) API

/// 메뉴 정의
enum GAME_STATE {
    MENU,
    PLAYING,
    LOADING,
    SETTING
};

GAME_STATE currentStage = MENU; /// 게임 시작 시 초기 상태를 메뉴로 설정한다.

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            if (currentStage == MENU)
            {
                RECT clientRect;
                GetClientRect(hWnd, &clientRect);

                COLORREF colorStart, colorEnd;

                /// 상단 시작 색상 : 연한 보라색 RGB(210, 210, 255)
                colorStart = RGB(210, 210, 255);
                /// 하단 끝 색상 : 흰색 RGB(255, 255, 255)
                colorEnd = RGB(255, 255, 255);

                /// 구조체 정의 : TRIVERTEX - 그라데이션의 정점, 색상 경계점 정의
                TRIVERTEX vertex[2];

                /// --- 정점 [0] : 상단 시작 ---
                vertex[0].x = clientRect.left;
                vertex[0].y = clientRect.top;
                // GDI+ : 16비트 RGB 값을 사용, 8비트 RGB 값에 256(1<<8)을 곱해 확장
                vertex[0].Red = GetRValue(colorStart) << 8;
                vertex[0].Green = GetGValue(colorStart) << 8;
                vertex[0].Blue = GetBValue(colorStart) << 8;
                vertex[0].Alpha = 0x000;

                /// --- 정점 [1] : 하단 끝 ---
                vertex[1].x = clientRect.right;
                vertex[1].y = clientRect.bottom;

                vertex[1].Red = GetRValue(colorEnd) << 8;
                vertex[1].Green = GetGValue(colorEnd) << 8;
                vertex[1].Blue = GetBValue(colorEnd) << 8;
                vertex[1].Alpha = 0x000;

                /// 구조체 정의 : GRADIENT_RECT - 채울 영역과 정점 연결
                GRADIENT_RECT gRect;
                gRect.UpperLeft = 0;        // 정점 0 - 상단을 영역의 왼쪽 위
                gRect.LowerRight = 1;       // 정점 1 - 하단을 영역의 오른쪽 아래

                /// GradientFill 함수 호출
                /// GRADIENT_FILL_RECT_V - 채우기 모기
                GradientFill(hdc, vertex, 2 /*사용할 정점*/, &gRect, 1 /*채울 영역*/, GRADIENT_FILL_RECT_V);

            }
            else if (currentStage == PLAYING)
            {
               
            }
            else if (currentStage == LOADING)
            {

            }
            else if (currentStage == SETTING)
            {

            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
