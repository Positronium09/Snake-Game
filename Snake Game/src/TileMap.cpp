#include "TileMap.h"


TileMap::TileMap()
{
	Init();
}

void TileMap::Init()
{
	//rows = new tileRow[TILE_COUNTY];
	if (rows != NULL)
	memset(rows, EMPTY_TILE, TILE_COUNTX * TILE_COUNTY);
}

tileRow* TileMap::GetTileRow(size_t index)
{
	if (rows == NULL)
	{
		return NULL;
	}
	if (index >= TILE_COUNTY)
	{
		return NULL;
	}
	return &rows[index];
}

tile* TileMap::GetTile(size_t x, size_t y)
{
	tileRow* row = GetTileRow(y);
	if (row == NULL)
	{
		return NULL;
	}
	return row->GetTile(x);
}

tile* TileMap::GetTile(POINT p)
{
	return GetTile(p.x, p.y);
}

BOOL TileMap::SetTile(POINT p, UINT value)
{
	tile* tile = GetTile(p);
	if (tile != NULL)
	{
		(*tile) = value;
		return TRUE;
	}
	return FALSE;
}

BOOL TileMap::SetTile(size_t x, size_t y, UINT value)
{
	tile* tile = GetTile(x, y);
	if (tile != NULL)
	{
		(*tile) = value;
		return TRUE;
	}
	return FALSE;
}

tileRow* TileMap::operator[](size_t index)
{
	return GetTileRow(index);
}
