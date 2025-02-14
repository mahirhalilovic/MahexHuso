#include "headers/LevelEditor.hpp"

LevelEditor::LevelEditor(HWND hwnd) : m_hwnd{hwnd} {
    m_gridWidth = 25;
    m_gridHeight = 15;
    m_selectedTile = TileType::NONE;
    m_selectedOrientation = Orientation::HORIZONTAL;
    m_currentID = 0;
    m_grid.resize(m_gridWidth, std::vector<Tile>(m_gridHeight));

    m_mahexStart = m_mahexEnd = {-1, -1};
    m_husoStart = m_husoEnd = {-1, -1};

    m_selectedMenu = LevelEditorMenu::TILES;
    
    ConstructButtons();
}

void LevelEditor::ConstructButtons() {
    // Menu buttons
    m_buttonMenuTile = Button(1220, 20, 100, 50, L"TILES");
    m_buttonMenuMechanical = Button(1350, 20, 100, 50, L"MECHANICAL");
    m_buttonMenuPlayer = Button(1480, 20, 100, 50, L"PLAYER");

    // Tile buttons
    m_buttonTile = Button(1250, 120, 200, 50, L"TILE");
    m_buttonSpikes = Button(1250, 190, 200, 50, L"SPIKES");
    m_buttonCoin = Button(1250, 260, 200, 50, L"COIN");

    // Mechanical buttons
    m_buttonKeydownBlock = Button(1250, 120, 200, 50, L"KEYDOWN BLOCK");
    m_buttonPressureBlock = Button(1250, 190, 200, 50, L"PRESSURE BLOCK");
    m_buttonHorizontal = Button(1250, 300, 125, 50, L"HORIZONTAL");
    m_buttonVertical = Button(1425, 300, 125, 50, L"VERTICAL");

    // Player buttons
    m_buttonMahexStart = Button(1250, 120, 200, 50, L"MAHEX START");
    m_buttonMahexEnd = Button(1250, 190, 200, 50, L"MAHEX END");
    m_buttonHusoStart = Button(1250, 260, 200, 50, L"HUSO START");
    m_buttonHusoEnd = Button(1250, 330, 200, 50, L"HUSO END");

    // Footer buttons
    m_buttonCancel = Button(1220, 650, 100, 50, L"CANCEL");
    m_buttonEdit = Button(1350, 650, 100, 50, L"EDIT");
    m_buttonSave = Button(1480, 650, 100, 50, L"SAVE");

    m_buttonCancel.SetFont(L"i pixel u", 26, false);
    m_buttonEdit.SetFont(L"i pixel u", 26, false);
    m_buttonSave.SetFont(L"i pixel u", 26, false);
    m_buttonTile.SetFont(L"i pixel u", 26, false);
    m_buttonMahexStart.SetFont(L"i pixel u", 26, false);
    m_buttonMahexEnd.SetFont(L"i pixel u", 26, false);
    m_buttonHusoStart.SetFont(L"i pixel u", 26, false);
    m_buttonHusoEnd.SetFont(L"i pixel u", 26, false);
}

void LevelEditor::RenderButtons(const HDC &hdc) {
    m_buttonMenuTile.Render(hdc);
    m_buttonMenuMechanical.Render(hdc);
    m_buttonMenuPlayer.Render(hdc);

    switch(m_selectedMenu) {
        case LevelEditorMenu::TILES:
            m_buttonTile.Render(hdc);
            m_buttonSpikes.Render(hdc);
            m_buttonCoin.Render(hdc);
            break;

        case LevelEditorMenu::MECHANICAL:
            m_buttonKeydownBlock.Render(hdc);
            m_buttonPressureBlock.Render(hdc);
            m_buttonVertical.Render(hdc);
            m_buttonHorizontal.Render(hdc);
            break;

        case LevelEditorMenu::PLAYER:
            m_buttonMahexStart.Render(hdc);
            m_buttonMahexEnd.Render(hdc);
            m_buttonHusoStart.Render(hdc);
            m_buttonHusoEnd.Render(hdc);
            break;
    }

    m_buttonCancel.Render(hdc);
    m_buttonEdit.Render(hdc);
    m_buttonSave.Render(hdc);
}

