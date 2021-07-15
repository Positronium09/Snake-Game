#pragma once

#include <windows.h>

#define DIR_LEFT VK_LEFT
#define DIR_UP VK_UP
#define DIR_RIGHT VK_RIGHT
#define DIR_DOWN VK_DOWN

#define UPDATE_TIME 50

#define TILE_CX 10
#define TILE_CY 10

#define INVALID_TILE 0x00
#define EMPTY_TILE 0x01
#define SNAKE 0x02
#define FOOD 0x04

#define CONTINUE 0x08
#define GAME_OVER 0x10
#define VICTORY 0x20
#define CRASH 0x40


#define MAX_SIZE (TILE_COUNTX * TILE_COUNTY)

#define PLAYFIELDX 800
#define PLAYFIELDY 600
#define TILE_COUNTX (PLAYFIELDX / 10)
#define TILE_COUNTY (PLAYFIELDY / 10)
