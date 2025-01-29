#pragma once

#include <windows.h>

#include "Player.hpp"
#include "Level.hpp"

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define MOVE_SPEED 5
#define JUMP_HEIGHT 10

class Game {
	public:
		Game(HWND);
		void Update();
		void Render();

	private:
		HWND m_hwnd;
		Player Mahex;
		Level m_level;

		void CheckInput();
};