void LevelEditor::RenderTiling(const HDC &hdc, const HDC &hdcBuffer) {
    HDC hdcMem = CreateCompatibleDC(hdc);

    switch(m_selectedMenu) {
        case LevelEditorMenu::TILES:
            SelectObject(hdcMem, Tiles::m_tileMask);
            BitBlt(hdcBuffer, 1501, 121, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_tileBitmap);
            BitBlt(hdcBuffer, 1501, 121, 48, 48, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, Tiles::m_spikesBitmap);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_spikesMask);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, Tiles::m_coinBitmap);
            BitBlt(hdcBuffer, 1517, 277, 16, 16, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_coinMask);
            BitBlt(hdcBuffer, 1517, 277, 16, 16, hdcMem, 0, 0, SRCAND);
            break;

        case LevelEditorMenu::MECHANICAL:
            SelectObject(hdcMem, Tiles::m_keydownBlockBitmap);
            BitBlt(hdcBuffer, 1501, 121, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_keydownBlockMask);
            BitBlt(hdcBuffer, 1501, 121, 48, 48, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, Tiles::m_pressureBlockBitmap);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_pressureBlockMask);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCAND);
            break;

        case LevelEditorMenu::PLAYER:
            SelectObject(hdcMem, Tiles::m_mahexEndBitmap);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_mahexEndMask);
            BitBlt(hdcBuffer, 1501, 191, 48, 48, hdcMem, 0, 0, SRCAND);
            SelectObject(hdcMem, Tiles::m_husoEndBitmap);
            BitBlt(hdcBuffer, 1501, 331, 48, 48, hdcMem, 0, 0, SRCPAINT);
            SelectObject(hdcMem, Tiles::m_husoEndMask);
            BitBlt(hdcBuffer, 1501, 331, 48, 48, hdcMem, 0, 0, SRCAND);
            break;
    }

    DeleteDC(hdcMem);
}

