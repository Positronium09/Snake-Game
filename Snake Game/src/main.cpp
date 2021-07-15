#include <queue>
#include <cstdlib>
#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>

#include "colors.h"
#include "GameManager.h"
#include "utils.h"
#include "globals.h"
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX))


HINSTANCE hInstance = 0;
HWND hMainWindow = 0;

BOOL over = TRUE;
ULONGLONG score = 0;
ULONGLONG highscore = 0;

GameManager manager;
que movement;
int timerID = 1;

BOOL start = TRUE;
BOOL paused = FALSE;

TCHAR mainWindowClassName[] = TEXT("mainWindowClass");

HBRUSH backgroundBrush = CreateSolidBrush(backgroundColor);
HBRUSH foodBrush = CreateSolidBrush(RED);
HBRUSH snakeBrush = CreateSolidBrush(CYAN);

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void OnCreate(HWND hWnd);
void OnDestroy();


int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg = { 0 };
	HACCEL hAccel = 0;

	hInstance = _hInstance;

	WNDCLASSEX mainWindowClass;
	SecureZeroMemory(&mainWindowClass, sizeof(WNDCLASSEX));

	mainWindowClass.cbSize = sizeof(WNDCLASSEX);
	mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	mainWindowClass.hbrBackground = backgroundBrush;
	mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	mainWindowClass.hInstance = hInstance;
	mainWindowClass.lpszClassName = mainWindowClassName;
	mainWindowClass.lpfnWndProc = MainWindowProc;
	mainWindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(APP_ICON));

	if (!RegisterClassEx(&mainWindowClass))
	{
		TCHAR text[25];
		prints(text, L"%d", GetLastError());
		MessageBox(GetDesktopWindow(), TEXT("RegisterClassEx Failed"), text, MB_OK);
		return GetLastError();
	}

	hMainWindow = CreateWindowEx(NULL, mainWindowClassName, TEXT("Snake Game | Current Score 0 / High Score 0"), WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);

	if (hMainWindow == NULL)
	{
		TCHAR text[25];
		prints(text, L"%d", GetLastError());
		MessageBox(GetDesktopWindow(), TEXT("RegisterClassEx Failed"), text, MB_OK);
		return GetLastError();
	}
	
	ShowWindow(hMainWindow, nShowCmd);
	UpdateWindow(hMainWindow);
	
	#pragma warning (disable : 6387)
	hAccel = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(NULL));
	#pragma warning (default : 6387)

	while (GetMessageW(&msg, NULL, NULL, NULL))
	{
		if (!TranslateAcceleratorW(hMainWindow, hAccel, &msg))
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static char lastKey = DIR_RIGHT;

	switch (msg)
	{
		case WM_CREATE:
		{
			OnCreate(hWnd);
			return 0;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			HDC bufDc = CreateCompatibleDC(hdc);

			BITMAP bitmapInfo;
			memset(&bitmapInfo, 0, sizeof(BITMAP));

			HBITMAP hBitmap = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
			GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);

			HBITMAP bufBmp = CreateCompatibleBitmap(hdc, bitmapInfo.bmWidth, bitmapInfo.bmHeight);
			memset(&bitmapInfo, 0, sizeof(BITMAP));
			GetObject(bufBmp, sizeof(BITMAP), &bitmapInfo);
			HBITMAP old = (HBITMAP)SelectObject(bufDc, bufBmp);

			
			OnPaint(hWnd, bufDc);


			BitBlt(hdc, 0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight, bufDc, 0, 0, SRCCOPY);

			SelectObject(bufDc, old);

			DeleteObject(bufBmp);
			DeleteDC(bufDc);

			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_TIMER:
		{
			if (wParam != timerID)
			{
				break;
			}

			GameLogic(hWnd);

			break;
		}

		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
				{
					if (!paused && !over)
					{
						KillTimer(hWnd, timerID);
						InvalidateRect(hWnd, NULL, FALSE);
						paused = TRUE;
					}
					else if (paused && !over)
					{
						RECT rc;
						SetRect(&rc, 340, 290, 450, 310);
						InvalidateRect(hWnd, &rc, TRUE);
						SetTimer(hWnd, timerID, UPDATE_TIME, NULL);
						paused = FALSE;
					}
					break;
				}
				case VK_RETURN:
				{
					if (over || start)
					{
						Restart(hWnd);
						over = FALSE;
						start = FALSE;
						lastKey = DIR_RIGHT;
					}
					break;
				}
				case DIR_UP:
				{
					if (!over && !start && lastKey != wParam)
					{
						movement.push(wParam);
						lastKey = wParam;
					}
					break;
				}
				case DIR_LEFT:
				{
					if (!over && !start && lastKey != wParam)
					{
						movement.push(wParam);
						lastKey = wParam;
					}
					break;
				}
				case DIR_RIGHT:
				{
					if (!over && !start && lastKey != wParam)
					{
						movement.push(wParam);
						lastKey = wParam;
					}
					break;
				}
				case DIR_DOWN:
				{
					if (!over && !start && lastKey != wParam)
					{
						movement.push(wParam);
						lastKey = wParam;
					}
					break;
				}
				default:
				{
					return 1;
				}
			}
			return 0;
		}

		case WM_DESTROY:
		{
			OnDestroy();
			PostQuitMessage(0);
			break;
		}

		case WM_QUIT:
		{
			DestroyWindow(hWnd);
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}

void OnCreate(HWND hWnd)
{
	RECT rc = CalculatePlayFieldSize(hWnd, WINDOW_STYLE);

	SetWindowPos(hWnd, NULL, 0, 0, RECTWIDTH(rc), RECTHEIGHT(rc), SWP_NOMOVE);

	manager.Init();
	manager.Start();
	
	InvalidateRect(hWnd, NULL, TRUE);
}

void OnDestroy()
{
	DeleteObject(backgroundBrush);
	DeleteObject(foodBrush);
	DeleteObject(snakeBrush);
}
