// MyExplorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include <windowsx.h>

#include <winuser.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
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
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	/*for (int i = 0; i < drives.Size(); ++i)
	{
		wchar_t wstr[1001];
		wcscpy(wstr, drives[i]->LABEL());
		wcscat(wstr, L"\n");
		wcscat(wstr, drives[i]->NAME());
		wcscat(wstr, L"\nDung lượng trống: ");
		wcscat(wstr, CText::NumberToWCS(drives[i]->FreeSpace().SIZE(), 2));
		wcscat(wstr, drives[i]->FreeSpace().TYPE());
		wcscat(wstr, L"\nDung lượng tổng: ");
		wcscat(wstr, CText::NumberToWCS(drives[i]->TotalSpace().SIZE(), 2));
		wcscat(wstr, drives[i]->TotalSpace().TYPE());

		MessageBox(0, wstr, 0, 0);
	}*/

	/*std::list<CFile> list = CExplorer::GetListDirectoryAndFile(L"C:\\");

	for (CFile file : list)
	{
		wchar_t wstr[1001];
		wcscpy(wstr, file.PATH());
		wcscat(wstr, L"\n");

		wcscat(wstr, file.NAME());
		wcscat(wstr, L"\n");

		wcscat(wstr, file.DATEMODIFIED());
		wcscat(wstr, L"\n");

		wcscat(wstr, file.TYPE());
		wcscat(wstr, L"\n");

		wcscat(wstr, CText::NumberToWCS(file.CAPACITY().ROUNDSIZE(), 2));
		wcscat(wstr, L" ");
		wcscat(wstr, file.CAPACITY().TYPE());

		MessageBox(0, wstr, 0, 0);
	}*/

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MYEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYEXPLORER));

	MSG msg;

	// Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYEXPLORER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYEXPLORER);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 1000, 700, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
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

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd)
{
	delete drives;
	PostQuitMessage(0);
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	int wmId = id;
	// Parse the menu selections:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	//Get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	RECT main;
	if (GetWindowRect(hWnd, &main))
	{
		int width = main.right - main.left;
		int height = main.bottom - main.top;

		listView.Init(drives, width - 315, height - 58, 300, 0, hWnd, hInst);
		treeView.Init(drives, 300, height - 58, 0, 0, hWnd, hInst);
	}



	return TRUE;
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	HTREEITEM hTreeItem;
	__int32 nCurSelIndex;
	LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)pnm;

	switch (pnm->code)
	{
	case TVN_ITEMEXPANDING:
		treeView.InitChild(lpnmTree->itemOld.hItem, lpnmTree->itemNew.hItem);
		break; //case TVN_ITEMEXPANDING
	case TVN_SELCHANGED:
		listView.LoadChild(treeView.GetPath(lpnmTree->itemNew.hItem));
		hTreeItem = TreeView_GetNextItem(treeView.GetHandle(), NULL, TVGN_CARET);
		TreeView_Expand(treeView.GetHandle(), lpnmTree->itemNew.hItem, TVE_EXPAND);
		break; //case TVN_SELCHANGED
	case NM_DBLCLK:
		if (pnm->hwndFrom == listView.GetHandle())
		{
			listView.LoadChild();
		}
		break; //case NM_DBLCLK:
	}

	return 0;
}