void LevelEditor::RenderTiles(const HDC &hdc, const HDC &hdcBuffer) {
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBRUSH mahexStartBrush = CreateSolidBrush(0xff);
    HBRUSH husoStartBrush = CreateSolidBrush(0xff0000);

    for(int x = 0; x < m_gridWidth; x++) {
        for(int y = 0; y < m_gridHeight; y++) {
            TileType tileType = m_grid[x][y].m_type;
            if(tileType != TileType::NONE) {
                RECT rect = {
                    x * 48,
                    y * 48,
                    (x + 1) * 48,
                    (y + 1) * 48
                };
                RECT plate;

                HBRUSH brush = nullptr;

                switch(tileType) {
                    case TileType::TILE:
                        SelectObject(hdcMem, Tiles::m_tileBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_tileMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::SPIKES:
                        SelectObject(hdcMem, Tiles::m_spikesBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_spikesMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::COIN:
                        SelectObject(hdcMem, Tiles::m_coinBitmap);
                        BitBlt(hdcBuffer, rect.left + 16, rect.top + 16, 16, 16, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_coinMask);
                        BitBlt(hdcBuffer, rect.left + 16, rect.top + 16, 16, 16, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::KEYDOWN_BLOCK:
                        SelectObject(hdcMem, Tiles::m_keydownBlockBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_keydownBlockMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::KEYDOWN_PLATE:
                    case TileType::PRESSURE_PLATE_START:
                        if(m_grid[x][y].m_orientation == Orientation::HORIZONTAL) {
                            SelectObject(hdcMem, Tiles::m_plateHorizontalBitmap);
                            BitBlt(hdcBuffer, rect.left, rect.top + 16, 48, 16, hdcMem, 0, 0, SRCPAINT);
                            SelectObject(hdcMem, Tiles::m_plateHorizontalMask);
                            BitBlt(hdcBuffer, rect.left, rect.top + 16, 48, 16, hdcMem, 0, 0, SRCAND);
                        } else {
                            SelectObject(hdcMem, Tiles::m_plateVerticalBitmap);
                            BitBlt(hdcBuffer, rect.left + 16, rect.top, 16, 48, hdcMem, 0, 0, SRCPAINT);
                            SelectObject(hdcMem, Tiles::m_plateVerticalMask);
                            BitBlt(hdcBuffer, rect.left + 16, rect.top, 16, 48, hdcMem, 0, 0, SRCAND);
                        }
                        break;
                    case TileType::PRESSURE_BLOCK:
                        SelectObject(hdcMem, Tiles::m_pressureBlockBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_pressureBlockMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::PRESSURE_PLATE_END:
                        if(m_grid[x][y].m_orientation == Orientation::HORIZONTAL) {
                            rect.top = rect.top + 16;
                            rect.bottom = rect.bottom - 16;
                            brush = CreateSolidBrush(0xbf40bf);
                        } else {
                            rect.left = rect.left + 16;
                            rect.right = rect.right - 16;
                            brush = CreateSolidBrush(0xbf40bf);
                        }
                        break;
                    case TileType::MAHEX_START:
                        brush = mahexStartBrush;
                        break;
                    case TileType::MAHEX_END:
                        SelectObject(hdcMem, Tiles::m_mahexEndBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_mahexEndMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                    case TileType::HUSO_START:
                        brush = husoStartBrush;
                        break;
                    case TileType::HUSO_END:
                        SelectObject(hdcMem, Tiles::m_husoEndBitmap);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCPAINT);
                        SelectObject(hdcMem, Tiles::m_husoEndMask);
                        BitBlt(hdcBuffer, rect.left, rect.top, 48, 48, hdcMem, 0, 0, SRCAND);
                        break;
                }

                if(brush) {
                    FillRect(hdcBuffer, &rect, brush);
                    DeleteObject(brush);
                }
            }
        }
    }

    DeleteObject(mahexStartBrush);
    DeleteObject(husoStartBrush);
    DeleteDC(hdcMem);
}

void LevelEditor::CheckHoverStatus(const POINT &mousePos) {
    m_buttonMenuTile.ResetHoverState();
    m_buttonMenuMechanical.ResetHoverState();
    m_buttonMenuPlayer.ResetHoverState();

    m_buttonTile.ResetHoverState();
    m_buttonSpikes.ResetHoverState();
    m_buttonCoin.ResetHoverState();

    m_buttonKeydownBlock.ResetHoverState();
    m_buttonPressureBlock.ResetHoverState();
    m_buttonVertical.ResetHoverState();
    m_buttonHorizontal.ResetHoverState();

    m_buttonMahexStart.ResetHoverState();
    m_buttonMahexEnd.ResetHoverState();
    m_buttonHusoStart.ResetHoverState();
    m_buttonHusoEnd.ResetHoverState();

    m_buttonCancel.ResetHoverState();
    m_buttonEdit.ResetHoverState();
    m_buttonSave.ResetHoverState();

    m_buttonMenuTile.hovered = m_buttonMenuTile.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonMenuMechanical.hovered = m_buttonMenuMechanical.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonMenuPlayer.hovered = m_buttonMenuPlayer.IsMouseOver(mousePos.x, mousePos.y);

    m_buttonTile.hovered = m_buttonTile.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonSpikes.hovered = m_buttonSpikes.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonCoin.hovered = m_buttonCoin.IsMouseOver(mousePos.x, mousePos.y);

    m_buttonKeydownBlock.hovered = m_buttonKeydownBlock.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonPressureBlock.hovered = m_buttonPressureBlock.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonVertical.hovered = m_buttonVertical.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonHorizontal.hovered = m_buttonHorizontal.IsMouseOver(mousePos.x, mousePos.y);

    m_buttonMahexStart.hovered = m_buttonMahexStart.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonMahexEnd.hovered = m_buttonMahexEnd.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonHusoStart.hovered = m_buttonHusoStart.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonHusoEnd.hovered = m_buttonHusoEnd.IsMouseOver(mousePos.x, mousePos.y);

    m_buttonCancel.hovered = m_buttonCancel.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonEdit.hovered = m_buttonEdit.IsMouseOver(mousePos.x, mousePos.y);
    m_buttonSave.hovered = m_buttonSave.IsMouseOver(mousePos.x, mousePos.y);
}

void LevelEditor::CheckMousePress(const POINT &mousePos) {
    if(m_buttonSave.IsMouseOver(mousePos.x, mousePos.y)) {
        std::string filePath;
        if(ShowSaveFileDialog(filePath)) {
            if(SaveToFile(filePath)) {
                MessageBox(NULL, L"Level saved successfully!", L"Success", MB_OK);
                shouldExitToMainMenu = true;
                return;
            }
        }
    } else if(m_buttonEdit.IsMouseOver(mousePos.x, mousePos.y)) {
        std::string filePath;
        if(ShowOpenFileDialog(filePath)) {
            LoadFromFile(filePath);
            return;
        }
    } else if(m_buttonCancel.IsMouseOver(mousePos.x, mousePos.y)) {
        shouldExitToMainMenu = true;
        return;
    } else if(m_buttonMenuTile.IsMouseOver(mousePos.x, mousePos.y)) {
        RemoveNonFinishedMechanicalTile();
        m_selectedMenu = LevelEditorMenu::TILES;
    } else if(m_buttonMenuMechanical.IsMouseOver(mousePos.x, mousePos.y)) {
        RemoveNonFinishedMechanicalTile();
        m_selectedMenu = LevelEditorMenu::MECHANICAL;
    } else if(m_buttonMenuPlayer.IsMouseOver(mousePos.x, mousePos.y)) {
        RemoveNonFinishedMechanicalTile();
        m_selectedMenu = LevelEditorMenu::PLAYER;
    }

    switch(m_selectedMenu) {
        case LevelEditorMenu::TILES:
            if(m_buttonTile.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::TILE;
            } else if(m_buttonSpikes.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::SPIKES;
            } else if(m_buttonCoin.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::COIN;
            }
            break;

        case LevelEditorMenu::MECHANICAL:
            if(m_buttonKeydownBlock.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::KEYDOWN_BLOCK;
            } else if(m_buttonPressureBlock.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::PRESSURE_BLOCK;
            } else if(m_buttonHorizontal.IsMouseOver(mousePos.x, mousePos.y)) {
                m_selectedOrientation = Orientation::HORIZONTAL;
            } else if(m_buttonVertical.IsMouseOver(mousePos.x, mousePos.y)) {
                m_selectedOrientation = Orientation::VERTICAL;
            }
            break;

        case LevelEditorMenu::PLAYER:
            if(m_buttonMahexStart.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::MAHEX_START;
            } else if(m_buttonMahexEnd.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::MAHEX_END;
            } else if(m_buttonHusoStart.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::HUSO_START;
            } else if(m_buttonHusoEnd.IsMouseOver(mousePos.x, mousePos.y)) {
                RemoveNonFinishedMechanicalTile();
                m_selectedTile = TileType::HUSO_END;
            }
            break;
    }
}

void LevelEditor::RemoveNonFinishedMechanicalTile() {
    for(int i = 0; i < m_grid.size(); ++i) {
        for(int j = 0; j < m_grid[i].size(); ++j) {
            if(m_grid[i][j].m_id == m_currentID) {
                TileType type = m_grid[i][j].m_type;
                if(type == TileType::KEYDOWN_BLOCK || type == TileType::KEYDOWN_PLATE ||
                    type == TileType::PRESSURE_BLOCK || type == TileType::PRESSURE_PLATE_START || type == TileType::PRESSURE_PLATE_END) {
                    m_grid[i][j].m_type = TileType::NONE;
                }
            }
        }
    }
    m_mechanicalTileInProgress = false;
}

void LevelEditor::SetTile(int x, int y, TileType tileType) {
    TileType currentTile = GetTile(x, y).m_type;

    if(x >= 0 && x < m_gridWidth && y >= 0 && y < m_gridHeight) {
        switch(currentTile) {
            case TileType::MAHEX_START:
                m_mahexStart = {-1, -1};
                break;
            case TileType::HUSO_START:
                m_husoStart = {-1, -1};
                break;
            case TileType::KEYDOWN_BLOCK:
            case TileType::KEYDOWN_PLATE:
            case TileType::PRESSURE_BLOCK:
            case TileType::PRESSURE_PLATE_START:
            case TileType::PRESSURE_PLATE_END:
                if(tileType == currentTile && GetTile(x, y).m_id == m_currentID) break;

                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_id == GetTile(x, y).m_id)
                            m_grid[i][j].m_type = TileType::NONE;
                    }
                }
                break;
        }

        switch(tileType) {
            case TileType::MAHEX_START:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == TileType::MAHEX_START) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_mahexStart = {x * 48 + 8, y * 48 + 16};
                break;
            case TileType::MAHEX_END:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == TileType::MAHEX_END) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_mahexEnd = {x * 48 + 8, y * 48 + 16};
                break;
            case TileType::HUSO_START:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == TileType::HUSO_START) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_husoStart = {x * 48 + 8, y * 48 + 16};
                break;
            case TileType::HUSO_END:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == TileType::HUSO_END) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_husoEnd = {x * 48 + 8, y * 48 + 16};
                break;
            case TileType::KEYDOWN_BLOCK:
            case TileType::PRESSURE_BLOCK:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == tileType && m_grid[i][j].m_id == m_currentID) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_grid[x][y].m_id = m_currentID;
                m_blockPlaced = true;
                break;
            case TileType::KEYDOWN_PLATE:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == tileType && m_grid[i][j].m_id == m_currentID) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_grid[x][y].m_orientation = m_selectedOrientation;
                m_grid[x][y].m_id = m_currentID;
                m_blockPlaced = true;
                break;
            case TileType::PRESSURE_PLATE_START:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == tileType && m_grid[i][j].m_id == m_currentID) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_grid[x][y].m_orientation = m_selectedOrientation;
                m_grid[x][y].m_id = m_currentID;
                m_pressurePlateStartX = x;
                m_pressurePlateStartY = y;
                m_blockPlaced = true;
                break;

            case TileType::PRESSURE_PLATE_END:
                for(int i = 0; i < m_grid.size(); ++i) {
                    for(int j = 0; j < m_grid[i].size(); ++j) {
                        if(m_grid[i][j].m_type == tileType && m_grid[i][j].m_id == m_currentID) {
                            m_grid[i][j].m_type = TileType::NONE;
                        }
                    }
                }
                m_grid[x][y].m_id = m_currentID;
                m_grid[x][y].m_orientation = m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_orientation;
                if(m_grid[x][y].m_posX == m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_posX) {
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_startPos = m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_posY;
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_endPos = m_grid[x][y].m_posY;
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_movement = Orientation::VERTICAL;
                } else if(m_grid[x][y].m_posY == m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_posY) {
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_startPos = m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_posX;
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_endPos = m_grid[x][y].m_posX;
                    m_grid[m_pressurePlateStartX][m_pressurePlateStartY].m_movement = Orientation::HORIZONTAL;
                } else {
                    m_platesNotAligned = true;
                }
                m_blockPlaced = true;
                break;
        }

        m_grid[x][y].m_type = tileType;
    }
}

