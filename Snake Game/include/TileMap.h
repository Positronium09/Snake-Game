#pragma once

#include <windows.h>
#include "globals.h"


typedef BYTE tile;

typedef struct _tilerow
{
	private:
	tile tiles[TILE_COUNTX];

	public:
	_tilerow();
	~_tilerow();

	tile* GetTile(size_t index);
	BOOL SetTile(size_t index, UINT value);
	tile* operator[](size_t index);
} tileRow;

class TileMap
{
	private:
	tileRow rows[MAX_SIZE];

	public:
	TileMap();
	void Init();

	tileRow* GetTileRow(size_t index);
	tile* GetTile(size_t x, size_t y);
	tile* GetTile(POINT p);


	BOOL SetTile(size_t x, size_t y, UINT value);
	BOOL SetTile(POINT p, UINT value);

	tileRow* operator[](size_t index);
};
