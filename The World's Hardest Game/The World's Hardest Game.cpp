// The World's Hardest Game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "The World's Hardest Game.h"
#pragma comment(lib, "msimg32.lib")
#include "Windows.h"

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
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THEWORLDSHARDESTGAME));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_THEWORLDSHARDESTGAME);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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


/// 메뉴 항목 ID 정의
#define ITEM_START_GAME          1
#define ITEM_LOAD_GAME           2
#define ITEM_SETTING             3
#define ITEM_EXIT                4

/// 타이머 ID 및 지연 시간 정의
#define TUTORIAL_TIMER_ID        100
#define TUTORIAL_DISPLAY_TIME    2000  // 2초

/// 색상 코드
/// 배경 색 LEVEL 1 ~ 20 : 181, 181, 255 ( 연한 파란색 ) / 적 : 4, 4, 242
/// 배경 색 LEVEL 21 ~ 30 : 209, 188, 245 ( 연한 보라색 ) / 적 : 79, 66, 110
/// 배경 색 LEVEL 31 ~ 40 : 240, 168, 169 ( 연한 빨간색 )  / 적 : 248, 254, 79
/// 부활 지점, 시작 지점, 도착 지점 : 182, 254, 180 ( 연한 초록색 )
/// 코인 : 255, 233, 0 ( 노란색 )

int stageColor[] = { RGB(181, 181, 255), RGB(209, 188, 245), RGB(240, 168, 169) };

/// 배경색의 전체 화면 : FillRect( ) API

/// 위에 함수 사용
void DrawBackGroundGradient(HDC hdc, HWND hwnd);

void DrawBackGroundGradient(HDC hdc, HWND hWnd)
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

/// 내용 글꼴 사용 함수
void ScreenFont(HDC hdc, HWND hWnd);

void ScreenFont(HDC hdc, HWND hWnd)
{
    HFONT Font, OldFont;

    Font = CreateFont(
        45,                        // 글꼴 높이
        0,                         // 글꼴 너비
        0,                         // 문자 기울기 각도
        0,                         // 기본 문자 기울기 각도
        FW_BOLD,                  // 글꼴 굵기
        FALSE,                     // 이탤릭체 여부
        FALSE,                     // 밑줄 여부
        FALSE,                     // 취소선 여부
        DEFAULT_CHARSET,          // 문자 집합
        OUT_DEFAULT_PRECIS,       // 출력 정밀도
        CLIP_DEFAULT_PRECIS,      // 클리핑 정밀도
        DEFAULT_QUALITY,          // 출력 품질
        DEFAULT_PITCH | FF_SWISS, // 글꼴 피치 및 패밀리
        L"Arial"                   // 글꼴 이름
	);

    OldFont = (HFONT)SelectObject(hdc, Font);

    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
};

/// 글꼴 버튼 누르기 전용 사용
void MenuFont(HDC hdc, HWND hWnd);

void MenuFont(HDC hdc, HWND hWnd)
{
    /// 메뉴 항목
    HFONT menuFont, menuOldFont;

    menuFont = CreateFont(
        35,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );
    menuOldFont = (HFONT)SelectObject(hdc, menuFont);

    SetTextColor(hdc, RGB(81, 83, 95));
    SetBkMode(hdc, TRANSPARENT);
}

/// 메뉴 정의
enum GameState {
    MENU,
    PLAYING,
    LOADING,
    SETTING,

    /// 상태 화면 만들기
    TUTORIAL_INTRO,
    TUTORIAL_BEGIN,
    TUTORIAL_GAME,
    TUTORIAL_END
};

GameState currentStage = MENU; /// 게임 시작 시 초기 상태를 메뉴로 설정한다.

/// 메뉴 항목 구조체
struct MenuItem {
    const wchar_t* text;
    int id;
    RECT rect;
};

MenuItem menuList[] = {
    { L"START GAME", ITEM_START_GAME, { 100, 300, 300, 350 } } ,
    { L"LOAD GAME", ITEM_LOAD_GAME, { 100, 370, 280, 420 } },
    { L"SETTING", ITEM_SETTING, { 100, 440, 235, 490 } },
    { L"EXIT", ITEM_EXIT, { 100, 510, 170, 560 } }
};

