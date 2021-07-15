#include "Snake.h"


#pragma warning (disable : 26451)

Snake::Snake()
{
	Init();
}

void Snake::Init()
{
	if (parts != NULL)
	{
		memset(parts, -1, SNAKE_SIZE * sizeof(pos));
	}
	dir = DIR_RIGHT;
	size = 3;
}

pos* Snake::GetPart(size_t index)
{
	size_t count = SNAKE_SIZE;
	if (index < count)
	{
		return &parts[index];
	}
	return NULL;
}

pos* Snake::GetLast()
{
	return GetPart(GetSize() - 1);
}

pos* Snake::GetFirst()
{
	return GetPart(0);
}

void Snake::AddPart(pos _p)
{
	if (size == SNAKE_SIZE)
	{
		return;
	}
	*GetPart(size) = _p;
	size++;
}

void Snake::SetMoveDir(UINT _dir)
{
	dir = _dir;
}

UINT Snake::GetMoveDir()
{
	return dir;
}

LONGLONG Snake::GetSize()
{
	return size;
}

#pragma warning (default : 26451)
