#pragma once

#include <windows.h>
#include "TileMap.h"
#include "GameManager.h"

#define RECTHEIGHT(rc) (rc.bottom - rc.top)
#define RECTWIDTH(rc) (rc.right - rc.left)

#ifdef UNICODE
#define prints wsprintf
#else
#define prints sprintf
#endif

using std::queue;
typedef queue<char> que;

RECT CalculatePlayFieldSize(HWND hWnd, DWORD style);
void GameLogic(HWND hWnd);
void Restart(HWND hWnd);
void OnPaint(HWND hWnd, HDC hdc);