int menuItem_count = sizeof(menuList) / sizeof(menuList[0]);


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

    case WM_LBUTTONDOWN:
    {
        if (currentStage == MENU)
        {
            HDC hdc = GetDC(hWnd);

            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            ScreenFont(hdc, hWnd);

            for (int i = 0; i <= menuItem_count; i++)
            {
                // PtInRect 함수 : 특정 좌표가 사각형 영역 내에 있는지 확인
                // 사각형 영역 : menuList[i].rect
                // 마우스 좌표 : (mouseX, mouseY)
                // 마우스 좌표가 메뉴 항목 영역 내에 있는지 확인
                // 포인터 반환 값이 TRUE이면 마우스가 해당 메뉴 항목 위에 있음

                if (PtInRect(&menuList[i].rect, { mouseX, mouseY }))
                {
                    switch (menuList[i].id)
                    {
                    case ITEM_START_GAME:
                        /// 상태 전환 : 메뉴 -> TUTORIAL_INTRO
                        currentStage = TUTORIAL_INTRO;
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    case ITEM_LOAD_GAME:
                        currentStage = LOADING;
                        DrawBackGroundGradient(hdc, hWnd);
                        break;
                    case ITEM_SETTING:
                        currentStage = SETTING;
                        DrawBackGroundGradient(hdc, hWnd);
                        break;
                    case ITEM_EXIT:
                        DestroyWindow(hWnd);
                        break;
                    }
                    break;
                }
                else if (currentStage == TUTORIAL_INTRO)
                {

                    // 상태 전환 : TUTORIAL_INTRO -> TUTORIAL_GAME (CONTINUE 클릭)
                    // CONTINUE 버튼 클릭 시
                    if (mouseX > 550 && mouseX < 750 && mouseY > 500 && mouseY < 550)
                    {
                        currentStage = TUTORIAL_GAME;

                        /// 타이머 설정 : TUTORIAL_TIMER_ID, TUTORIAL_DISPLAY_TIME
                        /// 2초 후에 타이머 이벤트 발생
                        SetTimer(hWnd, TUTORIAL_DISPLAY_TIME, TUTORIAL_DISPLAY_TIME, NULL);

                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                }
            }

            ReleaseDC(hWnd, hdc);
        };
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        if (currentStage == MENU)
        {
            DrawBackGroundGradient(hdc, hWnd);

            /// 제목
            ScreenFont(hdc, hWnd);

            WCHAR TittleA[60] = { L"THE WORLD'S" };
            TextOut(hdc, 40, 30, TittleA, lstrlenW(TittleA));

            WCHAR TittleB[60] = { L"VERISON 1.5" };
            TextOut(hdc, 800, 170, TittleB, lstrlenW(TittleB));


            /// 메뉴 항목
            MenuFont(hdc, hWnd);

            WCHAR menuTittle[60] = {};

            for (int i = 0; i <= menuItem_count; i++)
            {
                TextOut(hdc, menuList[i].rect.left, menuList[i].rect.top, menuList[i].text, lstrlenW(menuList[i].text));
            }
        }
        else if (currentStage == TUTORIAL_INTRO)
        {
            DrawBackGroundGradient(hdc, hWnd);
            ScreenFont(hdc, hWnd);

            WCHAR tutorialIntroText[] = { L"Welcome to the World's Hardest Game!" };

            TextOut(hdc, 220, 200, tutorialIntroText, lstrlenW(tutorialIntroText));
            MenuFont(hdc, hWnd);

            WCHAR clickBack[] = { L"BACK" };
            WCHAR clickContinue[] = { L"CONTINUE" };
            WCHAR clickSpace[] = { L"(SPACE BAR)" };
            
            TextOut(hdc, 350, 400, clickBack, lstrlenW(clickBack));
            TextOut(hdc, 650, 400, clickContinue, lstrlenW(clickContinue));
        }
        else if (currentStage == TUTORIAL_BEGIN)
        {

        }
        else if (currentStage == TUTORIAL_GAME)
        {

        }
        else if (currentStage == TUTORIAL_END)
        {

        }
        else if (currentStage == PLAYING)
        {

        }
        else if (currentStage == LOADING)
        {
            DrawBackGroundGradient(hdc, hWnd);
        }
        else if (currentStage == SETTING)
        {
            DrawBackGroundGradient(hdc, hWnd);
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
