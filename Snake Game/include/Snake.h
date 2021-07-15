#pragma once

#include <windows.h>
#include "globals.h"

#define SNAKE_SIZE (MAX_SIZE)


typedef struct _pos
{
	LONGLONG x = -1;
	LONGLONG y = -1;

	_pos() {  }
	_pos(LONGLONG& _x, LONGLONG& _y) { x = _x; y = _y; }
	_pos(LONGLONG _x, LONGLONG _y) { x = _x; y = _y; }
	operator POINT() { POINT _p = { x, y }; return _p; }
	_pos& operator+=(const _pos& other) { x += other.x; y += other.y; return *this; }
	_pos operator+(const _pos& other) { _pos _p; _p.x = x + other.x; _p.y = y + other.y; return _p; }
	_pos& operator-=(const _pos& other) { x -= other.x; y -= other.y; return *this; }
	_pos operator-(const _pos& other) { _pos _p; _p.x = x - other.x; _p.y = y - other.y; return _p; }
} pos;
class Snake
{
	private:
	pos parts[SNAKE_SIZE];
	UINT dir = DIR_RIGHT;
	LONGLONG size = 3;

	public:
	
	Snake();
	void Init();
	pos* GetPart(size_t index);
	pos* GetLast();
	pos* GetFirst();
	void AddPart(pos position);
	UINT GetMoveDir();
	void SetMoveDir(UINT dir);
	LONGLONG GetSize();
};
