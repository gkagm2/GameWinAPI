// WinAPI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "framework.h"
#include "WinAPI.h"
#include "CCore.h"
#include <vector>
using std::vector;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND g_hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CALLBACK       TimerProc(HWND, UINT, UINT, DWORD);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));
    HANDLE hSetTimer = (HANDLE)SetTimer(g_hWnd, 1, 100, (TIMERPROC)TimerProc);
    HANDLE hSetTimer2 = (HANDLE)SetTimer(g_hWnd, 2, 100, (TIMERPROC)TimerProc);


    MSG msg{};
    CCore::GetInstance()->Init(g_hWnd, POINT{ 1280, 768 });

    // 1초에 1000씩 셈.
    int iPrevCount = GetTickCount64();// 현재 카운트
    int iCallCount = 0;


    // Main message loop:
    while (true)
    {
        // 메세지를 슬쩍 보겠다는것임.
        // 메세지가 있든 없든 상관없음.

        // 픽 메세지의 특징이 보기만 하기때문에 메세지가 있으면 지워야 한다. 때문에 PM_REMOVE를 이용한다.
        // 메세지가 있으면 큐에서 없애고 가지고 와야 함. 
        // 메세지가 없으면 없는대로 종료 됨.
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { // PM_REMOVE로 메세지를 제거해야 한다.
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else { // 메세지가 발생하지 않으면
            int iCurCount = GetTickCount64();
            // 1초가 지난 시점.
            if (iCurCount - iPrevCount > 1000) { // 1초에 한번씩 안에 들어옴.
                wchar_t szBuffer[255] = {};
                swprintf_s(szBuffer, 255, L"Progress call Count : %d", iCallCount);
                SetWindowText(g_hWnd, szBuffer);

                iPrevCount = iCurCount;
                iCallCount = 0;
            }
            ++iCallCount;

            CCore::GetInstance()->Progress();
        }
    }

    KillTimer(g_hWnd, 1);
    KillTimer(g_hWnd, 2);

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc,
        wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = 0; //MAKEINTRESOURCEW(IDC_WINAPI);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    wsprintf(szTitle, L"Sagacity Jang World");
    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

float g_pos[2];
int offX = 300;
int offY = 300;
bool isDraw = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 마우스 좌표는 lParam에 들어옴. lParam에 2byte씩 넣은것임.
    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
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
        // TODO: Add any drawing code that uses hdc here...

        // 도형
        //{
        //    int n = 2;
        //    int d = 71;
        //    float r = 130;
        //    float x, y;
        //    x = cos(0) * r + offX;
        //    y = sin(0) * r + offY;
        //    for (int i = 0; i < 360; ++i) {
        //        int k = i * d;
        //        r = cos(n * k)* 120;
        //        MoveToEx(hdc, x, y, nullptr);
        //        x = cos(i) * r + offX;
        //        y = sin(i) * r + offY;
        //    
        //        LineTo(hdc, x, y);
        //    }
        //    //++r;
        //}

        // 도넛 그리기 
        /*{
            int r = 300;
            int x = cos(0) * r + offX;
            int y = sin(0) * r + offY;
            for (float  i = 0; i <= 900; i += 1.5f) {
                MoveToEx(hdc, x, y, nullptr);
                int xx = cos(i)* r + offX;
                int yy = sin(i) * r + offY;
                LineTo(hdc, xx, yy);
                x = xx;
                y = yy;
            }
        }*/
        // 원 그리기
        /*{
            int r = 150;
            float x = cos(0) * r + offX;
            float y = sin(0) * r + offY;
            for (float i = 0; i <= 3.14f * 2; i += 0.005f) {
                MoveToEx(hdc, x, y, nullptr);
                x = cos(i) * r + offX;
                y = sin(i) * r + offY;
                LineTo(hdc, x, y);
            }
        }*/

        // 타원 그리기
        /*{
            float a, b;
            a = 1.0f;
            b = 2.0f;
            float r = 300;
            float x = (cos(0)) / (a * a) * r + offX;
            float y = (sin(0)) / (b * b) * r + offY;

            for (float i = 0; i <= 3.14f * 2; i += 0.01f) {
                MoveToEx(hdc, x, y, nullptr);
                x = (cos(i)) / (a * a) * r + offX;
                y = (sin(i)) / (b * b) * r + offY;
                LineTo(hdc, x, y);
            }
        }*/

        // 중심점에서부터 원으로 레이캐스팅해보기
        /*{
            float x, y;
            x = y = 0;
            float r = 300;
            float rayDirX;
            float rayDirY;
            x += offX;
            y += offY;
            for (float i = 0; i <= 3.14 * 2; i += 0.01f) {
                MoveToEx(hdc, x, y, nullptr);
                rayDirX = cos(i) * r + x;
                rayDirY = sin(i) * r + y;
                LineTo(hdc, rayDirX, rayDirY);
            }
        }*/
        // 원의 등고선 표현해보기
        /*{
            for (int r = 30; r <= 250; r += 15) {
                int x = cos(0) * r + offX;
                int y = sin(0) * r + offY;
                for (float i = 0; i <= 3.14f * 2; i += 0.005f) {
                    MoveToEx(hdc, x, y, nullptr);
                    x = cos(i) * r + offX;
                    y = sin(i) * r + offY;
                    LineTo(hdc, x, y);
                }
            }
        }*/
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEMOVE:
        if (isDraw) {
            HDC hdc = GetDC(g_hWnd);
            MoveToEx(hdc, g_pos[0], g_pos[1], nullptr);
            g_pos[0] = LOWORD(lParam);
            g_pos[1] = HIWORD(lParam);
            LineTo(hdc, g_pos[0], g_pos[1]);
        }
        break;
    case WM_LBUTTONDOWN:
        isDraw = true;
        g_pos[0] = LOWORD(lParam);
        g_pos[1] = HIWORD(lParam);
        /*MB_ICONHAND
        MB_ICONQUESTION
        MB_ICONEXCLAMATION
        MB_ICONASTERISK*/
        break;
    case WM_LBUTTONUP:
        isDraw = false;

        break;
    case WM_KEYDOWN:
        break;
        break;
        break;
    case WM_DESTROY:
        KillTimer(g_hWnd, 1);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
    HDC hdc;
    int i;
    hdc = GetDC(hWnd);
    RECT clientRect{};
    GetClientRect(hWnd, &clientRect);
    GetWindowRect(hWnd, &clientRect);
    /*for (i = 0; i < 100; ++i) {
        SetPixel(hdc, rand() % 1248, rand() % 758, RGB(rand() % 256, rand() % 256, rand() % 256));
    }*/

    int l, k;
    l = 0; k = 0;
    int r, g, b;
    r = g = b = 0;
    for (r = 0; r <= 255; ++r) {
        for (g = 0; g <= 255; ++g) {
            for (b = 0; b <= 255; ++b) {
                SetPixel(hdc, k, l, RGB(r, g, b));
                if (k >= 1280)
                    k = 0;
                if (l >= 758)
                    l = 0;
                ++k;
                ++l;
            }
        }
    }
    ReleaseDC(hWnd, hdc);
}

// Message handler for about box.
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
