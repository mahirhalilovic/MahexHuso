#pragma once

#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include "headers/json.hpp"
#include "Button.hpp"
#include "Tiles.hpp"

using json = nlohmann::json;

#define IsKeyPressed(x) (GetAsyncKeyState(x) & 0x8000 ? 1 : 0)

enum class LevelEditorMenu {
    TILES,
    MECHANICAL,
    PLAYER
};

class LevelEditor {
    public:
    LevelEditor() = default;
    LevelEditor(HWND);
    void Render();
    void Update();

    std::string TileTypeToString(const TileType&);
    TileType StringToTileType(const std::string&);
    std::string OrientationToString(const Orientation&);
    Orientation StringToOrientation(const std::string&);

    bool shouldExitToMainMenu = false;

    private:
    HWND m_hwnd;
    int m_gridWidth, m_gridHeight;
    std::vector<std::vector<Tile>> m_grid;
    TileType m_selectedTile;
    Orientation m_selectedOrientation;
    bool m_blockPlaced = false;
    bool m_platesNotAligned = false;

    POINT m_mahexStart, m_husoStart;
    POINT m_mahexEnd, m_husoEnd;

    LevelEditorMenu m_selectedMenu;

    Button m_buttonMenuTile, m_buttonMenuMechanical, m_buttonMenuPlayer;
    Button m_buttonTile, m_buttonSpikes, m_buttonCoin;
    Button m_buttonKeydownBlock, m_buttonPressureBlock, m_buttonVertical, m_buttonHorizontal;
    Button m_buttonMahexStart, m_buttonMahexEnd, m_buttonHusoStart, m_buttonHusoEnd;
    Button m_buttonCancel, m_buttonEdit, m_buttonSave;
    bool m_spacePressed = true, m_escapePressed = false, m_mechanicalTileInProgress;
    int m_pressurePlateStartX, m_pressurePlateStartY;
    int m_currentID;

    void ConstructButtons();
    void CheckHoverStatus(const POINT&);
    void RenderButtons(const HDC&);
    void RenderTiles(const HDC&, const HDC&);
    void RenderTiling(const HDC&, const HDC&);
    void CheckMousePress(const POINT&);
    void RemoveNonFinishedMechanicalTile();

    void SetTile(int, int, TileType);
    Tile GetTile(int, int) const;

    void CheckInput();
    bool SaveToFile(const std::string&);
    void LoadFromFile(const std::string&);
    bool ShowSaveFileDialog(std::string& outFilePath);
    bool ShowOpenFileDialog(std::string& outFilePath);
};