Tile LevelEditor::GetTile(int x, int y) const {
    if(x >= 0 && x < m_gridWidth && y >= 0 && y < m_gridHeight) {
        return m_grid[x][y];
    }

    return Tile(0, 0, 0, 0, TileType::NONE, NULL, NULL);
}

std::string LevelEditor::TileTypeToString(const TileType &tileType) {
    switch(tileType) {
        case TileType::TILE:
            return "TILE";
        case TileType::SPIKES:
            return "SPIKES";
        case TileType::COIN:
            return "COIN";
        case TileType::KEYDOWN_BLOCK:
            return "KEYDOWN_BLOCK";
        case TileType::KEYDOWN_PLATE:
            return "KEYDOWN_PLATE";
        case TileType::PRESSURE_BLOCK:
            return "PRESSURE_BLOCK";
        case TileType::PRESSURE_PLATE_START:
            return "PRESSURE_PLATE_START";
        case TileType::PRESSURE_PLATE_END:
            return "PRESSURE_PLATE_END";
        case TileType::MAHEX_START:
            return "MAHEX_START";
        case TileType::MAHEX_END:
            return "MAHEX_END";
        case TileType::HUSO_START:
            return "HUSO_START";
        case TileType::HUSO_END:
            return "HUSO_END";
    }

    return "NONE";
}

