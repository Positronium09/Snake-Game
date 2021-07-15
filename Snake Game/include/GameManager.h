#pragma once

#include "TileMap.h"
#include "Snake.h"
#include "globals.h"


class GameManager
{
	private:
	TileMap* map = NULL;
	Snake* player = NULL;
	UINT lastDir = DIR_RIGHT;

	pos foodPos;
	pos erasePos;

	public:
	GameManager();

	void Init();
	UINT Start();
	UINT Restart();
	UINT MovePlayer(char key);
	UINT MovePlayer();
	void SetTile(pos* position);
	void SetTile(pos* position, UINT value);
	tile* GetTile(POINT p);
	tile* GetTile(size_t x, size_t y);

	pos* GetSnakePart(size_t index);
	size_t GetSnakeSize();
	pos GetFoodPos();
	pos GetErasePos();

	void AddSnake();
	pos DirToPos(UINT dir);
	UINT PosToDir(pos _p);

	void GenerateFood();
};
