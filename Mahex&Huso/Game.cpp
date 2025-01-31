#include "headers/Game.hpp"

Game::Game(HWND hwnd) : m_hwnd{hwnd}, m_display{Display(hwnd)} {
    Mahex = Player{100, 100,
                   (HBITMAP) LoadImage(NULL, L"assets/images/player.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   (HBITMAP) LoadImage(NULL, L"assets/images/player_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                   0.f, 0.f, true, 0, IDLE};

    Tile tile1 = {100, 500,
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  true, true};
    Tile tile2 = {200, 500,
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  true, true};
    Tile tile3 = {300, 400,
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  (HBITMAP) LoadImage(NULL, L"assets/images/tile_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE),
                  true, true};

    m_level.push_back(tile1);
    m_level.push_back(tile2);
    m_level.push_back(tile3);
}

void Game::Update() {
    CheckInput();
    UpdatePlayer(Mahex);
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

    RECT currentRect = {player.m_posX, player.m_posY, player.m_posX + 64, player.m_posY + 64};
    RECT nextRect = {newPosX, newPosY, newPosX + 64, newPosY + 64};

    player.m_grounded = false;

    for(const Tile& tile : m_level) {
        RECT intersection;
        RECT groundCheck = {currentRect.left, currentRect.bottom, currentRect.right, currentRect.bottom + 1};
        RECT tileCheck = {tile.m_posX, tile.m_posY, tile.m_posX + 64, tile.m_posY + 64};
        if(IntersectRect(&intersection, &groundCheck, &tileCheck)) {
            player.m_grounded = true;
        }

        if(IntersectRect(&intersection, &nextRect, &tileCheck)) {
            if(player.m_velY > 0 && currentRect.bottom <= tileCheck.top) {
                newPosY = tileCheck.top - 64;
                player.m_velY = 0;
                player.m_grounded = true;
            } else if(player.m_velY < 0 && currentRect.top >= tileCheck.bottom) {
                newPosY = tileCheck.bottom;
                player.m_velY = 0;
            } else if(player.m_velX > 0 && currentRect.right <= tileCheck.left) {
                newPosX = tileCheck.left - 64;
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
    for(const Tile& tile : m_level) {
        SelectObject(hdcMem, tile.m_mask);
        BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, 64, 64, hdcMem, 0, 0, SRCPAINT);
        SelectObject(hdcMem, tile.m_bitmap);
        BitBlt(hdcBuffer, tile.m_posX, tile.m_posY, 64, 64, hdcMem, 0, 0, SRCAND);
    }

    // Player
    SelectObject(hdcMem, Mahex.m_mask);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, 64, 64, hdcMem, 0, 0, SRCPAINT);
    SelectObject(hdcMem, Mahex.m_bitmap);
    BitBlt(hdcBuffer, Mahex.m_posX, Mahex.m_posY, 64, 64, hdcMem, 0, 0, SRCAND);

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

    DeleteObject(brush);

    DeleteDC(hdcMem);
    DeleteObject(hbmBuffer);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmOldBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwnd, hdc);
}

void Game::CheckInput() {
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
    }

    if(IsKeyPressed('Q')) {
        SendMessage(m_hwnd, WM_CLOSE, 0, 0);
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