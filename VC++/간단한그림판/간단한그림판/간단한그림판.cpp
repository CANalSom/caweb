// 간단한그림판.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "간단한그림판.h"

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
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
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


HWND g_button;

#define IDM_CLICK_BUTTON    999

int g_flag;
int g_push;
int g_x;
int g_y;
int g_color = RGB(0, 0, 0);
int g_colorface = RGB(255, 255, 255);
int g_brush = 1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        g_button = CreateWindow(L"Button", L"지우기", WS_CHILD | WS_VISIBLE, 10, 10, 60, 30, hWnd, (HMENU)IDM_CLICK_BUTTON, hInst, NULL);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        g_push = TRUE;

        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);
    }
        break;
    case WM_LBUTTONUP:
    {
        g_push = FALSE;

        int x, y;
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        HDC hdc = GetDC(hWnd);

        HPEN myPen;
        HBRUSH myBrush;

        myPen = CreatePen(PS_SOLID, g_brush, g_color);
        SelectObject(hdc, myPen);

        myBrush = CreateSolidBrush(g_colorface);
        SelectObject(hdc, myBrush);
        
        /// 그리기 모양
        if (g_flag == 0)
        {
            break;
        }
        if (g_flag == 1)   /// 선형
        {
            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);
        }
        else if (g_flag == 2)   /// 사각형
        {
            Rectangle(hdc, g_x, g_y, x, y);
        }
        else if (g_flag == 3)   /// 타원형
        {
            Ellipse(hdc, g_x, g_y, x, y);
        }

        g_x = x;
        g_y = y;

        DeleteObject(myPen);
        DeleteObject(myBrush);

        ReleaseDC(hWnd, hdc);
    }
        break;
    case WM_MOUSEMOVE:
        {
        if ( (g_push == TRUE) && (g_flag == 4) )
            {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            HDC hdc = GetDC(hWnd);

            HPEN myPen;

            myPen = CreatePen(PS_SOLID, g_brush, g_color);
            SelectObject(hdc, myPen);

            MoveToEx(hdc, g_x, g_y, NULL);
            LineTo(hdc, x, y);

            g_x = x;
            g_y = y;

            DeleteObject(myPen);

            ReleaseDC(hWnd, hdc);
            
            }
        }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            /// 선 선택
            case ID_LINE:
                g_flag = 1;
                break;

            case ID_RECTANGLE_LINE:
                g_flag = 2;
                break;

            case ID_ELLIPSE_LINE:
                g_flag = 3;
                break;

            case ID_FREE_LINE:
                g_flag = 4;
                break;
            
            /// 색상 선택
            case ID_RED:
                g_color = RGB(255, 0, 0);
                break;

            case ID_BLUE:
                g_color = RGB(0, 0, 255);
            break;

            case ID_GREEN:
                g_color = RGB(0, 255, 0);
                break;

            case ID_BLACK:
                g_color = RGB(0, 0, 0);
                break;

            case ID_BLACKFACE:
                g_colorface = RGB(0, 0, 0);
                break;

            case ID_WHITE:
                g_colorface = RGB(255, 255, 255);
                break;

            case ID_CRIMSON:
                g_colorface = RGB(200, 100, 100);
                break;

            /// 굵기 선택
            case ID_1PX:
                g_brush = 1;
                break;

            case ID_5PX:
                g_brush = 5;
                break;

            case ID_10PX:
                g_brush = 10;
                break;

            /// 지우기 버튼
            case IDM_CLICK_BUTTON:
                InvalidateRect(hWnd, NULL, TRUE);
                MessageBox(hWnd, L"지웠습니다.", L"지우기", NULL);
                break;

            /// 기타
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
