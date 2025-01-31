#pragma once

#include <windows.h>

#include "Player.hpp"
#include "Level.hpp"
#include "Display.hpp"

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define MOVE_SPEED 5
#define JUMP_HEIGHT 12

class Game {
	public:
		Game(HWND);
		void Update();
		void Render();

	private:
		HWND m_hwnd;
		Player Mahex;
		//std::vector<Level> m_levels;
		//Level m_currentLevel;
		std::vector<Tile> m_level;
		Display m_display;

		bool m_fullscreen = false;
		int m_winWidth = 1920;
		int m_winHeight = 1080;

		void UpdatePlayer(Player&);
		void CheckInput();
		bool ToggleFullScreen();
		bool ToggleFullScreen(bool);
};