TileType LevelEditor::StringToTileType(const std::string &str) {     
    if(str == "TILE") return TileType::TILE;
    else if(str == "SPIKES") return TileType::SPIKES;
    else if(str == "COIN") return TileType::COIN;
    else if(str == "KEYDOWN_BLOCK") return TileType::KEYDOWN_BLOCK;
    else if(str == "KEYDOWN_PLATE") return TileType::KEYDOWN_PLATE;
    else if(str == "PRESSURE_BLOCK") return TileType::PRESSURE_BLOCK;
    else if(str == "PRESSURE_PLATE_START") return TileType::PRESSURE_PLATE_START;
    else if(str == "PRESSURE_PLATE_END") return TileType::PRESSURE_PLATE_END;
    else if(str == "MAHEX_START") return TileType::MAHEX_START;
    else if(str == "MAHEX_END") return TileType::MAHEX_END;
    else if(str == "HUSO_START") return TileType::HUSO_START;
    else if(str == "HUSO_END") return TileType::HUSO_END;
    else return TileType::NONE;
}

std::string LevelEditor::OrientationToString(const Orientation &orientation) {
    switch(orientation) {
        case Orientation::HORIZONTAL:
            return "HORIZONTAL";
        case Orientation::VERTICAL:
            return "VERTICAL";
    }

    return "NONE";
}

