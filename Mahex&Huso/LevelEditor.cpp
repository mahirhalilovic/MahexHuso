#include "headers/LevelEditor.hpp"

LevelEditor::LevelEditor(HWND hwnd) : m_hwndMainWindow{hwnd} {
    m_gridWidth = 25;
    m_gridHeight = 15;
    m_selectedTile = TileType::TILE;
    m_grid.resize(m_gridWidth, std::vector<TileType>(m_gridHeight, TileType::NONE));

    m_mahexStart = m_mahexEnd = {-1, -1};
    m_husoStart = m_husoEnd = {-1, -1};

    m_buttonCancel = Button(1220, 650, 100, 50, L"CANCEL");
    m_buttonEdit = Button(1350, 650, 100, 50, L"EDIT");
    m_buttonSave = Button(1480, 650, 100, 50, L"SAVE");

    m_buttonCancel.SetFont(L"i pixel u", 26, false);
    m_buttonEdit.SetFont(L"i pixel u", 26, false);
    m_buttonSave.SetFont(L"i pixel u", 26, false);
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

std::string LevelEditor::TileTypeToString(const TileType &tile) {
    switch(tile) {
        case TileType::TILE:
            return "TILE";
        case TileType::MAHEX_START_POINT:
            return "MAHEX_START_POINT";
        case TileType::MAHEX_END_POINT:
            return "MAHEX_END_POINT";
        case TileType::HUSO_START_POINT:
            return "HUSO_START_POINT";
        case TileType::HUSO_END_POINT:
            return "HUSO_END_POINT";
        default:
            return "";
    }
}

TileType LevelEditor::StringToTileType(const std::string &str) {
    if(str == "TILE") return TileType::TILE;
    else if(str == "MAHEX_START_POINT") return TileType::MAHEX_START_POINT;
    else if(str == "MAHEX_END_POINT") return TileType::MAHEX_END_POINT;
    else if(str == "HUSO_START_POINT") return TileType::HUSO_START_POINT;
    else if(str == "HUSO_END_POINT") return TileType::HUSO_END_POINT;
    else return TileType::NONE;
}

bool LevelEditor::SaveToFile(const std::string& filename) {
    try {
        json levelData;
        levelData["tiles"] = json::array();

        for(int x = 0; x < m_gridWidth; x++) {
            for(int y = 0; y < m_gridHeight; y++) {
                if(m_grid[x][y] != TileType::NONE) {
                    json tileData = {
                        {"x", x * 48},
                        {"y", y * 48},
                        {"bitmap", "assets/images/tile.bmp"},
                        {"mask", "assets/images/tile_mask.bmp"},
                        {"safe", true},
                        {"collideable", true},
                        {"type", TileTypeToString(m_grid[x][y])}
                    };
                    levelData["tiles"].push_back(tileData);
                }
            }
        }

        if(m_mahexStart.x == -1 || m_mahexEnd.x == -1 || m_husoStart.x == -1 || m_husoEnd.x == -1) {
            MessageBox(m_hwndMainWindow, L"Start and end positions must be defined!", L"Error", MB_OK);
            return false;
        }

        json startPoints = {
            {"mahex", {
                {"x", m_mahexStart.x},
                {"y", m_mahexStart.y}
            }},
            {"huso", {
                {"x", m_husoStart.x},
                {"y", m_husoStart.y}
            }}
        };
        json endPoints = {
            {"mahex", {
                {"x", m_mahexEnd.x},
                {"y", m_mahexEnd.y}
            }},
            {"huso", {
                {"x", m_husoEnd.x},
                {"y", m_husoEnd.y}
            }}
        };

        levelData["start_points"] = startPoints;
        levelData["end_points"] = endPoints;

        std::ofstream file(filename);
        file << levelData.dump(4);
    } catch(const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
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
        TileType type = StringToTileType(tileData["type"]);
        SetTile(x, y, type);
    }

    m_mahexStart.x = levelData["start_points"]["mahex"]["x"];
    m_mahexStart.y = levelData["start_points"]["mahex"]["y"];
    m_mahexEnd.x = levelData["end_points"]["mahex"]["x"];
    m_mahexEnd.y = levelData["end_points"]["mahex"]["y"];
    m_husoStart.x = levelData["start_points"]["huso"]["x"];
    m_husoStart.y = levelData["start_points"]["huso"]["y"];
    m_husoEnd.x = levelData["end_points"]["huso"]["x"];
    m_husoEnd.y = levelData["end_points"]["huso"]["y"];
}

bool LevelEditor::ShowSaveFileDialog(std::string& outFilePath) {
    OPENFILENAMEA ofn;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hwndMainWindow;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = "json";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if(GetSaveFileNameA(&ofn)) {
        outFilePath = ofn.lpstrFile;
        return true;
    }
    return false;
}

bool LevelEditor::ShowOpenFileDialog(std::string& outFilePath) {
    OPENFILENAMEA ofn;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hwndMainWindow;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "JSON Files\0*.json\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileNameA(&ofn)) {
        outFilePath = ofn.lpstrFile;
        return true;
    }
    return false;
}

