#include "headers/Game.hpp"
#pragma comment(lib, "Msimg32.lib")

Game::Game(HWND hwnd) : m_hwnd{hwnd}, m_display{Display(hwnd)} {
    m_backgroundMainMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_mainmenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_backgroundMaskMainMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_mask_mainmenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    buttonMainMenuPlay = Button(500, 235, 200, 50, L"PLAY");
    buttonMainMenuOptions = Button(500, 335, 200, 50, L"OPTIONS");
    buttonMainMenuExit = Button(500, 435, 200, 50, L"EXIT");
    buttonPlayMenuGameLevels = Button(500, 235, 200, 50, L"GAME LEVELS");
    buttonPlayMenuCustomLevels = Button(500, 335, 200, 50, L"CUSTOM LEVELS");
    buttonPlayMenuBack = Button(500, 435, 200, 50, L"BACK");

    buttonMainMenuPlay.SetFont(L"i pixel u", 26, false);
    buttonMainMenuOptions.SetFont(L"i pixel u", 26, false);
    buttonMainMenuExit.SetFont(L"i pixel u", 26, false);
    buttonPlayMenuGameLevels.SetFont(L"i pixel u", 26, false);
    buttonPlayMenuCustomLevels.SetFont(L"i pixel u", 26, false);
    buttonPlayMenuBack.SetFont(L"i pixel u", 26, false);

    Mahex = Player{0, 0,
                   (HBITMAP) LoadImage(NULL, L"assets/images/player.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   (HBITMAP) LoadImage(NULL, L"assets/images/player_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   0.f, 0.f, true, 0, IDLE};

    std::vector<Tile> tiles;

    for(int i = 0; i < 25; ++i) {
        tiles.push_back({48 * i, 672,
                        (HBITMAP) LoadImage(NULL, L"assets/images/tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                        (HBITMAP) LoadImage(NULL, L"assets/images/tile_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                        true, true});
    }

    m_currentLevel.m_tiles = std::move(tiles);
    m_currentLevel.m_startingPoint = {56, 640};
}

void Game::Update() {
    CheckInput();
    if(m_state == GameState::IN_GAME) UpdatePlayer(Mahex);
    //UpdatePlayer(Huso);
}

void Game::UpdatePlayer(Player &player) {
    if(!player.m_grounded) {
        player.m_velY += 0.5f;
    }

    int newPosX = player.m_posX + (int) player.m_velX;
    int newPosY = player.m_posY + (int) player.m_velY;

    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    RECT currentRect = {player.m_posX, player.m_posY, player.m_posX + PLAYER_SIZE, player.m_posY + PLAYER_SIZE};
    RECT nextRect = {newPosX, newPosY, newPosX + PLAYER_SIZE, newPosY + PLAYER_SIZE};

    player.m_grounded = false;

    for(const Tile& tile : m_currentLevel.m_tiles) {
        RECT intersection;
        RECT groundCheck = {currentRect.left, currentRect.bottom, currentRect.right, currentRect.bottom + 1};
        RECT tileCheck = {tile.m_posX, tile.m_posY, tile.m_posX + PLAYER_SIZE, tile.m_posY + PLAYER_SIZE};
        if(IntersectRect(&intersection, &groundCheck, &tileCheck)) {
            player.m_grounded = true;
        }

        if(IntersectRect(&intersection, &nextRect, &tileCheck)) {
            if(player.m_velY > 0 && currentRect.bottom <= tileCheck.top) {
                newPosY = tileCheck.top - PLAYER_SIZE;
                player.m_velY = 0;
                player.m_grounded = true;
            } else if(player.m_velY < 0 && currentRect.top >= tileCheck.bottom) {
                newPosY = tileCheck.bottom;
                player.m_velY = 0;
            } else if(player.m_velX > 0 && currentRect.right <= tileCheck.left) {
                newPosX = tileCheck.left - PLAYER_SIZE;
                player.m_velX = 0;
            } else if(player.m_velX < 0 && currentRect.left >= tileCheck.right) {
                newPosX = tileCheck.right;
                player.m_velX = 0;
            }
        }
    }

    player.m_posX = newPosX;
    player.m_posY = newPosY;
}

void Game::Render() {
    switch(m_state) {
        case GameState::MAIN_MENU:
            RenderMainMenu();
            break;

        case GameState::PLAY_MENU:
            RenderPlayMenu();
            break;

        case GameState::GAME_LEVELS:
            RenderGameLevels();
            break;

        case GameState::CUSTOM_LEVELS:
            RenderCustomLevels();
            break;

        case GameState::LEVEL_EDITOR:
            RenderLevelEditor();
            break;

        case GameState::OPTIONS:
            RenderOptions();
            break;

        case GameState::IN_GAME:
            RenderInGame();
            break;

        case GameState::PAUSE:
            RenderInGame();
            RenderPause();
            break;
    }
}

void Game::RenderMainMenu() {
    HDC hdc = GetDC(m_hwnd);
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    SelectObject(hdcMem, m_backgroundMainMenu);
    BitBlt(hdcBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
    SelectObject(hdcMem, m_backgroundMaskMainMenu);
    BitBlt(hdcBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCAND);

    buttonMainMenuPlay.Render(hdcBuffer);
    buttonMainMenuOptions.Render(hdcBuffer);
    buttonMainMenuExit.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderPlayMenu() {
    HDC hdc = GetDC(m_hwnd);
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    SelectObject(hdcMem, m_backgroundMainMenu);
    BitBlt(hdcBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCPAINT);
    SelectObject(hdcMem, m_backgroundMaskMainMenu);
    BitBlt(hdcBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcMem, 0, 0, SRCAND);

    buttonPlayMenuGameLevels.Render(hdcBuffer);
    buttonPlayMenuCustomLevels.Render(hdcBuffer);
    buttonPlayMenuBack.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderGameLevels() {}

void Game::RenderCustomLevels() {}

void Game::RenderLevelEditor() {}

void Game::RenderOptions() {}

void Game::RenderInGame() {
    HDC hdc = GetDC(m_hwnd);

    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    //HBITMAP hbmOld = (HBITMAP) SelectObject(hdcMem, m_level.m_background);
    //BitBlt(hdcBuffer, 0, 0, 1280, 720, hdcMem, 0, 0, SRCCOPY);

    HBRUSH brush = CreateSolidBrush(0xe6d8ad);
    FillRect(hdcBuffer, &clientRect, brush);

    // Tiles
    for(const Tile& tile : m_currentLevel.m_tiles) {
        SelectObject(hdcMem, tile.m_mask);
        BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
        SelectObject(hdcMem, tile.m_bitmap);
        BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
    }

    // Player
    SelectObject(hdcMem, Mahex.m_mask);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, PLAYER_SIZE, PLAYER_SIZE, hdcMem, 0, 0, SRCPAINT);
    SelectObject(hdcMem, Mahex.m_bitmap);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, PLAYER_SIZE, PLAYER_SIZE, hdcMem, 0, 0, SRCAND);

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

    DeleteObject(brush);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderPause() {
    HDC hdc = GetDC(m_hwnd);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    int rectWidth = 300;
    int rectHeight = 200;
    int rectX = (clientRect.right - rectWidth) / 2;
    int finalRectY = (clientRect.bottom - rectHeight) / 2;

    if(!m_animationInProgress) {
        m_pauseMenuY = -rectHeight;
        m_pauseTargetY = finalRectY;
        m_animationInProgress = true;
    }

    if(m_pauseMenuY < m_pauseTargetY) {
        m_pauseMenuY += (m_pauseTargetY - m_pauseMenuY) * 0.1f;
        if(m_pauseTargetY - m_pauseMenuY < 0.5f) {
            m_pauseMenuY = m_pauseTargetY;
        }
    }

    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    RECT pauseRect = {rectX, (int) m_pauseMenuY, rectX + rectWidth, (int) m_pauseMenuY + rectHeight};
    FillRect(hdc, &pauseRect, whiteBrush);

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HPEN oldPen = (HPEN) SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rectX, (int) m_pauseMenuY, rectX + rectWidth, (int) m_pauseMenuY + rectHeight);

    DeleteObject(whiteBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    ReleaseDC(m_hwnd, hdc);
}

void Game::CheckInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(m_hwnd, &mousePos);

    //ResetButtonHoverStates();

    buttonMainMenuPlay.ResetHoverState();
    buttonMainMenuOptions.ResetHoverState();
    buttonMainMenuExit.ResetHoverState();
    buttonPlayMenuGameLevels.ResetHoverState();
    buttonPlayMenuCustomLevels.ResetHoverState();
    buttonPlayMenuBack.ResetHoverState();

    switch(m_state) {
        case GameState::MAIN_MENU:
            buttonMainMenuPlay.hovered = buttonMainMenuPlay.IsMouseOver(mousePos.x, mousePos.y);
            buttonMainMenuOptions.hovered = buttonMainMenuOptions.IsMouseOver(mousePos.x, mousePos.y);
            buttonMainMenuExit.hovered = buttonMainMenuExit.IsMouseOver(mousePos.x, mousePos.y);
            break;

        case GameState::PLAY_MENU:
            buttonPlayMenuGameLevels.hovered = buttonPlayMenuGameLevels.IsMouseOver(mousePos.x, mousePos.y);
            buttonPlayMenuCustomLevels.hovered = buttonPlayMenuCustomLevels.IsMouseOver(mousePos.x, mousePos.y);
            buttonPlayMenuBack.hovered = buttonPlayMenuBack.IsMouseOver(mousePos.x, mousePos.y);
            break;
    }

    if(IsKeyPressed(VK_LEFT)) {
        Mahex.m_direction = DIRECTION_LEFT;
        Mahex.m_velX = -MOVE_SPEED;
    } else if(IsKeyPressed(VK_RIGHT)) {
        Mahex.m_direction = DIRECTION_RIGHT;
        Mahex.m_velX = MOVE_SPEED;
    } else {
        Mahex.m_direction = IDLE;
        Mahex.m_velX = 0;
    }

    if(IsKeyPressed(VK_UP) && Mahex.m_grounded) {
        Mahex.m_velY = -JUMP_HEIGHT;
        Mahex.m_grounded = false;
    }

    if(IsKeyPressed('R')) {
        Mahex.m_posX = 100;
        Mahex.m_posY = 100;
        Mahex.m_velX = 0;
        Mahex.m_velY = 0;
    }

    if(IsKeyPressed(VK_ESCAPE)) {
        if(!m_escapeButtonPressed) {
            m_escapeButtonPressed = true;
            if(m_state == GameState::IN_GAME) {
                m_state = GameState::PAUSE;
            } else if(m_state == GameState::PAUSE) {
                m_state = GameState::IN_GAME;
                m_animationInProgress = false;
            }
        }
    } else {
        m_escapeButtonPressed = false;
    }

    if(IsKeyPressed(VK_BACK)) {
        if(m_state == GameState::IN_GAME)
            m_state = GameState::PLAY_MENU;
    }

    if(IsKeyPressed('Q')) {
        SendMessage(m_hwnd, WM_CLOSE, 0, 0);
    }

    if(IsKeyPressed(VK_LBUTTON)) {
        if(!m_mouseButtonPressed) {
            m_mouseButtonPressed = true;
            ProcessMouseClick(mousePos);
        }
    } else {
        m_mouseButtonPressed = false;
    }
}

void Game::ProcessMouseClick(POINT mousePos) {
    switch(m_state) {
        case GameState::MAIN_MENU:
            if(buttonMainMenuPlay.IsMouseOver(mousePos.x, mousePos.y)) {
                buttonMainMenuPlay.ResetHoverState();
                buttonMainMenuOptions.ResetHoverState();
                buttonMainMenuExit.ResetHoverState();
                m_state = GameState::PLAY_MENU;
            } else if(buttonMainMenuOptions.IsMouseOver(mousePos.x, mousePos.y)) {
                buttonMainMenuPlay.ResetHoverState();
                buttonMainMenuOptions.ResetHoverState();
                buttonMainMenuExit.ResetHoverState();
                m_state = GameState::OPTIONS;
            } else if(buttonMainMenuExit.IsMouseOver(mousePos.x, mousePos.y)) {
                SendMessage(m_hwnd, WM_CLOSE, 0, 0);
            }
            break;

        case GameState::PLAY_MENU:
            if(buttonPlayMenuGameLevels.IsMouseOver(mousePos.x, mousePos.y)) {
                buttonPlayMenuGameLevels.ResetHoverState();
                buttonPlayMenuCustomLevels.ResetHoverState();
                buttonPlayMenuBack.ResetHoverState();
                //m_state = GameState::GAME_LEVELS;
                m_state = GameState::IN_GAME;
                LoadLevel();
            } else if(buttonPlayMenuCustomLevels.IsMouseOver(mousePos.x, mousePos.y)) {
                buttonPlayMenuGameLevels.ResetHoverState();
                buttonPlayMenuCustomLevels.ResetHoverState();
                buttonPlayMenuBack.ResetHoverState();
                m_state = GameState::CUSTOM_LEVELS;
            } else if(buttonPlayMenuBack.IsMouseOver(mousePos.x, mousePos.y)) {
                buttonPlayMenuGameLevels.ResetHoverState();
                buttonPlayMenuCustomLevels.ResetHoverState();
                buttonPlayMenuBack.ResetHoverState();
                m_state = GameState::MAIN_MENU;
            }
            break;

        case GameState::GAME_LEVELS:
        case GameState::CUSTOM_LEVELS:
        case GameState::LEVEL_EDITOR:
        case GameState::OPTIONS:
        case GameState::IN_GAME:
            break;
    }
}

void Game::LoadLevel() {
    Mahex.m_posX = m_currentLevel.m_startingPoint.x;
    Mahex.m_posY = m_currentLevel.m_startingPoint.y;
    Mahex.m_velX = 0;
    Mahex.m_velY = 0;
    Mahex.m_grounded = false;
}

bool Game::LoadCustomFont() {
    m_fontHandle = (HANDLE) AddFontResourceEx(L"assets/fonts/pixel.ttf", FR_PRIVATE, 0);
    if(m_fontHandle == 0) {
        return false;
    }

    m_customFont = CreateFont(
        32,
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"pixel"
    );

    return m_customFont != nullptr;
}

void Game::CleanupFont() {
    if(m_customFont) {
        DeleteObject(m_customFont);
        m_customFont = nullptr;
    }
    if(m_fontHandle) {
        RemoveFontResourceEx(L"assets/fonts/YourFont.ttf", FR_PRIVATE, 0);
        m_fontHandle = nullptr;
    }
}

//bool Game::ToggleFullScreen(bool toFullScreen) {
//	if(toFullScreen == true) {
//		if(m_fullscreen == true)
//			return true;
// 
//		//DEVMODE dm = {0};
//
//		//if(!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
//		//	return false;
//
//		//devMode.dmPelsWidth = m_winWidth;
//		//devMode.dmPelsHeight = m_winHeight;
//		//devMode.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;
//
//		//if(ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
//		//	return false;
//
//		//SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_POPUP);
//		//SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, NULL);
//
//		//SetWindowPos(m_hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
//		//ShowWindow(m_hwnd, SW_SHOW);
//
//		//m_fullscreen = true;
//		//return true;
//
//		MONITORINFO mi = {sizeof(mi)};
//		if(!GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
//			return false;
//		}
//
//		// Remove window border styles
//		SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
//		SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
//
//		// Set window size to monitor size using the work area from MONITORINFO
//		SetWindowPos(m_hwnd, HWND_TOP,
//			mi.rcMonitor.left, mi.rcMonitor.top,
//			mi.rcMonitor.right - mi.rcMonitor.left,
//			mi.rcMonitor.bottom - mi.rcMonitor.top,
//			SWP_FRAMECHANGED);
//
//		return true;
//	} else {
//		if(m_fullscreen == false)
//			return true;
//
//		ChangeDisplaySettings(NULL, 0);
//
//		SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION);
//		SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_CLIENTEDGE);
//
//		SetWindowPos(m_hwnd, NULL, 150, 70, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
//		ShowWindow(m_hwnd, SW_SHOW);
//
//		m_fullscreen = false;
//		return true;
//
//		//SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
//		//SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW);
//
//		//// Set to a default windowed size (e.g. 1280x720)
//		//SetWindowPos(m_hwnd, HWND_TOP,
//		//	100, 100,    // x, y position
//		//	1280, 720,   // width, height
//		//	SWP_FRAMECHANGED);
//
//		//return true;
//	}
//}
//
//bool Game::ToggleFullScreen() {
//    if(!m_fullscreen) {
//        // Going fullscreen
//        MONITORINFO mi = {sizeof(mi)};
//        if(!GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
//            return false;
//        }
//
//        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
//        SetWindowPos(m_hwnd, HWND_TOP,
//            mi.rcMonitor.left, mi.rcMonitor.top,
//            mi.rcMonitor.right - mi.rcMonitor.left,
//            mi.rcMonitor.bottom - mi.rcMonitor.top,
//            SWP_FRAMECHANGED);
//
//        m_fullscreen = true;
//    } else {
//        // Going windowed - use standard window style
//        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
//
//        // Fixed windowed size
//        SetWindowPos(m_hwnd, HWND_TOP,
//            100, 100,    // position
//            800, 600,    // size
//            SWP_FRAMECHANGED);
//
//        m_fullscreen = false;
//    }
//
//    return true;
//}