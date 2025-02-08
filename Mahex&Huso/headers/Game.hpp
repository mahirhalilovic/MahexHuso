#pragma once

#include <windows.h>
#include <fstream>
#include <codecvt>
#include <locale>
#include <set>
#include "json.hpp"

#include "Player.hpp"
#include "Level.hpp"
#include "LevelEditor.hpp"
#include "Display.hpp"
#include "Button.hpp"
#include "Label.hpp"

using json = nlohmann::json;

#define WINDOW_WIDTH 1200
#define WINDOW_WIDTH_EDITOR 1600
#define WINDOW_HEIGHT 720

#define TILE_SIZE 48
#define PLAYER_SIZE 32
#define COIN_SIZE 16
#define PLATE_WIDTH 16
#define PLATE_LENGTH 48

#define FPS_MAX 60

#define MOVE_SPEED 5
#define JUMP_HEIGHT 10

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
			PAUSE,
			GAME_WIN,
			GAME_OVER
		};

		GameState m_state = GameState::MAIN_MENU;
		HWND m_hwnd;
		std::string m_currentWorkingDirectory;

		std::vector<Level> m_levels;
		Level m_loadedLevel;
		int m_currentLevel = 1;
		unsigned int m_coins, m_score;
		std::set<int> m_activePressureBlocks;

		LevelEditor m_levelEditor;
		Display m_display;

		Player Mahex, Huso;

		HBITMAP m_backgroundMainMenu;
		HBITMAP m_backgroundMaskMainMenu;
		HBITMAP m_backgroundPauseMenu;
		HBITMAP m_backgroundMaskPauseMenu;

		bool m_escapeButtonPressed = false;
		float m_pauseMenuY = -300.0f;
		float m_pauseTargetY = 0.0f;
		bool m_animationInProgressPause = false;

		float m_gameWinMenuY = -350.0f;
		float m_gameWinTargetY = 0.0f;
		bool m_animationInProgressGameWin = false;

		float m_gameOverMenuY = -250.0f;
		float m_gameOverTargetY = 0.0f;
		bool m_animationInProgressGameOver = false;

		Label m_label;
		Label labelGameWinScore, labelGameWinHighScore;
		Label labelGameOver;

		bool m_mouseButtonPressed = false;
		Button buttonBack;
		Button buttonMainMenuPlay, buttonMainMenuOptions, buttonMainMenuExit;
		Button buttonPlayMenuGameLevels, buttonPlayMenuCustomLevels;
		Button buttonCustomLevelsPlay, buttonCustomLevelsEdit;
		Button buttonPauseMenuResume, buttonPauseMenuRestart, buttonPauseMenuOptions, buttonPauseMenuQuit;
		Button buttonGameWinNext, buttonGameWinRestart, buttonGameWinQuit;
		Button buttonGameOverRestart, buttonGameOverQuit;

		HANDLE m_fontHandle = nullptr;
		HFONT m_customFont = nullptr;

		bool m_fullscreen = false;
		int m_winWidth = 1920;
		int m_winHeight = 1080;

		void ConstructButtons();
		void CheckHoverStatus(const POINT &mousePos);

		void RenderMainMenu();
		void RenderPlayMenu();
		void RenderGameLevels();
		void RenderCustomLevels();
		void RenderLevelEditor();
		void RenderOptions();
		void RenderInGame();
		void RenderPause();
		void RenderGameWin();
		void RenderGameOver();
		void RenderTiles(const HDC&, const HDC&);

		void CheckPressureBlocks();

		void UpdatePlayer(Player&);
		void CheckInput();
		void ProcessMouseClick(POINT);

		void CheckWinningCondition();

		void LoadLevel(int);
		bool LoadLevelFromJSON(std::string, int);
		Tile LoadTile(const json&);

		void UpdateWindowSize(bool isEditor);

		//bool LoadCustomFont();
		//void CleanupFont();

		bool ToggleFullScreen();
		bool ToggleFullScreen(bool);
};