void LevelEditor::CheckInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(m_hwndMainWindow, &mousePos);

    m_buttonCancel.ResetHoverState();
    m_buttonEdit.ResetHoverState();
    m_buttonSave.ResetHoverState();

    m_buttonCancel.hovered = m_buttonCancel.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonEdit.hovered = m_buttonEdit.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonSave.hovered = m_buttonSave.IsMouseOver(mousePos.x, mousePos.y);

    int gridX = mousePos.x / 48;
    int gridY = mousePos.y / 48;

    if(IsKeyPressed(VK_LBUTTON)) {
        if(!m_mousePressed) {
            SetTile(gridX, gridY, m_selectedTile);
            if(m_buttonSave.IsMouseOver(mousePos.x, mousePos.y)) {
                std::string filePath;
                if(ShowSaveFileDialog(filePath)) {
                    if(SaveToFile(filePath)) {
                        MessageBox(NULL, L"Level saved successfully!", L"Success", MB_OK);
                        shouldExitToMainMenu = true;
                    }
                }
            } else if(m_buttonEdit.IsMouseOver(mousePos.x, mousePos.y)) {
                std::string filePath;
                if(ShowOpenFileDialog(filePath)) {
                    LoadFromFile(filePath);
                }
            } else if(m_buttonCancel.IsMouseOver(mousePos.x, mousePos.y)) {
                    shouldExitToMainMenu = true;
            }
        }
    } else if(IsKeyPressed(VK_RBUTTON)) {
        if(!m_mousePressed) {
            SetTile(gridX, gridY, TileType::NONE);
        }
    } else {
        m_mousePressed = false;
    }

    // Handle keyboard shortcuts
    if(IsKeyPressed('0')) m_selectedTile = TileType::NONE;
    if(IsKeyPressed('1')) m_selectedTile = TileType::TILE;
    if(IsKeyPressed('2')) m_selectedTile = TileType::MAHEX_START_POINT;
    if(IsKeyPressed('3')) m_selectedTile = TileType::MAHEX_END_POINT;
    if(IsKeyPressed('4')) m_selectedTile = TileType::HUSO_START_POINT;
    if(IsKeyPressed('5')) m_selectedTile = TileType::HUSO_END_POINT;
    if(IsKeyPressed('S')) {
        if(SaveToFile("customLevel.json")) {
            MessageBox(NULL, L"Level saved!", L"Info", MB_OK);
        }
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

    // Draw grid tiles
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

    // Render buttons
    m_buttonCancel.Render(hdcBuffer);
    m_buttonEdit.Render(hdcBuffer);
    m_buttonSave.Render(hdcBuffer);

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

    DeleteObject(tileBrush);
    DeleteObject(mahexStartBrush);
    DeleteObject(mahexEndBrush);
    DeleteObject(husoStartBrush);
    DeleteObject(husoEndBrush);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmBuffer);
    DeleteDC(hdcBuffer);
    ReleaseDC(m_hwndMainWindow, hdc);
}

void LevelEditor::Update() {
    CheckInput();
}