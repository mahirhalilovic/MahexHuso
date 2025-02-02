#pragma once

#include <windows.h>

#include "Player.hpp"
#include "Level.hpp"
#include "Display.hpp"
#include "Button.hpp"

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

#define FPS_MAX 60
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
		enum class GameState {
			MAIN_MENU,
			PLAY_MENU,
			GAME_LEVELS,
			CUSTOM_LEVELS,
			LEVEL_EDITOR,
			OPTIONS,
			IN_GAME
		};

		GameState m_state = GameState::MAIN_MENU;
		HWND m_hwnd;
		Player Mahex;
		//Player Huso;
		//std::vector<Level> m_levels;
		//Level m_currentLevel;
		std::vector<Tile> m_level;
		Display m_display;

		Button buttonMainMenuPlay, buttonMainMenuOptions, buttonMainMenuExit;
		Button buttonPlayMenuGameLevels, buttonPlayMenuCustomLevels, buttonPlayMenuBack;

		bool m_fullscreen = false;
		int m_winWidth = 1920;
		int m_winHeight = 1080;

		void RenderMainMenu();
		void RenderPlayMenu();
		void RenderGameLevels();
		void RenderCustomLevels();
		void RenderLevelEditor();
		void RenderOptions();
		void RenderInGame();

		void UpdatePlayer(Player&);
		void CheckInput();
		void ProcessMouseClick(POINT);
		bool ToggleFullScreen();
		bool ToggleFullScreen(bool);
};