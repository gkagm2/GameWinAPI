#include "stdafx.h"
#include "framework.h"
#include "WinAPI.h"
#include "CCore.h"

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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc(); // prompt창에 메모리 누수 발생 시 라인이 뜨는데 그 라인의 숫자를 파라미터에 넣으면 메모리 누수 발생지점에서 중단점이 걸린다.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    // TODO : Delete this
    srand(static_cast<int>(time(nullptr))); // TEST CODE

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));

    MSG msg{};

    
    int x = (UINT)GetSystemMetrics(SM_CXSCREEN);
    int y = (UINT)GetSystemMetrics(SM_CYSCREEN);
    Vector2 vResolution = ResizeScaleMaintainRatio(x, y, 1280, 768);

    CCore::GetInstance()->Init(g_hWnd, POINT{ LONG(vResolution.x), LONG(vResolution.y) });
    //CCore::GetInstance()->Init(g_hWnd, POINT{ 1280, 768 });

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
            CCore::GetInstance()->Progress();
        }
    }

    return (int)msg.wParam;
}

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
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = 0; //MAKEINTRESOURCEW(IDC_WINAPI);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

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

    ShowWindow(g_hWnd, nCmdShow); // nCmdShow = true
    UpdateWindow(g_hWnd);

    return TRUE;
}

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
            DialogBox(nullptr, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case ID_TILE_CREATE:
        case ID_TILE_SAVE:
        case ID_TILE_LOAD:
            MenuContainer(wmId);
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
   
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_KEYDOWN:
        break;
    break;
        break;
    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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
