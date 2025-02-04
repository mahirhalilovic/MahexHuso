#pragma once

#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include "headers/json.hpp"
#include "Button.hpp"

using json = nlohmann::json;

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

enum class TileType {
    NONE,
    TILE,
    MAHEX_START_POINT,
    MAHEX_END_POINT,
    HUSO_START_POINT,
    HUSO_END_POINT
};

class LevelEditor {
    public:
    LevelEditor() = default;
    LevelEditor(HWND);
    void SetTile(int, int, TileType);
    TileType GetTile(int, int) const;
    void Render();
    void Update();

    bool shouldExitToMainMenu = false;
    static LevelEditor* instance;

    private:
    HWND m_hwndMainWindow;
    int m_gridWidth, m_gridHeight;
    std::vector<std::vector<TileType>> m_grid;
    TileType m_selectedTile;

    POINT m_mahexStart{}, m_husoStart;
    POINT m_mahexEnd, m_husoEnd;

    Button m_buttonCancel, m_buttonEdit, m_buttonSave;
    bool m_mousePressed = true;

    std::string TileTypeToString(const TileType&);
    TileType StringToTileType(const std::string&);

    void CheckInput();
    bool SaveToFile(const std::string&);
    void LoadFromFile(const std::string&);
    bool ShowSaveFileDialog(std::string& outFilePath);
    bool ShowOpenFileDialog(std::string& outFilePath);
};