Orientation LevelEditor::StringToOrientation(const std::string &str) {
    if(str == "HORIZONTAL") return Orientation::HORIZONTAL;
    else if(str == "VERTICAL") return Orientation::VERTICAL;
    else return Orientation::NONE;
}

bool LevelEditor::SaveToFile(const std::string& filename) {
    try {
        json levelData;
        levelData["tiles"] = json::array();

        for(int x = 0; x < m_gridWidth; x++) {
            for(int y = 0; y < m_gridHeight; y++) {
                if(m_grid[x][y].m_type != TileType::NONE && m_grid[x][y].m_type != TileType::PRESSURE_PLATE_END) {
                    json tileData = {
                        {"x", x * 48},
                        {"y", y * 48},
                        {"type", TileTypeToString(m_grid[x][y].m_type)},
                    };

                    switch(m_grid[x][y].m_type) {
                        case TileType::KEYDOWN_BLOCK:
                        case TileType::PRESSURE_BLOCK:
                            tileData.push_back({"id", m_grid[x][y].m_id});
                            break;
                        case TileType::KEYDOWN_PLATE:
                            tileData.push_back({"id", m_grid[x][y].m_id});
                            tileData.push_back({"orientation", OrientationToString(m_grid[x][y].m_orientation)});
                            break;
                        case TileType::PRESSURE_PLATE_START:
                            tileData.push_back({"id", m_grid[x][y].m_id});
                            tileData.push_back({"orientation", OrientationToString(m_grid[x][y].m_orientation)});
                            if(m_grid[x][y].m_movement == Orientation::VERTICAL) {
                                tileData.push_back({"startPos", y * 48});
                            } else {
                                tileData.push_back({"startPos", x * 48});
                            }
                            tileData.push_back({"endPos", m_grid[x][y].m_endPos});
                            tileData.push_back({"movement", OrientationToString(m_grid[x][y].m_movement)});
                            break;
                    }

                    levelData["tiles"].push_back(tileData);
                }
            }
        }

        if(m_mahexStart.x == -1 || m_mahexEnd.x == -1 || m_husoStart.x == -1 || m_husoEnd.x == -1) {
            MessageBox(m_hwnd, L"Start and end positions must be defined!", L"Error", MB_OK);
            return false;
        }

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
    m_grid.resize(m_gridWidth, std::vector<Tile>(m_gridHeight));

    for(const auto& tileData : levelData["tiles"]) {
        int x = tileData["x"] / 48;
        int y = tileData["y"] / 48;
        TileType type = StringToTileType(tileData["type"]);

        m_grid[x][y].m_posX = tileData["x"];
        m_grid[x][y].m_posY = tileData["y"];
        m_grid[x][y].m_type = type;

        switch(type) {
            case TileType::MAHEX_START:
                m_mahexStart.x = tileData["x"];
                m_mahexStart.y = tileData["y"];
                break;
            case TileType::MAHEX_END:
                m_mahexEnd.x = tileData["x"];
                m_mahexEnd.y = tileData["y"];
                break;
            case TileType::HUSO_START:
                m_husoStart.x = tileData["x"];
                m_husoStart.y = tileData["y"];
                break;
            case TileType::HUSO_END:
                m_husoEnd.x = tileData["x"];
                m_husoEnd.y = tileData["y"];
                break;
            case TileType::KEYDOWN_BLOCK:
            case TileType::PRESSURE_BLOCK:
                m_grid[x][y].m_id = tileData["id"];
                if(m_currentID < m_grid[x][y].m_id) m_currentID = m_grid[x][y].m_id;
                break;
            case TileType::KEYDOWN_PLATE:
                m_grid[x][y].m_id = tileData["id"];
                if(m_currentID < m_grid[x][y].m_id) m_currentID = m_grid[x][y].m_id;
                m_grid[x][y].m_orientation = StringToOrientation(tileData["orientation"]);
                break;
            case TileType::PRESSURE_PLATE_START:
                m_grid[x][y].m_id = tileData["id"];
                if(m_currentID < m_grid[x][y].m_id) m_currentID = m_grid[x][y].m_id;
                m_grid[x][y].m_orientation = StringToOrientation(tileData["orientation"]);
                m_grid[x][y].m_movement = StringToOrientation(tileData["movement"]);
                m_grid[x][y].m_endPos = tileData["endPos"];
                if(m_grid[x][y].m_movement == Orientation::VERTICAL) {
                    int endPosY = m_grid[x][y].m_endPos / 48;
                    m_grid[x][endPosY].m_posX = x * 48;
                    m_grid[x][endPosY].m_posY = endPosY * 48;
                    m_grid[x][endPosY].m_type = TileType::PRESSURE_PLATE_END;
                    m_grid[x][endPosY].m_id = m_grid[x][y].m_id;
                    m_grid[x][endPosY].m_orientation = m_grid[x][y].m_orientation;
                } else {
                    int endPosX = m_grid[x][y].m_endPos / 48;
                    m_grid[endPosX][y].m_posX = endPosX * 48;
                    m_grid[endPosX][y].m_posY = y * 48;
                    m_grid[endPosX][y].m_type = TileType::PRESSURE_PLATE_END;
                    m_grid[endPosX][y].m_id = m_grid[x][y].m_id;
                    m_grid[endPosX][y].m_orientation = m_grid[x][y].m_orientation;
                }
                break;
        }
    }

    ++m_currentID;
}

bool LevelEditor::ShowSaveFileDialog(std::string& outFilePath) {
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
        outFilePath = ofn.lpstrFile;
        return true;
    }
    return false;
}

