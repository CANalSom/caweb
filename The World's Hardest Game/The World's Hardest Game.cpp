// The World's Hardest Game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "The World's Hardest Game.h"
#pragma comment(lib, "msimg32.lib")
#include "Windows.h"
#include "time.h"

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




/// 맵
#define MAP_WIDTH  700
#define MAP_HEIGHT 300
RECT g_mapRect = { 250, 150, 1000, 525 };

HBITMAP mapLv1 = NULL;
HDC memDCMap1 = NULL;


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    /// 창 고정하기
    const int ClientWidth = 1200;
	const int ClientHeight = 650;


    /// 창 스타일에 WS_THICKFRAME(크기 조절 가능)과 WS_MAXIMIZEBOX(최대화 버튼)를 제외
    /// WS_CAPTION(제목) | WS_SYSMENU(시스템 메뉴) | WS_MINIMIZEBOX(최소화 버튼) | WS_BORDER(테두리)
    DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;

	RECT windowRect = { 0, 0, ClientWidth, ClientHeight };

    /// 클라이언트 영역 크기에 맞춰 창의 전체 크기 계산.
    /// (제목 표시줄과 테두리 두께가 포함된 최종 창 크기)
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    int FinalWidth = windowRect.right - windowRect.left;
	int FinalHeight = windowRect.bottom - windowRect.top;

    /// 화면 중앙에 생성
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int startX = (screenWidth - FinalWidth) / 2;
    int startY = (screenHeight - FinalHeight) / 2;


    HWND hWnd = CreateWindowW(
        szWindowClass, 
        szTitle, 
        dwStyle,
        startX,
        startY,
        FinalWidth,
        FinalHeight,
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr
    );

    if (!hWnd)
    {
        return FALSE;
    }
    
    mapLv1 = (HBITMAP)LoadImageW(hInstance, L"image\\Level1.bmp", IMAGE_BITMAP, MAP_WIDTH, MAP_HEIGHT, LR_LOADFROMFILE);
    if (mapLv1 == NULL)
    {
        // 로드가 실패하면 이 메시지 박스가 떠야 합니다.
        MessageBox(hWnd, L"이미지 로드 실패! 경로를 확인하세요.", L"ERROR", MB_OK | MB_ICONERROR);
        return FALSE; // 로드 실패 시 프로그램 종료
    }
	HDC hdc = GetDC(hWnd);
	memDCMap1 = CreateCompatibleDC(hdc);
	SelectObject(memDCMap1, mapLv1);
	ReleaseDC(hWnd, hdc);

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

/// 취소 및 계속
#define ITEM_FIRSTSCREENT        5
#define ITEM_PLAY                6


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

void HardestGameFont(HDC hdc, HWND hwnd)
{
    HFONT HGFont, OldHGFont;

    HGFont = CreateFont(
        120, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial Black"
    );

    OldHGFont = (HFONT)SelectObject(hdc, HGFont);
    SetBkMode(hdc, TRANSPARENT);

 
    SetTextColor(hdc, RGB(0, 0, 0)); 
    TextOut(hdc, 45, 60, L"HARDEST GAME", lstrlenW(L"HARDEST GAME")); 

    SetTextColor(hdc, RGB(58, 109, 184)); 
    TextOut(hdc, 40, 55, L"HARDEST GAME", lstrlenW(L"HARDEST GAME"));

    SelectObject(hdc, HGFont);
    DeleteObject(OldHGFont);
}

/// 글꼴 버튼 누르기 전용 사용
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
    TUTORIAL_INTRO
};

GameState currentStage = MENU; /// 게임 시작 시 초기 상태를 메뉴로 설정한다.

/// 메뉴 항목 구조체
typedef struct {
    const wchar_t* text;
    int id;
    RECT rect;
} ButtonClick;

ButtonClick menuList[] = {
    { L"START GAME", ITEM_START_GAME, { 100, 300, 300, 350 } } ,
    { L"LOAD GAME", ITEM_LOAD_GAME, { 100, 370, 280, 420 } },
    { L"SETTING", ITEM_SETTING, { 100, 440, 235, 490 } },
    { L"EXIT", ITEM_EXIT, { 100, 510, 170, 560 } }
};

ButtonClick clickItem[] = {
    L"BACK", ITEM_FIRSTSCREENT, { 350, 400, 450, 450 },
    L"CONTINUE", ITEM_PLAY, { 650, 400, 800, 450 }
};

int menuItem_count = sizeof(menuList) / sizeof(menuList[0]);
int clickItem_count = sizeof(clickItem) / sizeof(clickItem[0]);


/// 게임 스테이지 선언
int g_currentLevel = 1;

void Level1(HDC hdc)
{
    int mapX = g_mapRect.left;
    int mapY = g_mapRect.top;

    if (memDCMap1)
    {
        BitBlt(hdc, mapX, mapY, MAP_WIDTH, MAP_HEIGHT, memDCMap1, 0, 0, SRCCOPY);
    }
}

void Level2(HDC hdc)
{

}

/// 플레이어
int playerX = 100;
int playerY = 100;
const int playerSpeed = 5;

bool bKeyDown[256] = { false };

