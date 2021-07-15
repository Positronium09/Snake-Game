#include <cstdlib>
#include <intrin.h>

#include "GameManager.h"

#pragma intrinsic(__rdtsc)


GameManager::GameManager()
{
	Init();
}

void GameManager::Init()
{
	map = new TileMap;
	player = new Snake;
}

UINT GameManager::Start()
{
	size_t middleX = TILE_COUNTX / 2;
	size_t middleY = TILE_COUNTY / 2;

	pos* firstPart = player->GetPart(0);
	pos* secondPart = player->GetPart(1);
	pos* thirdPart = player->GetPart(2);

	firstPart->x = middleX;
	firstPart->y = middleY;
	secondPart->x = middleX - 1;
	secondPart->y = middleY;
	thirdPart->x = middleX - 2;
	thirdPart->y = middleY;

	SetTile(firstPart);
	SetTile(secondPart);
	SetTile(thirdPart);

	GenerateFood();

	return 0;
}

UINT GameManager::Restart()
{
	player->Init();
	map->Init();
	Start();
	lastDir = DIR_RIGHT;
	return 0;
}

UINT GameManager::MovePlayer(char key)
{
	UINT _return = CONTINUE;

	INT moveDir = player->GetMoveDir() - key;

	if (moveDir != 2 && moveDir != -2)
	{
		player->SetMoveDir(key);
	}

	pos dir(0, 0);
	switch (player->GetMoveDir())
	{
		case DIR_UP:
		{
			dir.y--;
			break;
		}
		case DIR_LEFT:
		{
			dir.x--;
			break;
		}
		case DIR_RIGHT:
		{
			dir.x++;
			break;
		}
		case DIR_DOWN:
		{
			dir.y++;
			break;
		}
	}

	pos next = *player->GetFirst() + dir;

	tile* t = map->GetTile(next.x, next.y);
	if (t == NULL)
	{
		return CRASH;
	}

	if (0 > next.x || TILE_COUNTX < next.x || 0 > next.y || TILE_COUNTY < next.y || *t == SNAKE)
	{
		return GAME_OVER;
	}

	if (*t == FOOD)
	{
		_return = FOOD;
	}

	erasePos = *player->GetLast();
	map->SetTile(erasePos, EMPTY_TILE);

	for (LONGLONG i = player->GetSize() - 2; i >= 0; --i)
	{
		*player->GetPart(i + 1) = *player->GetPart(i);
		SetTile(player->GetPart(i + 1));
	}

	pos* lastPtr = player->GetLast();
	pos* lastPtr_1 = lastPtr - 1;
	lastDir = PosToDir(*lastPtr_1 - *lastPtr);

	*player->GetFirst() = next;
	SetTile(player->GetFirst());

	return _return;
}

UINT GameManager::MovePlayer()
{
	return MovePlayer(player->GetMoveDir());
}

void GameManager::SetTile(pos* _p, UINT value)
{
	map->SetTile((POINT)(*_p), value);
}

void GameManager::SetTile(pos* _p)
{
	map->SetTile((POINT)(*_p), SNAKE);
}

tile* GameManager::GetTile(POINT p)
{
	return map->GetTile(p);
}

tile* GameManager::GetTile(size_t x, size_t y)
{
	return map->GetTile(x, y);
}

void GameManager::AddSnake()
{
	pos last = *player->GetLast();
	pos dir = DirToPos(lastDir);
	pos addPos;
	addPos.x = last.x - dir.x;
	addPos.y = last.y + dir.y;
	bool found = false;

	tile* t = map->GetTile(addPos.x, addPos.y);

	if (t == NULL)
	{

	}
	else if (*t != EMPTY_TILE)
	{

	}
	else
	{
		found = true;
	}

	if (!found)
	{
		for (int i = DIR_LEFT; i < DIR_DOWN + 1; i++)
		{
			dir = DirToPos(i);
			addPos = last - dir;
			tile* t = map->GetTile(addPos.x, addPos.y);

			if (t == NULL)
			{

			}
			else if (*t != EMPTY_TILE)
			{

			}
			else
			{
				break;
			}
		}
	}

	player->AddPart(addPos);
	map->SetTile(addPos, SNAKE);
}

pos* GameManager::GetSnakePart(size_t index)
{
	return player->GetPart(index);
}

size_t GameManager::GetSnakeSize()
{
	return player->GetSize();
}

pos GameManager::GetFoodPos()
{
	return foodPos;
}

pos GameManager::GetErasePos()
{
	return erasePos;
}

void GameManager::GenerateFood()
{
	unsigned __int64 cycles;
	cycles = __rdtsc();
	srand((int)cycles);
	size_t x = rand() % TILE_COUNTX;
	size_t y = rand() % TILE_COUNTY;

	if (*(map->GetTile(x, y)) != EMPTY_TILE)
	{
		GenerateFood();
	}

	map->SetTile(x, y, FOOD);
	foodPos.x = x;
	foodPos.y = y;
}

UINT GameManager::PosToDir(pos _p)
{
	UINT dir = NULL;

	if (_p.x == 1)
	{
		dir |= VK_RIGHT;
	}
	if (_p.x == -1)
	{
		dir |= VK_LEFT;
	}
	if (_p.y == 1)
	{
		dir |= VK_UP;
	}
	if (_p.y == -1)
	{
		dir |= VK_DOWN;
	}

	return dir;
}

pos GameManager::DirToPos(UINT _d)
{
	pos p(0, 0);
	switch (_d)
	{
		case DIR_UP:
		{
			p.y--;
			break;
		}
		case DIR_LEFT:
		{
			p.x--;
			break;
		}
		case DIR_RIGHT:
		{
			p.x++;
			break;
		}
		case DIR_DOWN:
		{
			p.y++;
			break;
		}
	}
	return p;
}