void LevelEditor::CheckInput() {
    POINT mousePos;
    GetCursorPos(&mousePos);
    ScreenToClient(m_hwnd, &mousePos);

    CheckHoverStatus(mousePos);

    int gridX = mousePos.x / 48;
    int gridY = mousePos.y / 48;

    if(IsKeyPressed(VK_LBUTTON)) {
        SetTile(gridX, gridY, m_selectedTile);
        CheckMousePress(mousePos);
    } else if(IsKeyPressed(VK_RBUTTON)) {
        SetTile(gridX, gridY, TileType::NONE);
    }

    if(IsKeyPressed(VK_ESCAPE)) {
        if(!m_escapePressed) {
            m_escapePressed = true;
            if(m_mechanicalTileInProgress) {
                RemoveNonFinishedMechanicalTile();
            } else if(m_selectedTile != TileType::NONE) {
                m_selectedTile = TileType::NONE;
            } else if(MessageBox(m_hwnd, L"Do you want to exit level editor?", L"Exit?", MB_OKCANCEL) == IDOK) shouldExitToMainMenu = true;
        }
    } else {
        m_escapePressed = false;
    }

    if(IsKeyPressed(VK_SPACE)) {
        if(!m_spacePressed) {
            m_spacePressed = true;
            if(m_selectedTile == TileType::KEYDOWN_BLOCK) {
                if(!m_blockPlaced) return;
                m_selectedTile = TileType::KEYDOWN_PLATE;
                m_mechanicalTileInProgress = true;
                m_blockPlaced = false;
            } else if(m_selectedTile == TileType::KEYDOWN_PLATE) {
                if(!m_blockPlaced) return;
                m_selectedTile = TileType::KEYDOWN_BLOCK;
                m_mechanicalTileInProgress = false;
                m_blockPlaced = false;
                ++m_currentID;
            } else if(m_selectedTile == TileType::PRESSURE_BLOCK) {
                if(!m_blockPlaced) return;
                m_selectedTile = TileType::PRESSURE_PLATE_START;
                m_mechanicalTileInProgress = true;
                m_blockPlaced = false;
            } else if(m_selectedTile == TileType::PRESSURE_PLATE_START) {
                if(!m_blockPlaced) return;
                m_selectedTile = TileType::PRESSURE_PLATE_END;
                m_blockPlaced = false;
            } else if(m_selectedTile == TileType::PRESSURE_PLATE_END) {
                if(!m_blockPlaced) return;
                m_selectedTile = TileType::PRESSURE_BLOCK;
                m_mechanicalTileInProgress = false;
                m_blockPlaced = false;
                if(m_platesNotAligned) RemoveNonFinishedMechanicalTile();
                ++m_currentID;
            }
        }
    } else {
        m_spacePressed = false;
    }
}

