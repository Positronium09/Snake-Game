#include <queue>

#include "utils.h"
#include "colors.h"


extern BOOL over;
extern BOOL start;
extern BOOL paused;
extern ULONGLONG score;
extern ULONGLONG highscore;

extern GameManager manager;
extern que movement;
extern int timerID;

extern HBRUSH snakeBrush;
extern HBRUSH foodBrush;
extern HBRUSH backgroundBrush;


RECT CalculatePlayFieldSize(HWND hWnd, DWORD style)
{
	RECT rc = { 0, 0, PLAYFIELDX, PLAYFIELDY };

	AdjustWindowRect(&rc, style & ~WS_OVERLAPPED, FALSE);

	return rc;
}

void PrintScore(HWND hWnd)
{
	TCHAR text[49];
	prints(text, TEXT("Snake Game | Current Score %I64u / High Score %I64u"), score, highscore);
	SetWindowText(hWnd, text);
}

void GameLogic(HWND hWnd)
{
	UINT _return = CONTINUE;

	if (movement.size() == 0)
	{
		_return = manager.MovePlayer();
	}
	else
	{
		_return = manager.MovePlayer(movement.front());
		movement.pop();
	}

	if (_return == GAME_OVER || _return == CRASH)
	{
		HDC hdc = GetDC(hWnd);

		RECT rc = { 0 };
		RECT dummy = { 0 };
		GetClientRect(hWnd, &rc);
		SetRect(&rc, 0, 0, RECTWIDTH(rc), RECTHEIGHT(rc));

		SetTextColor(hdc, PINK);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, TEXT("Game Over!"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

		DrawText(hdc, TEXT("Press enter to restart"), -1, &dummy, DT_CENTER | DT_CALCRECT | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
		rc.top += RECTHEIGHT(dummy) * 3;
		DrawText(hdc, TEXT("Press enter to restart"), -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);

		ReleaseDC(hWnd, hdc);

		over = TRUE;
		score = 0;
		PrintScore(hWnd);
		KillTimer(hWnd, timerID);
		return;
	}

	if (_return == FOOD)
	{
		manager.AddSnake();
		manager.GenerateFood();
		score++;
		if (score > highscore)
		{
			highscore = score;
		}
		PrintScore(hWnd);
	}

	InvalidateRect(hWnd, NULL, FALSE);
}


void Restart(HWND hWnd)
{
	manager.Restart();
	SetTimer(hWnd, timerID, UPDATE_TIME, NULL);
	InvalidateRect(hWnd, NULL, TRUE);
	score = 0;
}

void SetRectToPoint(RECT* rc, pos p)
{
	rc->left = p.x * 10;
	rc->top = p.y * 10;

	rc->right = rc->left + 10;
	rc->bottom = rc->top + 10;
}

void OnPaint(HWND hWnd, HDC hdc)
{
	if (start)
	{
		RECT rc;

		GetClientRect(hWnd, &rc);
		SetRect(&rc, 0, 0, RECTWIDTH(rc), RECTHEIGHT(rc));

		SetTextColor(hdc, PINK);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, TEXT("Press enter to start"), -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
		return;
	}
	if (over)
	{
		return;
	}

	RECT rc = { 0 };
	HBRUSH brush = NULL;

	pos erase = manager.GetErasePos();
	SetRectToPoint(&rc, erase);
	FillRect(hdc, &rc, backgroundBrush);

	pos food = manager.GetFoodPos();
	SetRectToPoint(&rc, food);
	FillRect(hdc, &rc, foodBrush);

	for (LONGLONG i = 0; i < manager.GetSnakeSize(); i++)
	{
		pos p = *manager.GetSnakePart(i);

		SetRectToPoint(&rc, p);

		FillRect(hdc, &rc, snakeBrush);
	}
	if (paused)
	{
		RECT rc;

		GetClientRect(hWnd, &rc);
		SetRect(&rc, 0, 0, RECTWIDTH(rc), RECTHEIGHT(rc));

		SetTextColor(hdc, PINK);
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, TEXT("Game Paused"), -1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
	}
}
