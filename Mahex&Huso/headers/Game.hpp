#pragma once

#include <windows.h>

#include "Player.hpp"
#include "Level.hpp"
#include "Display.hpp"
#include "Button.hpp"

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

#define TILE_SIZE 48
#define PLAYER_SIZE 32

#define FPS_MAX 60

#define MOVE_SPEED 5
#define JUMP_HEIGHT 8

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
			IN_GAME,
			PAUSE
		};

		GameState m_state = GameState::MAIN_MENU;
		HWND m_hwnd;
		Player Mahex;
		//Player Huso;
		//std::vector<Level> m_levels;
		Level m_currentLevel;
		Display m_display;

		HBITMAP m_backgroundMainMenu;
		HBITMAP m_backgroundMaskMainMenu;

		bool m_escapeButtonPressed = false;
		float m_pauseMenuY = -200.0f;
		float m_pauseTargetY = 0.0f;
		bool m_animationInProgress = false;

		bool m_mouseButtonPressed = false;
		Button buttonMainMenuPlay, buttonMainMenuOptions, buttonMainMenuExit;
		Button buttonPlayMenuGameLevels, buttonPlayMenuCustomLevels, buttonPlayMenuBack;

		HANDLE m_fontHandle = nullptr;
		HFONT m_customFont = nullptr;

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
		void RenderPause();

		void UpdatePlayer(Player&);
		void CheckInput();
		void ProcessMouseClick(POINT);

		void LoadLevel();

		bool LoadCustomFont();
		void CleanupFont();

		bool ToggleFullScreen();
		bool ToggleFullScreen(bool);
};