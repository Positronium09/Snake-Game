#include "TileMap.h"


_tilerow::_tilerow()
{
	if (tiles != NULL)
	{
		memset(tiles, EMPTY_TILE, sizeof(tile) * TILE_COUNTX);
	}
}

tile* _tilerow::GetTile(size_t index)
{
	if (index >= TILE_COUNTX)
	{
		return NULL;
	}
	if (tiles != NULL)
	{
		return &tiles[index];
	}
	return NULL;
}

_tilerow::~_tilerow()
{
	if (tiles != NULL)
	{
		delete[] tiles;
	}
}

BOOL _tilerow::SetTile(size_t index, UINT value)
{
	tile* tile = GetTile(index);
	if (tile != NULL)
	{
		(*tile) = value;
		return TRUE;
	}
	return FALSE;
}

tile* _tilerow::operator[](size_t index)
{
	return GetTile(index);
}
