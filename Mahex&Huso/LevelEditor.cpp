#include "headers/LevelEditor.hpp"


LevelEditor::LevelEditor(HWND hwnd) : m_hwndMainWindow{hwnd} {
    m_gridWidth = 25;
    m_gridHeight = 15;
    m_selectedTile = TileType::TILE;
    m_grid.resize(m_gridWidth, std::vector<TileType>(m_gridHeight, TileType::NONE));

    m_mahexStart = m_mahexEnd = {-1, -1};
    m_husoStart = m_husoEnd = {-1, -1};
}

void LevelEditor::SetTile(int x, int y, TileType tileType) {
    TileType currentTile = GetTile(x, y);

    if(x >= 0 && x < m_gridWidth && y >= 0 && y < m_gridHeight) {
        switch(currentTile) {
            case TileType::MAHEX_START_POINT:
                m_mahexStart = {-1, -1};
                break;
            case TileType::MAHEX_END_POINT:
                m_mahexEnd = {-1, -1};
                break;
            case TileType::HUSO_START_POINT:
                m_husoStart = {-1, -1};
                break;
            case TileType::HUSO_END_POINT:
                m_husoEnd = {-1, -1};
                break;
        }

        switch(tileType) {
            case TileType::MAHEX_START_POINT:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j] == TileType::MAHEX_START_POINT) {
                            m_grid[i][j] = TileType::NONE;
                        }
                    }
                }
                m_mahexStart = {x * 48 + 8, y * 48 + 16};
                break;
            case TileType::MAHEX_END_POINT:
                m_mahexEnd = {x * 48 + 8, y * 48 + 16};
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j] == TileType::MAHEX_END_POINT) {
                            m_grid[i][j] = TileType::NONE;
                        }
                    }
                }
                break;
            case TileType::HUSO_START_POINT:
                m_husoStart = {x * 48 + 8, y * 48 + 16};
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j] == TileType::HUSO_START_POINT) {
                            m_grid[i][j] = TileType::NONE;
                        }
                    }
                }
                break;
            case TileType::HUSO_END_POINT:
                m_husoEnd = {x * 48 + 8, y * 48 + 16};
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j] == TileType::HUSO_END_POINT) {
                            m_grid[i][j] = TileType::NONE;
                        }
                    }
                }
                break;
        }

        m_grid[x][y] = tileType;
    }
}

TileType LevelEditor::GetTile(int x, int y) const {
    if(x >= 0 && x < m_gridWidth && y >= 0 && y < m_gridHeight) {
        return m_grid[x][y];
    }
    return TileType::NONE;
}

void LevelEditor::SaveToFile(const std::string& filename) {
    json levelData;
    levelData["tiles"] = json::array();

    for(int x = 0; x < m_gridWidth; x++) {
        for(int y = 0; y < m_gridHeight; y++) {
            if(m_grid[x][y] != TileType::NONE) {
                json tileData = {
                    {"x", x * 48},
                    {"y", y * 48},
                    {"type", m_grid[x][y]}
                };
                levelData["tiles"].push_back(tileData);
            }
        }
    }

    std::ofstream file(filename);
    file << levelData.dump(4);
}

void LevelEditor::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        MessageBox(NULL, L"Failed to open level file!", L"Error", MB_OK);
        return;
    }

    json levelData;
    file >> levelData;

    m_grid.clear();
    m_grid.resize(m_gridWidth, std::vector<TileType>(m_gridHeight, TileType::NONE));

    for(const auto& tileData : levelData["tiles"]) {
        int x = tileData["x"] / 48;
        int y = tileData["y"] / 48;
        TileType type = tileData["type"];
        SetTile(x, y, type);
    }
}

void LevelEditor::CheckInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(m_hwndMainWindow, &mousePos);

    int gridX = mousePos.x / 48;
    int gridY = mousePos.y / 48;

    if(IsKeyPressed(VK_LBUTTON)) {
        if(!m_mousePressed) {
            SetTile(gridX, gridY, m_selectedTile);
        }
    } else if(IsKeyPressed(VK_RBUTTON)) {
        if(!m_mousePressed) {
            SetTile(gridX, gridY, TileType::NONE);
        }
    } else {
        m_mousePressed = false;
    }

    if(IsKeyPressed('0')) m_selectedTile = TileType::NONE;
    if(IsKeyPressed('1')) m_selectedTile = TileType::TILE;
    if(IsKeyPressed('2')) m_selectedTile = TileType::MAHEX_START_POINT;
    if(IsKeyPressed('3')) m_selectedTile = TileType::MAHEX_END_POINT;
    if(IsKeyPressed('4')) m_selectedTile = TileType::HUSO_START_POINT;
    if(IsKeyPressed('5')) m_selectedTile = TileType::HUSO_END_POINT;

    if(IsKeyPressed('S')) {
        SaveToFile("customLevel.json");
        MessageBox(NULL, L"Level saved!", L"Info", MB_OK);
    }
}

void LevelEditor::Render() {
    HDC hdc = GetDC(m_hwndMainWindow);

    HDC hdcBuffer = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwndMainWindow, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    HBRUSH backgroundBrush = CreateSolidBrush(0xe6d8ad);
    HPEN pen = CreatePen(PS_SOLID, 1, 0x0);

    SelectObject(hdcBuffer, pen);
    SelectObject(hdcBuffer, backgroundBrush);

    for(int i = 0; i < 15; ++i) {
        for(int j = 0; j < 25; ++j) {
            Rectangle(hdcBuffer, j * 48, i * 48, (j + 1) * 48, (i + 1) * 48);
        }
    }

    DeleteObject(backgroundBrush);
    DeleteObject(SelectObject(hdcBuffer, GetStockObject(BLACK_PEN)));

    HBRUSH tileBrush = CreateSolidBrush(0xff00);
    HBRUSH mahexStartBrush = CreateSolidBrush(0xffff00);
    HBRUSH mahexEndBrush = CreateSolidBrush(0xff);
    HBRUSH husoStartBrush = CreateSolidBrush(0xcbc0ff);
    HBRUSH husoEndBrush = CreateSolidBrush(0xa5ff);

    for(int x = 0; x < m_gridWidth; x++) {
        for(int y = 0; y < m_gridHeight; y++) {
            TileType tileType = m_grid[x][y];
            if(tileType != TileType::NONE) {
                RECT rect = {
                    x * 48,
                    y * 48,
                    (x + 1) * 48,
                    (y + 1) * 48
                };

                HBRUSH brush = nullptr;

                switch(tileType) {
                    case TileType::TILE:
                        brush = tileBrush;
                        break;
                    case TileType::MAHEX_START_POINT:
                        brush = mahexStartBrush;
                        break;
                    case TileType::MAHEX_END_POINT:
                        brush = mahexEndBrush;
                        break;
                    case TileType::HUSO_START_POINT:
                        brush = husoStartBrush;
                        break;
                    case TileType::HUSO_END_POINT:
                        brush = husoEndBrush;
                        break;
                }

                if(brush) {
                    FillRect(hdcBuffer, &rect, brush);
                }
            }
        }
    }

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteObject(tileBrush);
    DeleteObject(mahexStartBrush);
    DeleteObject(mahexEndBrush);
    DeleteObject(husoStartBrush);
    DeleteObject(husoEndBrush);
    ReleaseDC(m_hwndMainWindow, hdc);
}

void LevelEditor::Update() {
    CheckInput();
}