void LevelEditor::Render() {
    HDC hdc = GetDC(m_hwnd);
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);
    RECT clientRect;
    GetClientRect(m_hwnd, &clientRect);
    HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP hbmOldBuffer = (HBITMAP) SelectObject(hdcBuffer, hbmBuffer);

    HBRUSH backgroundBrush = CreateSolidBrush(0xe6d8ad);
    HPEN pen = CreatePen(PS_SOLID, 1, 0x0);

    HBRUSH mahexStartBrush = CreateSolidBrush(0xff);
    HBRUSH husoStartBrush = CreateSolidBrush(0xff0000);

    SelectObject(hdcBuffer, pen);
    SelectObject(hdcBuffer, backgroundBrush);

    for(int i = 0; i < 15; ++i) {
        for(int j = 0; j < 25; ++j) {
            Rectangle(hdcBuffer, j * 48, i * 48, (j + 1) * 48, (i + 1) * 48);
        }
    }

    DeleteObject(backgroundBrush);
    DeleteObject(pen);
    backgroundBrush = CreateSolidBrush(0xccb366);
    pen = CreatePen(PS_SOLID, 2, 0xb3922d);

    SelectObject(hdcBuffer, backgroundBrush);
    Rectangle(hdcBuffer, 1200, 0, 1600, 720);

    if(m_selectedMenu == LevelEditorMenu::PLAYER) {
        SelectObject(hdcBuffer, pen);
        SelectObject(hdcBuffer, mahexStartBrush);
        Rectangle(hdcBuffer, 1501, 121, 1549, 169);
        SelectObject(hdcBuffer, husoStartBrush);
        Rectangle(hdcBuffer, 1501, 261, 1549, 309);
    }

    DeleteObject(backgroundBrush);
    DeleteObject(pen);

    RenderButtons(hdcBuffer);
    RenderTiling(hdc, hdcBuffer);
    RenderTiles(hdc, hdcBuffer);

    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcBuffer, 0, 0, SRCCOPY);

    DeleteObject(mahexStartBrush);
    DeleteObject(husoStartBrush);
    SelectObject(hdcBuffer, hbmOldBuffer);
    DeleteObject(hbmBuffer);
    DeleteDC(hdcBuffer);
    DeleteDC(hdcMem);
    ReleaseDC(m_hwnd, hdc);
}

void LevelEditor::Update() {
    CheckInput();
}