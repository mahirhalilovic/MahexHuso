#include "headers/Game.hpp"
#pragma comment(lib, "Msimg32.lib")

Game::Game(HWND hwnd) : m_hwnd{hwnd}, m_display{Display(hwnd)} {
    Mahex = Player{0, 0,
                   (HBITMAP) LoadImage(NULL, L"assets/images/mahex.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   (HBITMAP) LoadImage(NULL, L"assets/images/mahex_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   0.f, 0.f, true, 0, IDLE};
    Huso = Player{0, 0,
                   (HBITMAP) LoadImage(NULL, L"assets/images/huso.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   (HBITMAP) LoadImage(NULL, L"assets/images/huso_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   0.f, 0.f, true, 0, IDLE};

    wchar_t buffer[MAX_PATH];
    DWORD result = GetCurrentDirectory(MAX_PATH, buffer);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    m_currentWorkingDirectory = converter.to_bytes(buffer);

    m_levels.resize(6);
    m_finishedLevels.resize(5);
    m_finishedLevelsScores.resize(5);

    m_coins = m_score = 0;
    m_currentLevel = 1;

    if(!LoadSettings()) {
        MessageBox(m_hwnd, L"Settings could not have been loaded.", L"Error", MB_OK);
        SendMessage(m_hwnd, WM_CLOSE, 0, 0);
    }
    ConstructButtons();
    ConstructLabels();
    LoadBitmaps();
    LoadSounds();
}

Game::~Game() {
    SaveSettings();
}

void Game::ConstructButtons() {
    buttonBack = Button(950, 620, 200, 50, L"BACK");
    buttonBack.SetFont(L"i pixel u", 26, false);

    buttonMainMenuPlay = Button(500, 235, 200, 50, L"PLAY");
    buttonMainMenuOptions = Button(500, 335, 200, 50, L"OPTIONS");
    buttonMainMenuExit = Button(500, 435, 200, 50, L"EXIT");
    buttonMainMenuPlay.SetFont(L"i pixel u", 26, false);
    buttonMainMenuOptions.SetFont(L"i pixel u", 26, false);
    buttonMainMenuExit.SetFont(L"i pixel u", 26, false);

    buttonPlayMenuGameLevels = Button(500, 235, 200, 50, L"GAME LEVELS");
    buttonPlayMenuCustomLevels = Button(500, 335, 200, 50, L"CUSTOM LEVELS");
    buttonPlayMenuGameLevels.SetFont(L"i pixel u", 26, false);
    buttonPlayMenuCustomLevels.SetFont(L"i pixel u", 26, false);

    buttonGameLevelsNext = Button(950, 335, 200, 50, L"NEXT");
    buttonGameLevelsPrevious = Button(50, 335, 200, 50, L"PREVIOUS");
    buttonGameLevelsPlay = Button(500, 560, 200, 50, L"PLAY");
    buttonGameLevelsNext.SetFont(L"i pixel u", 26, false);
    buttonGameLevelsPrevious.SetFont(L"i pixel u", 26, false);
    buttonGameLevelsPlay.SetFont(L"i pixel u", 26, false);

    buttonCustomLevelsPlay = Button(450, 235, 300, 50, L"PLAY CUSTOM LEVEL");
    buttonCustomLevelsEdit = Button(450, 335, 300, 50, L"CREATE CUSTOM LEVEL");
    buttonCustomLevelsPlay.SetFont(L"i pixel u", 26, false);
    buttonCustomLevelsEdit.SetFont(L"i pixel u", 26, false);

    buttonPauseMenuResume = Button(0, 0, 200, 50, L"RESUME");
    buttonPauseMenuRestart = Button(0, 0, 200, 50, L"RESTART");
    buttonPauseMenuOptions = Button(0, 0, 200, 50, L"OPTIONS");
    buttonPauseMenuQuit = Button(0, 0, 200, 50, L"QUIT");
    buttonPauseMenuResume.SetFont(L"i pixel u", 26, false);
    buttonPauseMenuRestart.SetFont(L"i pixel u", 26, false);
    buttonPauseMenuOptions.SetFont(L"i pixel u", 26, false);
    buttonPauseMenuQuit.SetFont(L"i pixel u", 26, false);

    buttonGameWinNext = Button(0, 0, 200, 50, L"NEXT");
    buttonGameWinRestart = Button(0, 0, 200, 50, L"RESTART");
    buttonGameWinQuit = Button(0, 0, 200, 50, L"QUIT");
    buttonGameWinNext.SetFont(L"i pixel u", 26, false);
    buttonGameWinRestart.SetFont(L"i pixel u", 26, false);
    buttonGameWinQuit.SetFont(L"i pixel u", 26, false);

    buttonGameOverRestart = Button(0, 0, 200, 50, L"RESTART");
    buttonGameOverQuit = Button(0, 0, 200, 50, L"QUIT");
    buttonGameOverRestart.SetFont(L"i pixel u", 26, false);
    buttonGameOverQuit.SetFont(L"i pixel u", 26, false);

    buttonOptionsMusic = Button(450, 285, 300, 50, L"MUSIC: ON");
    buttonOptionsSoundEffects = Button(450, 385, 300, 50, L"SOUND EFFECTS: ON");
    buttonOptionsMusic.SetFont(L"i pixel u", 26, false);
    buttonOptionsSoundEffects.SetFont(L"i pixel u", 26, false);

    if(!musicEnabled) buttonOptionsMusic.SetText(L"MUSIC: OFF");
    if(!soundEffectsEnabled) buttonOptionsSoundEffects.SetText(L"SOUND EFFECTS: OFF");
}

void Game::CheckHoverStatus(const POINT &mousePos) {
    buttonBack.ResetHoverState();

    buttonMainMenuPlay.ResetHoverState();
    buttonMainMenuOptions.ResetHoverState();
    buttonMainMenuExit.ResetHoverState();

    buttonPlayMenuGameLevels.ResetHoverState();
    buttonPlayMenuCustomLevels.ResetHoverState();

    buttonGameLevelsNext.ResetHoverState();
    buttonGameLevelsPrevious.ResetHoverState();
    buttonGameLevelsPlay.ResetHoverState();

    buttonCustomLevelsPlay.ResetHoverState();
    buttonCustomLevelsEdit.ResetHoverState();

    buttonPauseMenuResume.ResetHoverState();
    buttonPauseMenuRestart.ResetHoverState();
    buttonPauseMenuOptions.ResetHoverState();
    buttonPauseMenuQuit.ResetHoverState();

    buttonGameWinNext.ResetHoverState();
    buttonGameWinRestart.ResetHoverState();
    buttonGameWinQuit.ResetHoverState();

    buttonGameOverRestart.ResetHoverState();
    buttonGameOverQuit.ResetHoverState();

    buttonOptionsMusic.ResetHoverState();
    buttonOptionsSoundEffects.ResetHoverState();


    buttonBack.hovered = buttonBack.IsMouseOver(mousePos.x, mousePos.y);

    buttonMainMenuPlay.hovered = buttonMainMenuPlay.IsMouseOver(mousePos.x, mousePos.y);
    buttonMainMenuOptions.hovered = buttonMainMenuOptions.IsMouseOver(mousePos.x, mousePos.y);
    buttonMainMenuExit.hovered = buttonMainMenuExit.IsMouseOver(mousePos.x, mousePos.y);

    buttonPlayMenuGameLevels.hovered = buttonPlayMenuGameLevels.IsMouseOver(mousePos.x, mousePos.y);
    buttonPlayMenuCustomLevels.hovered = buttonPlayMenuCustomLevels.IsMouseOver(mousePos.x, mousePos.y);

    buttonGameLevelsNext.hovered = buttonGameLevelsNext.IsMouseOver(mousePos.x, mousePos.y);
    buttonGameLevelsPrevious.hovered = buttonGameLevelsPrevious.IsMouseOver(mousePos.x, mousePos.y);
    buttonGameLevelsPlay.hovered = buttonGameLevelsPlay.IsMouseOver(mousePos.x, mousePos.y);

    buttonCustomLevelsPlay.hovered = buttonCustomLevelsPlay.IsMouseOver(mousePos.x, mousePos.y);
    buttonCustomLevelsEdit.hovered = buttonCustomLevelsEdit.IsMouseOver(mousePos.x, mousePos.y);

    buttonPauseMenuResume.hovered = buttonPauseMenuResume.IsMouseOver(mousePos.x, mousePos.y);
    buttonPauseMenuRestart.hovered = buttonPauseMenuRestart.IsMouseOver(mousePos.x, mousePos.y);
    buttonPauseMenuOptions.hovered = buttonPauseMenuOptions.IsMouseOver(mousePos.x, mousePos.y);
    buttonPauseMenuQuit.hovered = buttonPauseMenuQuit.IsMouseOver(mousePos.x, mousePos.y);

    buttonGameWinNext.hovered = buttonGameWinNext.IsMouseOver(mousePos.x, mousePos.y);
    buttonGameWinRestart.hovered = buttonGameWinRestart.IsMouseOver(mousePos.x, mousePos.y);
    buttonGameWinQuit.hovered = buttonGameWinQuit.IsMouseOver(mousePos.x, mousePos.y);

    buttonGameOverRestart.hovered = buttonGameOverRestart.IsMouseOver(mousePos.x, mousePos.y);
    buttonGameOverQuit.hovered = buttonGameOverQuit.IsMouseOver(mousePos.x, mousePos.y);

    buttonOptionsMusic.hovered = buttonOptionsMusic.IsMouseOver(mousePos.x, mousePos.y);
    buttonOptionsSoundEffects.hovered = buttonOptionsSoundEffects.IsMouseOver(mousePos.x, mousePos.y);
}

void Game::ConstructLabels() {
    m_label = Label(350, 50, 500, 100, L"MAHEX&&HUSO");
    m_label.SetFont(L"i pixel u", 72, false);

    labelGameLevelsLevel = Label(300, 50, 600, 32, L"");
    labelGameLevelsScore = Label(300, 85, 600, 32, L"");
    labelGameLevelsLevel.SetFont(L"i pixel u", 28, false);
    labelGameLevelsScore.SetFont(L"i pixel u", 28, false);

    labelGameWinScore = Label(0, 0, 200, 40, L"");
    labelGameWinHighScore = Label(0, 0, 200, 40, L"");
    labelGameWinScore.SetFont(L"i pixel u", 24, false);
    labelGameWinHighScore.SetFont(L"i pixel u", 24, false);

    labelGameOver = Label(0, 0, 200, 50, L"GAME OVER!!");
    labelGameOver.SetFont(L"i pixel u", 24, false);
}

void Game::LoadBitmaps() {
    m_backgroundMainMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_mainmenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_backgroundMaskMainMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_mask_mainmenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_backgroundPauseMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_pausemenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_backgroundMaskPauseMenu = (HBITMAP) LoadImage(NULL, L"assets/images/background_mask_pausemenu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    m_bitmapLevelOne = (HBITMAP) LoadImage(NULL, L"assets/level_images/level1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_bitmapLevelTwo = (HBITMAP) LoadImage(NULL, L"assets/level_images/level2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_bitmapLevelThree = (HBITMAP) LoadImage(NULL, L"assets/level_images/level3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_bitmapLevelFour = (HBITMAP) LoadImage(NULL, L"assets/level_images/level4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_bitmapLevelFive = (HBITMAP) LoadImage(NULL, L"assets/level_images/level5.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void Game::LoadSounds() {
    m_soundManager.PreloadSound("background", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/background.wav");
    m_soundManager.PreloadSound("click", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/click.wav");
    m_soundManager.PreloadSound("coin", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/coin.wav");
    m_soundManager.PreloadSound("gameover", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/gameover.wav");
    m_soundManager.PreloadSound("gamestart", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/gamestart.wav");
    m_soundManager.PreloadSound("gamewin", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/gamewin.wav");
    m_soundManager.PreloadSound("jump", std::wstring(m_currentWorkingDirectory.begin(), m_currentWorkingDirectory.end()) + L"/assets/sounds/jump.wav");
}

void Game::Update(int fpsCount) {
    if(fpsCount % 5 == 0) UpdateCounters();

    CheckInput();

    if(m_state == GameState::IN_GAME) {
        CheckWinningCondition();
        m_activePressureBlocks.clear();

        UpdatePlayer(Mahex);
        UpdatePlayer(Huso);

        CheckPressureBlocks();
    } else if(m_state == GameState::LEVEL_EDITOR) {
        m_levelEditor.Update();
        if(m_levelEditor.shouldExitToMainMenu) {
            UpdateWindowSize(false);
            m_state = GameState::CUSTOM_LEVELS;
            m_levelEditor.shouldExitToMainMenu = false;
        }
    }
}

void Game::UpdateCounters() {
    if(m_state == GameState::IN_GAME) {
        if(Mahex.m_grounded && Mahex.m_direction != Direction::IDLE) Mahex.m_spriteCounter = (Mahex.m_spriteCounter + 1) % 4;
        if(Huso.m_grounded && Huso.m_direction != Direction::IDLE) Huso.m_spriteCounter = (Huso.m_spriteCounter + 1) % 4;
    }
}

void Game::UpdatePlayer(Player &player) {
    if(!player.m_grounded) {
        player.m_velY += 0.5f;
    }

    int newPosX = player.m_posX + (int) player.m_velX;
    int newPosY = player.m_posY + (int) player.m_velY;

    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    RECT currentRect = {player.m_posX, player.m_posY, player.m_posX + PLAYER_WIDTH, player.m_posY + PLAYER_HEIGHT};
    RECT groundCheck = {currentRect.left, currentRect.bottom, currentRect.right, currentRect.bottom + 1};
    RECT nextRect = {newPosX, newPosY, newPosX + PLAYER_WIDTH, newPosY + PLAYER_HEIGHT};

    player.m_grounded = false;

    for(Tile& tile : m_levels[m_currentLevel - 1].m_tiles) {
        RECT intersection;
        RECT tileCheck = {tile.m_posX, tile.m_posY, tile.m_posX + TILE_SIZE, tile.m_posY + TILE_SIZE};

        if(tile.m_type == TileType::KEYDOWN_PLATE && tile.m_active) continue;
        if(tile.m_type == TileType::MAHEX_END || tile.m_type == TileType::HUSO_END) continue;
        if(tile.m_type == TileType::COIN && tile.m_active) continue;

        if(IntersectRect(&intersection, &groundCheck, &tileCheck)) {
            switch(tile.m_type) {
                case TileType::SPIKES:
                    m_state = GameState::GAME_OVER;
                    m_soundManager.StopBackgroundMusic();
                    if(soundEffectsEnabled) m_soundManager.PlayCustomSound("gameover");
                    return;
                case TileType::KEYDOWN_PLATE:
                case TileType::PRESSURE_PLATE_START:
                    if(player.m_velY < 0.1f) continue;
                    newPosY = tile.m_posY + 16 - PLAYER_HEIGHT;
                    player.m_grounded = true;
                    continue;
                case TileType::PRESSURE_BLOCK:
                    m_activePressureBlocks.insert(tile.m_id);
                    break;
            }

            player.m_grounded = true;
        }

        if(IntersectRect(&intersection, &nextRect, &tileCheck)) {
            if(tile.m_type == TileType::COIN) {
                tile.m_active = true;
                if(soundEffectsEnabled) m_soundManager.PlayCustomSound("coin");
                m_score += 10;
                continue;
            }

            if(player.m_velY > 0 && currentRect.bottom <= tileCheck.top + 2) {
                newPosY = tileCheck.top - PLAYER_HEIGHT;
                player.m_velY = 0;
                player.m_grounded = true;
            } else if(player.m_velY < 0 && currentRect.top >= tileCheck.bottom) {
                if(tile.m_type == TileType::PRESSURE_PLATE_START || tile.m_type == TileType::KEYDOWN_PLATE) {
                    newPosY = tileCheck.bottom - 16;
                } else {
                    newPosY = tileCheck.bottom;
                }
                player.m_velY = 0;
            } else if(player.m_velX > 0 && currentRect.right <= tileCheck.left) {
                newPosX = tileCheck.left - PLAYER_WIDTH;
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

void Game::CheckPressureBlocks() {
    for(Tile& tile : m_levels[m_currentLevel - 1].m_tiles) {
        if(tile.m_type == TileType::PRESSURE_BLOCK) {
            tile.m_active = (m_activePressureBlocks.find(tile.m_id) != m_activePressureBlocks.end());
        }
    }

    std::unordered_map<int, Tile*> plateMap;
    for(Tile &plate : m_levels[m_currentLevel - 1].m_tiles) {
        if(plate.m_type == TileType::PRESSURE_PLATE_START) {
            plateMap[plate.m_id] = &plate;
        }
    }

    for(Tile &tile : m_levels[m_currentLevel - 1].m_tiles) {
        if(tile.m_type == TileType::PRESSURE_BLOCK) {
            if(tile.m_active) {
                auto it = plateMap.find(tile.m_id);
                if(it != plateMap.end()) {
                    Tile *plate = it->second;
                    if(plate->m_movement == Orientation::VERTICAL) {
                        if(plate->m_startPos < plate->m_endPos) {
                            if(plate->m_posY < plate->m_endPos) plate->m_posY += 1;
                        } else {
                            if(plate->m_posY > plate->m_endPos) {
                                plate->m_posY -= 1;

                                RECT intersect;
                                RECT mahexCheck = {Mahex.m_posX, Mahex.m_posY, Mahex.m_posX + PLAYER_WIDTH, Mahex.m_posY + PLAYER_HEIGHT + 1};
                                RECT husoCheck = {Huso.m_posX, Huso.m_posY, Huso.m_posX + PLAYER_WIDTH, Huso.m_posY + PLAYER_HEIGHT + 1};
                                RECT tileCheck = {plate->m_posX, plate->m_posY, plate->m_posX + TILE_SIZE, plate->m_posY + TILE_SIZE};
                                if(IntersectRect(&intersect, &mahexCheck, &tileCheck)) {
                                    Mahex.m_posY -= 1;
                                } else if(IntersectRect(&intersect, &husoCheck, &tileCheck)) {
                                    Huso.m_posY -= 1;
                                }
                            }
                        }
                    } else {
                        if(plate->m_startPos < plate->m_endPos) {
                            if(plate->m_posX < plate->m_endPos) plate->m_posX += 1;
                        } else {
                            if(plate->m_posX > plate->m_endPos) plate->m_posX -= 1;
                        }
                    }
                }
            } else {
                auto it = plateMap.find(tile.m_id);
                if(it != plateMap.end()) {
                    Tile *plate = it->second;
                    if(plate->m_movement == Orientation::VERTICAL) {
                        if(plate->m_startPos < plate->m_endPos) {
                            if(plate->m_posY > plate->m_startPos) plate->m_posY -= 1;
                        } else {
                            if(plate->m_posY < plate->m_startPos) plate->m_posY += 1;
                        }
                    } else {
                        if(plate->m_startPos < plate->m_endPos) {
                            if(plate->m_posX > plate->m_startPos) plate->m_posX -= 1;
                        } else {
                            if(plate->m_posX < plate->m_startPos) plate->m_posX += 1;
                        }
                    }
                }
            }
        }
    }
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

        case GameState::GAME_WIN:
            RenderInGame();
            RenderGameWin();
            break;

        case GameState::GAME_OVER:
            RenderInGame();
            RenderGameOver();
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

    m_label.Render(hdcBuffer);
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
    buttonBack.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderGameLevels() {
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

    HPEN pen = CreatePen(PS_SOLID, 3, 0xb3922d);
    SelectObject(hdcBuffer, pen);
    Rectangle(hdcBuffer, 298, 178, 902, 542);
    DeleteObject(pen);

    switch(m_currentLevel) {
        case 1:
            SelectObject(hdcMem, m_bitmapLevelOne);
            break;
        case 2:
            SelectObject(hdcMem, m_bitmapLevelTwo);
            break;
        case 3:
            SelectObject(hdcMem, m_bitmapLevelThree);
            break;
        case 4:
            SelectObject(hdcMem, m_bitmapLevelFour);
            break;
        case 5:
            SelectObject(hdcMem, m_bitmapLevelFive);
            break;
    }

    BitBlt(hdcBuffer, 300, 180, 600, 360, hdcMem, 0, 0, SRCCOPY);

    labelGameLevelsLevel.Render(hdcBuffer);
    labelGameLevelsScore.Render(hdcBuffer);

    buttonGameLevelsNext.Render(hdcBuffer);
    buttonGameLevelsPrevious.Render(hdcBuffer);
    buttonGameLevelsPlay.Render(hdcBuffer);
    buttonBack.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderCustomLevels() {
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

    buttonCustomLevelsPlay.Render(hdcBuffer);
    buttonCustomLevelsEdit.Render(hdcBuffer);
    buttonBack.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderLevelEditor() {
    m_levelEditor.Render();
}

void Game::RenderOptions() {
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

    buttonOptionsMusic.Render(hdcBuffer);
    buttonOptionsSoundEffects.Render(hdcBuffer);
    buttonBack.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBuffer, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderInGame() {
    HDC hdc = GetDC(m_hwnd);

    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    HBRUSH brush = CreateSolidBrush(0xe6d8ad);
    FillRect(hdcBuffer, &clientRect, brush);

    // Tiles
    RenderTiles(hdc, hdcBuffer);

    // Player
    SelectObject(hdcMem, Mahex.m_mask);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, PLAYER_WIDTH, PLAYER_HEIGHT, hdcMem, Mahex.m_spriteCounter * PLAYER_WIDTH, (int) Mahex.m_direction * PLAYER_HEIGHT, SRCPAINT);
    SelectObject(hdcMem, Mahex.m_bitmap);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, PLAYER_WIDTH, PLAYER_HEIGHT, hdcMem, Mahex.m_spriteCounter * PLAYER_WIDTH, (int) Mahex.m_direction * PLAYER_HEIGHT, SRCAND);

    SelectObject(hdcMem, Huso.m_mask);
    BitBlt(hdcBuffer, Huso.m_posX, Huso.m_posY, PLAYER_WIDTH, PLAYER_HEIGHT, hdcMem, Huso.m_spriteCounter * PLAYER_WIDTH, (int) Huso.m_direction * PLAYER_HEIGHT, SRCPAINT);
    SelectObject(hdcMem, Huso.m_bitmap);
    BitBlt(hdcBuffer, Huso.m_posX, Huso.m_posY, PLAYER_WIDTH, PLAYER_HEIGHT, hdcMem, Huso.m_spriteCounter * PLAYER_WIDTH, (int) Huso.m_direction * PLAYER_HEIGHT, SRCAND);

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
    int rectHeight = 300;
    int rectX = (clientRect.right - rectWidth) / 2;
    int finalRectY = (clientRect.bottom - rectHeight) / 2;

    if(!m_animationInProgressPause) {
        m_pauseMenuY = -rectHeight;
        m_pauseTargetY = finalRectY;
        m_animationInProgressPause = true;
    }

    if(m_pauseMenuY < m_pauseTargetY) {
        m_pauseMenuY += (m_pauseTargetY - m_pauseMenuY) * 0.1f;
        if(m_pauseTargetY - m_pauseMenuY < 0.5f) {
            m_pauseMenuY = m_pauseTargetY;
        }
    }

    HBRUSH backgroundBrush = CreateSolidBrush(0xccb366);
    RECT pauseRect = {rectX, (int) m_pauseMenuY, rectX + rectWidth, (int) m_pauseMenuY + rectHeight};
    FillRect(hdc, &pauseRect, backgroundBrush);

    HPEN pen = CreatePen(PS_SOLID, 2, 0xb3922d);
    HPEN oldPen = (HPEN) SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rectX, (int) m_pauseMenuY, rectX + rectWidth, (int) m_pauseMenuY + rectHeight);

    buttonPauseMenuResume.SetPos(rectX + 50, m_pauseMenuY + 20);
    buttonPauseMenuRestart.SetPos(rectX + 50, m_pauseMenuY + 90);
    buttonPauseMenuOptions.SetPos(rectX + 50, m_pauseMenuY + 160);
    buttonPauseMenuQuit.SetPos(rectX + 50, m_pauseMenuY + 230);
    buttonPauseMenuResume.Render(hdc);
    buttonPauseMenuRestart.Render(hdc);
    buttonPauseMenuOptions.Render(hdc);
    buttonPauseMenuQuit.Render(hdc);

    DeleteObject(backgroundBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderGameWin() {
    HDC hdc = GetDC(m_hwnd);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    int rectWidth = 300;
    int rectHeight = 350;
    int rectX = (clientRect.right - rectWidth) / 2;
    int finalRectY = (clientRect.bottom - rectHeight) / 2;

    if(!m_animationInProgressGameWin) {
        m_gameWinMenuY = -rectHeight;
        m_gameWinTargetY = finalRectY;
        m_animationInProgressGameWin = true;
    }

    if(m_gameWinMenuY < m_gameWinTargetY) {
        m_gameWinMenuY += (m_gameWinTargetY - m_gameWinMenuY) * 0.1f;
        if(m_gameWinTargetY - m_gameWinMenuY < 0.5f) {
            m_gameWinMenuY = m_gameWinTargetY;
        }
    }

    HBRUSH backgroundBrush = CreateSolidBrush(0xccb366);
    RECT pauseRect = {rectX, (int) m_gameWinMenuY, rectX + rectWidth, (int) m_gameWinMenuY + rectHeight};
    FillRect(hdc, &pauseRect, backgroundBrush);

    HPEN pen = CreatePen(PS_SOLID, 2, 0xb3922d);
    HPEN oldPen = (HPEN) SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rectX, (int) m_gameWinMenuY, rectX + rectWidth, (int) m_gameWinMenuY + rectHeight);

    std::wstring str = L"SCORE: " + std::to_wstring(m_score);

    labelGameWinScore.SetText(str);

    labelGameWinScore.SetPos(rectX + 50, m_gameWinMenuY + 20);
    labelGameWinHighScore.SetPos(rectX + 50, m_gameWinMenuY + 60);

    labelGameWinScore.Render(hdc);
    labelGameWinHighScore.Render(hdc);
    
    if(!customLevelPlaying && m_currentLevel < 5) {
        buttonGameWinNext.SetPos(rectX + 50, m_gameWinMenuY + 140);
        buttonGameWinNext.Render(hdc);
    }
    buttonGameWinRestart.SetPos(rectX + 50, m_gameWinMenuY + 210);
    buttonGameWinQuit.SetPos(rectX + 50, m_gameWinMenuY + 280);
    buttonGameWinRestart.Render(hdc);
    buttonGameWinQuit.Render(hdc);

    DeleteObject(backgroundBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderGameOver() {
    HDC hdc = GetDC(m_hwnd);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);

    int rectWidth = 300;
    int rectHeight = 250;
    int rectX = (clientRect.right - rectWidth) / 2;
    int finalRectY = (clientRect.bottom - rectHeight) / 2;

    if(!m_animationInProgressGameOver) {
        m_gameOverMenuY = -rectHeight;
        m_gameOverTargetY = finalRectY;
        m_animationInProgressGameOver = true;
    }

    if(m_gameOverMenuY < m_gameOverTargetY) {
        m_gameOverMenuY += (m_gameOverTargetY - m_gameOverMenuY) * 0.1f;
        if(m_gameOverTargetY - m_gameOverMenuY < 0.5f) {
            m_gameOverMenuY = m_gameOverTargetY;
        }
    }

    HBRUSH backgroundBrush = CreateSolidBrush(0xccb366);
    RECT pauseRect = {rectX, (int) m_gameOverMenuY, rectX + rectWidth, (int) m_gameOverMenuY + rectHeight};
    FillRect(hdc, &pauseRect, backgroundBrush);

    HPEN pen = CreatePen(PS_SOLID, 2, 0xb3922d);
    HPEN oldPen = (HPEN) SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rectX, (int) m_gameOverMenuY, rectX + rectWidth, (int) m_gameOverMenuY + rectHeight);

    labelGameOver.SetPos(rectX + 50, m_gameOverMenuY + 20);
    labelGameOver.Render(hdc);

    buttonGameOverRestart.SetPos(rectX + 50, m_gameOverMenuY + 110);
    buttonGameOverQuit.SetPos(rectX + 50, m_gameOverMenuY + 180);
    buttonGameOverRestart.Render(hdc);
    buttonGameOverQuit.Render(hdc);

    DeleteObject(backgroundBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    ReleaseDC(m_hwnd, hdc);
}

void Game::RenderTiles(const HDC &hdc, const HDC &hdcBuffer) {
    HDC hdcMem = CreateCompatibleDC(hdc);

    for(const Tile& tile : m_levels[m_currentLevel - 1].m_tiles) {
        switch(tile.m_type) {
            case TileType::TILE:
                SelectObject(hdcMem, Tiles::m_tileBitmap);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, Tiles::m_tileMask);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                break;
            case TileType::SPIKES:
                SelectObject(hdcMem, Tiles::m_spikesBitmap);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, Tiles::m_spikesMask);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                break;
            case TileType::COIN:
                if(!tile.m_active) {
                    SelectObject(hdcMem, Tiles::m_coinBitmap);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY + 16, COIN_SIZE, COIN_SIZE, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_coinMask);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY + 16, COIN_SIZE, COIN_SIZE, hdcMem, 0, 0, SRCAND);
                }
                break;
            case TileType::KEYDOWN_BLOCK:
                SelectObject(hdcMem, Tiles::m_keydownBlockBitmap);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, Tiles::m_keydownBlockMask);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                break;
            case TileType::KEYDOWN_PLATE:
                if(tile.m_active) continue;

                if(tile.m_orientation == Orientation::HORIZONTAL) {
                    SelectObject(hdcMem, Tiles::m_plateHorizontalBitmap);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY + 16, PLATE_LENGTH, PLATE_WIDTH, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_plateHorizontalMask);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY + 16, PLATE_LENGTH, PLATE_WIDTH, hdcMem, 0, 0, SRCAND);
                } else {
                    SelectObject(hdcMem, Tiles::m_plateVerticalBitmap);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY, PLATE_WIDTH, PLATE_LENGTH, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_plateVerticalMask);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY, PLATE_WIDTH, PLATE_LENGTH, hdcMem, 0, 0, SRCAND);
                }
                break;
            case TileType::PRESSURE_BLOCK:
                if(tile.m_active) {
                    SelectObject(hdcMem, Tiles::m_pressureBlockActiveBitmap);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_pressureBlockActiveMask);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                } else {
                    SelectObject(hdcMem, Tiles::m_pressureBlockBitmap);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_pressureBlockMask);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                }
                break;
            case TileType::PRESSURE_PLATE_START:
                if(tile.m_orientation == Orientation::HORIZONTAL) {
                    SelectObject(hdcMem, Tiles::m_plateHorizontalBitmap);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY + 16, PLATE_LENGTH, PLATE_WIDTH, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_plateHorizontalMask);
                    BitBlt(hdcBuffer, tile.m_posX, tile.m_posY + 16, PLATE_LENGTH, PLATE_WIDTH, hdcMem, 0, 0, SRCAND);
                } else {
                    SelectObject(hdcMem, Tiles::m_plateVerticalBitmap);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY, PLATE_WIDTH, PLATE_LENGTH, hdcMem, 0, 0, SRCPAINT);
                    SelectObject(hdcMem, Tiles::m_plateVerticalMask);
                    BitBlt(hdcBuffer, tile.m_posX + 16, tile.m_posY, PLATE_WIDTH, PLATE_LENGTH, hdcMem, 0, 0, SRCAND);
                }
                break;
            case TileType::MAHEX_END:
                SelectObject(hdcMem, Tiles::m_mahexEndBitmap);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, Tiles::m_mahexEndMask);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                break;
            case TileType::HUSO_END:
                SelectObject(hdcMem, Tiles::m_husoEndBitmap);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCPAINT);
                SelectObject(hdcMem, Tiles::m_husoEndMask);
                BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, TILE_SIZE, TILE_SIZE, hdcMem, 0, 0, SRCAND);
                break;
        }
    }

    DeleteDC(hdcMem);
}

void Game::CheckInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(m_hwnd, &mousePos);

    CheckHoverStatus(mousePos);

    if(m_state == GameState::IN_GAME) {
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
            if(soundEffectsEnabled) m_soundManager.PlayCustomSound("jump");
        }
        if(IsKeyPressed(VK_DOWN)) {
            int tileX = (Mahex.m_posX + 16) / TILE_SIZE;
            int tileY = (Mahex.m_posY + PLAYER_HEIGHT) / TILE_SIZE;

            for(Tile& tile : m_levels[m_currentLevel - 1].m_tiles) {
                int currentTileX = tile.m_posX / TILE_SIZE;
                int currentTileY = tile.m_posY / TILE_SIZE;

                if(currentTileX == tileX && currentTileY == tileY) {
                    if(tile.m_type == TileType::KEYDOWN_BLOCK) {
                        for(Tile &plate : m_levels[m_currentLevel - 1].m_tiles) {
                            if(plate.m_id == tile.m_id)
                                plate.m_active = true;
                        }
                        break;
                    }
                }
            }
        }

        if(IsKeyPressed('A')) {
            Huso.m_direction = DIRECTION_LEFT;
            Huso.m_velX = -MOVE_SPEED;
        } else if(IsKeyPressed('D')) {
            Huso.m_direction = DIRECTION_RIGHT;
            Huso.m_velX = MOVE_SPEED;
        } else {
            Huso.m_direction = IDLE;
            Huso.m_velX = 0;
        }
        if(IsKeyPressed('W') && Huso.m_grounded) {
            Huso.m_velY = -JUMP_HEIGHT;
            Huso.m_grounded = false;
            if(soundEffectsEnabled) m_soundManager.PlayCustomSound("jump");
        }
        if(IsKeyPressed('S')) {
            int tileX = (Huso.m_posX + 16) / TILE_SIZE;
            int tileY = (Huso.m_posY + PLAYER_HEIGHT) / TILE_SIZE;

            for(Tile& tile : m_levels[m_currentLevel - 1].m_tiles) {
                int currentTileX = tile.m_posX / TILE_SIZE;
                int currentTileY = tile.m_posY / TILE_SIZE;

                if(currentTileX == tileX && currentTileY == tileY) {
                    if(tile.m_type == TileType::KEYDOWN_BLOCK) {
                        for(Tile &plate : m_levels[m_currentLevel - 1].m_tiles) {
                            if(plate.m_id == tile.m_id)
                                plate.m_active = true;
                        }
                        break;
                    }
                }
            }
        }
    }

    if(IsKeyPressed(VK_ESCAPE)) {
        if(!m_escapeButtonPressed) {
            m_escapeButtonPressed = true;
            if(m_state == GameState::IN_GAME) {
                m_state = GameState::PAUSE;
            } else if(m_state == GameState::PAUSE) {
                m_state = GameState::IN_GAME;
                m_animationInProgressPause = false;
            }
        }
    } else {
        m_escapeButtonPressed = false;
    }

    if(IsKeyPressed(VK_LBUTTON)) {
        if(!m_mouseButtonPressed) {
            if(m_soundManager.IsSoundPlaying("gamewin") || m_soundManager.IsSoundPlaying("gameover")) return;
            m_mouseButtonPressed = true;
            if(soundEffectsEnabled) m_soundManager.PlayCustomSound("click");
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
                m_state = GameState::PLAY_MENU;
            } else if(buttonMainMenuOptions.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::OPTIONS;
            } else if(buttonMainMenuExit.IsMouseOver(mousePos.x, mousePos.y)) {
                SaveSettings();
                SendMessage(m_hwnd, WM_CLOSE, 0, 0);
            }
            break;

        case GameState::PLAY_MENU:
            if(buttonPlayMenuGameLevels.IsMouseOver(mousePos.x, mousePos.y)) {
                if(customLevelPlaying) customLevelPlaying = false;
                m_state = GameState::GAME_LEVELS;
                m_currentLevel = 1;

                std::wstring level = L"LEVEL: " + std::to_wstring(m_currentLevel);
                labelGameLevelsLevel.SetText(level);

                if(m_finishedLevels[m_currentLevel - 1]) {
                    std::wstring str = L"SCORE: ";
                    str += std::to_wstring(m_finishedLevelsScores[m_currentLevel - 1]);
                    labelGameLevelsScore.SetText(str);
                } else {
                    labelGameLevelsScore.SetText(L"YOU HAVE NOT FINISHED THIS LEVEL YET!");
                }
            } else if(buttonPlayMenuCustomLevels.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::CUSTOM_LEVELS;
            } else if(buttonBack.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::MAIN_MENU;
            }
            break;

        case GameState::GAME_LEVELS:
            if(buttonGameLevelsNext.IsMouseOver(mousePos.x, mousePos.y)) {
                if(m_currentLevel < 5 && m_currentLevel < (m_numOfFinishedLevels + 1)) ++m_currentLevel;

                std::wstring level = L"LEVEL: " + std::to_wstring(m_currentLevel);
                labelGameLevelsLevel.SetText(level);

                if(m_finishedLevels[m_currentLevel - 1]) {
                    std::wstring score = L"SCORE: " + std::to_wstring(m_finishedLevelsScores[m_currentLevel - 1]);
                    labelGameLevelsScore.SetText(score);
                } else {
                    labelGameLevelsScore.SetText(L"YOU HAVE NOT FINISHED THIS LEVEL YET!");
                }
            } else if(buttonGameLevelsPrevious.IsMouseOver(mousePos.x, mousePos.y)) {
                if(m_currentLevel > 1) --m_currentLevel;

                std::wstring level = L"LEVEL: " + std::to_wstring(m_currentLevel);
                labelGameLevelsLevel.SetText(level);

                if(m_finishedLevels[m_currentLevel - 1]) {
                    std::wstring score = L"SCORE: " + std::to_wstring(m_finishedLevelsScores[m_currentLevel - 1]);
                    labelGameLevelsScore.SetText(score);
                } else {
                    labelGameLevelsScore.SetText(L"YOU HAVE NOT FINISHED THIS LEVEL YET!");
                }
            } else if(buttonGameLevelsPlay.IsMouseOver(mousePos.x, mousePos.y)) {
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                m_soundManager.StopBackgroundMusic();
                if(musicEnabled) m_soundManager.PlayBackgroundMusic();
            } else if(buttonBack.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::PLAY_MENU;
            }
            break;

        case GameState::CUSTOM_LEVELS:
            if(buttonCustomLevelsPlay.IsMouseOver(mousePos.x, mousePos.y)) {
                m_currentLevel = 6;
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                customLevelPlaying = true;
            } else if(buttonCustomLevelsEdit.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::LEVEL_EDITOR;
                m_levelEditor = LevelEditor(m_hwnd);
                UpdateWindowSize(true);
            } else if(buttonBack.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::PLAY_MENU;
            }
            break;

        case GameState::OPTIONS:
            if(buttonOptionsMusic.IsMouseOver(mousePos.x, mousePos.y)) {
                musicEnabled = !musicEnabled;

                if(musicEnabled) {
                    buttonOptionsMusic.SetText(L"MUSIC: ON");
                } else {
                    buttonOptionsMusic.SetText(L"MUSIC: OFF");
                }
            } else if(buttonOptionsSoundEffects.IsMouseOver(mousePos.x, mousePos.y)) {
                soundEffectsEnabled = !soundEffectsEnabled;

                if(soundEffectsEnabled) {
                    buttonOptionsSoundEffects.SetText(L"SOUND EFFECTS: ON");
                } else {
                    buttonOptionsSoundEffects.SetText(L"SOUND EFFECTS: OFF");
                }
            } else if(buttonBack.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::MAIN_MENU;
            }
            break;

        case GameState::LEVEL_EDITOR:
        case GameState::IN_GAME:
            break;

        case GameState::PAUSE:
            if(buttonPauseMenuResume.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::IN_GAME;
            } else if(buttonPauseMenuRestart.IsMouseOver(mousePos.x, mousePos.y)) {
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                m_soundManager.StopBackgroundMusic();
                if(musicEnabled) m_soundManager.PlayBackgroundMusic();
            } else if(buttonPauseMenuOptions.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::OPTIONS;
            } else if(buttonPauseMenuQuit.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::MAIN_MENU;
                if(customLevelPlaying) customLevelPlaying = false;
            }
            break;

        case GameState::GAME_WIN:
            if(buttonGameWinNext.IsMouseOver(mousePos.x, mousePos.y)) {
                ++m_currentLevel;
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                m_soundManager.StopBackgroundMusic();
                if(musicEnabled) m_soundManager.PlayBackgroundMusic();
            } else if(buttonGameWinRestart.IsMouseOver(mousePos.x, mousePos.y)) {
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                m_soundManager.StopBackgroundMusic();
                if(musicEnabled) m_soundManager.PlayBackgroundMusic();
            } else if(buttonGameWinQuit.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::MAIN_MENU;
                if(customLevelPlaying) customLevelPlaying = false;
            }
            break;

        case GameState::GAME_OVER:
            if(buttonGameOverRestart.IsMouseOver(mousePos.x, mousePos.y)) {
                LoadLevel(m_currentLevel);
                m_state = GameState::IN_GAME;
                m_soundManager.StopBackgroundMusic();
                if(musicEnabled) m_soundManager.PlayBackgroundMusic();
            } else if(buttonGameOverQuit.IsMouseOver(mousePos.x, mousePos.y)) {
                m_state = GameState::MAIN_MENU;
                if(customLevelPlaying) customLevelPlaying = false;
            }
            break;
    }
}

void Game::LoadLevel(int newLevel) {
    if(newLevel == 6) {
        OPENFILENAMEA ofn;
        char szFile[260] = {0};

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = m_hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if(GetOpenFileNameA(&ofn)) {
            customLevelPath = ofn.lpstrFile;
        } else {
            MessageBox(m_hwnd, L"Could not load custom level", L"Error", MB_OK | MB_ICONWARNING);
        }

        if(!LoadLevelFromJSON(customLevelPath, 5)) MessageBox(m_hwnd, L"Could not load level from JSON!", L"Error", MB_OK);

        Mahex.m_posX = m_levels[m_currentLevel - 1].m_startMahex.x;
        Mahex.m_posY = m_levels[m_currentLevel - 1].m_startMahex.y;
        Mahex.m_velX = 0;
        Mahex.m_velY = 0;
        Mahex.m_grounded = false;

        Huso.m_posX = m_levels[m_currentLevel - 1].m_startHuso.x;
        Huso.m_posY = m_levels[m_currentLevel - 1].m_startHuso.y;
        Huso.m_velX = 0;
        Huso.m_velY = 0;
        Huso.m_grounded = false;
    } else {
        std::string path = m_currentWorkingDirectory + "/levels/level";
        path += std::to_string(newLevel);
        path += ".json";

        LoadLevelFromJSON(path, newLevel - 1);

        Mahex.m_posX = m_levels[m_currentLevel - 1].m_startMahex.x;
        Mahex.m_posY = m_levels[m_currentLevel - 1].m_startMahex.y;
        Mahex.m_velX = 0;
        Mahex.m_velY = 0;
        Mahex.m_grounded = false;

        Huso.m_posX = m_levels[m_currentLevel - 1].m_startHuso.x;
        Huso.m_posY = m_levels[m_currentLevel - 1].m_startHuso.y;
        Huso.m_velX = 0;
        Huso.m_velY = 0;
        Huso.m_grounded = false;
    }
    m_score = 0;
}

void Game::CheckWinningCondition() {
    RECT currentMahexRect = {Mahex.m_posX, Mahex.m_posY, Mahex.m_posX + PLAYER_WIDTH, Mahex.m_posY + PLAYER_HEIGHT};
    RECT currentHusoRect = {Huso.m_posX, Huso.m_posY, Huso.m_posX + PLAYER_WIDTH, Huso.m_posY + PLAYER_HEIGHT};
    RECT tileMahexEnd = {m_mahexEnd.x + 22, m_mahexEnd.y + 46, m_mahexEnd.x + 26, m_mahexEnd.y + TILE_SIZE};
    RECT tileHusoEnd = {m_husoEnd.x + 22, m_husoEnd.y + 46, m_husoEnd.x + 26, m_husoEnd.y + TILE_SIZE};

    RECT intersection, intersection2;
    bool mahexEnd = IntersectRect(&intersection, &currentMahexRect, &tileMahexEnd);
    bool husoEnd = IntersectRect(&intersection2, &currentHusoRect, &tileHusoEnd);

    if(mahexEnd && husoEnd) {
        m_state = GameState::GAME_WIN;
        m_soundManager.StopBackgroundMusic();
        if(soundEffectsEnabled) m_soundManager.PlayCustomSound("gamewin");

        if(m_currentLevel < 6) {
            m_finishedLevels[m_currentLevel - 1] = true;
            ++m_numOfFinishedLevels;
            if(m_score > m_finishedLevelsScores[m_currentLevel - 1]) {
                m_finishedLevelsScores[m_currentLevel - 1] = m_score;

                labelGameWinHighScore.SetText(L"NEW HIGH SCORE!");
            } else {
                std::wstring score = L"HIGH SCORE: " + std::to_wstring(m_finishedLevelsScores[m_currentLevel - 1]);
                labelGameWinHighScore.SetText(score);
            }

            SaveSettings();
        }
    }
}

bool Game::LoadLevelFromJSON(std::string path, int id) {
    try {
        std::ifstream file(path);
        if(!file.is_open()) {
            MessageBox(NULL, L"Could not open JSON file", L"Error", MB_OK | MB_ICONERROR);
            return false;
        }

        json levelData;
        file >> levelData;
        file.close();

        m_loadedLevel.m_tiles.clear();

        for(const auto& tileData : levelData["tiles"]) {
            if(m_levelEditor.StringToTileType(tileData["type"]) == TileType::MAHEX_START) {
                m_loadedLevel.m_startMahex.x = tileData["x"];
                m_loadedLevel.m_startMahex.y = tileData["y"];
            } else if(m_levelEditor.StringToTileType(tileData["type"]) == TileType::HUSO_START) {
                m_loadedLevel.m_startHuso.x = tileData["x"];
                m_loadedLevel.m_startHuso.y = tileData["y"];
            } else if(m_levelEditor.StringToTileType(tileData["type"]) == TileType::MAHEX_END) {
                m_mahexEnd.x = tileData["x"];
                m_mahexEnd.y = tileData["y"];
                m_loadedLevel.m_tiles.push_back(LoadTile(tileData));
            } else if(m_levelEditor.StringToTileType(tileData["type"]) == TileType::HUSO_END) {
                m_husoEnd.x = tileData["x"];
                m_husoEnd.y = tileData["y"];
                m_loadedLevel.m_tiles.push_back(LoadTile(tileData));
            } else {
                m_loadedLevel.m_tiles.push_back(LoadTile(tileData));
            }
        }

        m_levels[id] = std::move(m_loadedLevel);

        return true;

    } catch(const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return false;
    }
}

Tile Game::LoadTile(const json &tileData) {
    Tile newTile;

    newTile.m_posX = tileData["x"];
    newTile.m_posY = tileData["y"];
    newTile.m_type = m_levelEditor.StringToTileType(tileData["type"]);

    switch(newTile.m_type) {
        case TileType::TILE:
        case TileType::SPIKES:
        case TileType::COIN:
        case TileType::HUSO_START:
        case TileType::HUSO_END:
            break;

        case TileType::KEYDOWN_BLOCK:
        case TileType::PRESSURE_BLOCK:
            newTile.m_id = tileData["id"];
            break;

        case TileType::KEYDOWN_PLATE:
            newTile.m_id = tileData["id"];
            newTile.m_orientation = m_levelEditor.StringToOrientation(tileData["orientation"]);
            break;

        case TileType::PRESSURE_PLATE_START:
            newTile.m_id = tileData["id"];
            newTile.m_orientation = m_levelEditor.StringToOrientation(tileData["orientation"]);
            newTile.m_startPos = tileData["startPos"];
            newTile.m_endPos = tileData["endPos"];
            newTile.m_movement = m_levelEditor.StringToOrientation(tileData["movement"]);
            break;
    }

    return newTile;
}

bool Game::LoadSettings() {
    try {
        std::string path = m_currentWorkingDirectory + "/settings.json";
        std::ifstream file(path);
        if(!file.is_open()) {
            return false;
        }

        json settings;
        file >> settings;
        file.close();

        json levelData = settings["levels"];

        for(int i = 1; i <= 5; ++i) {
            std::string key = "level" + std::to_string(i);

            m_finishedLevels[i - 1] = settings["levels"][key]["finished"];
            m_finishedLevelsScores[i - 1] = settings["levels"][key]["highscore"];

            if(m_finishedLevels[i - 1]) ++m_numOfFinishedLevels;
        }

        musicEnabled = settings["music"];
        soundEffectsEnabled = settings["soundEffects"];

        return true;

    } catch(const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return false;
    }
}

void Game::SaveSettings() {
    try {
        json data;
        data["levels"] = json::object();

        for(int i = 1; i <= 5; ++i) {
            json levelData = {
                {"finished", m_finishedLevels[i - 1]},
                {"highscore", m_finishedLevelsScores[i - 1]}
            };

            std::string levelKey = "level" + std::to_string(i);
            data["levels"][levelKey] = levelData;
        }

        data["music"] = musicEnabled;
        data["soundEffects"] = soundEffectsEnabled;

        std::string path = m_currentWorkingDirectory + "/settings.json";
        std::ofstream file(path);
        file << data.dump(4);
        file.close();
    } catch(const std::exception& e) {
        MessageBoxA(m_hwnd, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

void Game::UpdateWindowSize(bool isEditor) {
    RECT windowRect;
    GetWindowRect(m_hwnd, &windowRect);
    int currentX = windowRect.left;
    int currentY = windowRect.top;

    if(isEditor) {
        SetWindowPos(m_hwnd, NULL,
            currentX - 200, currentY,
            WINDOW_WIDTH_EDITOR + 16, WINDOW_HEIGHT + 39,
            SWP_NOZORDER);
    } else {
        SetWindowPos(m_hwnd, NULL,
            currentX + 200, currentY,
            WINDOW_WIDTH + 16, WINDOW_HEIGHT + 39,
            SWP_NOZORDER);
    }
}

//bool Game::LoadCustomFont() {
//    m_fontHandle = (HANDLE) AddFontResourceEx(L"assets/fonts/pixel.ttf", FR_PRIVATE, 0);
//    if(m_fontHandle == 0) {
//        return false;
//    }
//
//    m_customFont = CreateFont(
//        32,
//        0,
//        0,
//        0,
//        FW_NORMAL,
//        FALSE,
//        FALSE,
//        FALSE,
//        DEFAULT_CHARSET,
//        OUT_TT_PRECIS,
//        CLIP_DEFAULT_PRECIS,
//        CLEARTYPE_QUALITY,
//        DEFAULT_PITCH | FF_DONTCARE,
//        L"pixel"
//    );
//
//    return m_customFont != nullptr;
//}
//
//void Game::CleanupFont() {
//    if(m_customFont) {
//        DeleteObject(m_customFont);
//        m_customFont = nullptr;
//    }
//    if(m_fontHandle) {
//        RemoveFontResourceEx(L"assets/fonts/YourFont.ttf", FR_PRIVATE, 0);
//        m_fontHandle = nullptr;
//    }
//}

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