void PlayerPosition()
{
    int dx = 0;
    int dy = 0;

    if (bKeyDown[VK_LEFT])
    {
        dx -= playerSpeed;
	} else if(bKeyDown[VK_RIGHT])
    {
        dx += playerSpeed;
    }
    if (bKeyDown[VK_UP])
    {
        dy -= playerSpeed;
    }
    else if (bKeyDown[VK_DOWN])
    {
        dy += playerSpeed;
    }

    if (dx != 0 && dy != 0)
    {
        int diagonalSpeed = (int)(playerSpeed * 0.707);
        if (dx < 0) dx = -diagonalSpeed;
		if (dx > 0) dx = diagonalSpeed;
		if (dy < 0) dy = -diagonalSpeed;
		if (dy > 0) dy = diagonalSpeed;

        playerX += dx;
        playerY += dy;
    }
}

const int playerTotalSize = 37;
const int playerBorderThickness = 5;
const int playerInnerSize = playerTotalSize - (playerBorderThickness * 2);

void drawPlayer(HDC hdc)
{
    int left = playerX - playerTotalSize / 2;
	int top = playerY - playerTotalSize / 2;
	int right = playerX + playerTotalSize / 2;
	int bottom = playerY + playerTotalSize / 2;

    HPEN BlackPen, BlackOldPen;
    BlackPen = CreatePen(PS_SOLID, playerBorderThickness, RGB(0, 0, 0));
	BlackOldPen = (HPEN)SelectObject(hdc, BlackPen);
    
    HBRUSH RedBrush;
	RedBrush = CreateSolidBrush(RGB(255, 0, 0));
    
    HBRUSH BlackBrush, BlackOldBrush;
	BlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	BlackOldBrush = (HBRUSH)SelectObject(hdc, BlackBrush);

    Rectangle(hdc, left, top, right, bottom);

    SelectObject(hdc, GetStockObject(NULL_PEN));
	SelectObject(hdc, RedBrush);

    Rectangle(hdc, 
        left + playerBorderThickness, 
        top + playerBorderThickness,
		right - playerBorderThickness, 
        bottom - playerBorderThickness
    );

	SelectObject(hdc, BlackOldPen);
    SelectObject(hdc, BlackOldBrush);
    DeleteObject(BlackPen);
    DeleteObject(BlackBrush);
	DeleteObject(RedBrush);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
    {
        bKeyDown[wParam] = true;
    }
    break;
    case WM_KEYUP:
    {
        bKeyDown[wParam] = false;
    }
    break;
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
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);
        HDC hdc = GetDC(hWnd);
        ScreenFont(hdc, hWnd);

        if (currentStage == MENU)
        {
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
            }
        }
        else if (currentStage == TUTORIAL_INTRO)
        {
            for (int i = 0; i <= clickItem_count; i++)
            {
                if (PtInRect(&clickItem[i].rect, { mouseX, mouseY }))
                {
                    switch (clickItem[i].id)
                    {
                    case ITEM_FIRSTSCREENT:
                        currentStage = MENU;
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    case ITEM_PLAY:
                        currentStage = PLAYING;
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    }
                    break;
                }
            }
            ReleaseDC(hWnd, hdc);
        };
    }
    break;
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 1000 / 60, NULL);
    }
    break;
    case WM_TIMER:
    {
        PlayerPosition();
        InvalidateRect(hWnd, NULL, TRUE);
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

            WCHAR TittleB[60] = { L"VERISON 0.5" };
            TextOut(hdc, 800, 170, TittleB, lstrlenW(TittleB));

			HardestGameFont(hdc, hWnd);


            /// 메뉴 항목
            MenuFont(hdc, hWnd);

            for (int i = 0; i < menuItem_count; i++)
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

            for (int i = 0; i <= clickItem_count; i++)
            {
                TextOut(hdc, clickItem[i].rect.left, clickItem[i].rect.top, clickItem[i].text, lstrlenW(clickItem[i].text));
			}
        }
        else if (currentStage == PLAYING)
        {
            HBRUSH PlayingBrush = NULL;

            if (g_currentLevel >= 1 && g_currentLevel < 5)
            {
                PlayingBrush = CreateSolidBrush(RGB(181, 181, 255));

				FillRect(hdc, &ps.rcPaint, PlayingBrush);
            }
            else if (g_currentLevel >= 5 && g_currentLevel < 9)
            {
                PlayingBrush = CreateSolidBrush(RGB(214, 188, 249));

                FillRect(hdc, &ps.rcPaint, PlayingBrush);
            }
            else if (g_currentLevel >= 9 && g_currentLevel <= 10)
            {
                PlayingBrush = CreateSolidBrush(RGB(247, 171, 171));

                FillRect(hdc, &ps.rcPaint, PlayingBrush);
            }
            else if (g_currentLevel == 11)
            {
                /// 클리어 화면
            }

            if (g_currentLevel == 1)
            {
                Level1(hdc);
                drawPlayer(hdc);
            }
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
        if (memDCMap1)
        {
            DeleteDC(memDCMap1);
        }
        if (mapLv1)
        {
			DeleteObject(mapLv1);
        }
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
