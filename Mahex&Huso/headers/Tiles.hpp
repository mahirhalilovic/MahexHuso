#pragma once

#include "Object.hpp"

enum class TileType {
    NONE,
    TILE,
    SPIKES,
    COIN,
    KEYDOWN_BLOCK,
    KEYDOWN_PLATE,
    PRESSURE_BLOCK,
    PRESSURE_PLATE_START,
    PRESSURE_PLATE_END,
    MAHEX_START,
    MAHEX_END,
    HUSO_START,
    HUSO_END
};

enum class Orientation {
    NONE,
    HORIZONTAL,
    VERTICAL
};

struct Tile : Object {
    TileType m_type = TileType::NONE;
    int m_id = -1;
    Orientation m_orientation = Orientation::NONE;
    int m_startPos = 0;
    int m_endPos = 0;
    bool m_active = false;
    int m_counter = 0;

    Tile() = default;

    Tile(int posX, int posY, int width, int height, TileType type, HBITMAP bitmap, HBITMAP mask)
        : m_type{type} {
        m_posX = posX;
        m_posY = posY;
        m_bitmap = bitmap;
        m_mask = mask;
        m_id = 0;
        m_startPos = 0;
        m_endPos = 0;
        m_active = false;
        m_counter = 0;
        m_orientation = Orientation::NONE;
    }
};

namespace Tiles {
    extern HBITMAP m_tileBitmap;
    extern HBITMAP m_tileMask;
    extern HBITMAP m_spikesBitmap;
    extern HBITMAP m_spikesMask;
    extern HBITMAP m_coinBitmap;
    extern HBITMAP m_coinMask;
    extern HBITMAP m_keydownBlockBitmap;
    extern HBITMAP m_keydownBlockMask;
    extern HBITMAP m_pressureBlockBitmap;
    extern HBITMAP m_pressureBlockMask;
    extern HBITMAP m_pressureBlockActiveBitmap;
    extern HBITMAP m_pressureBlockActiveMask;
    extern HBITMAP m_plateHorizontalBitmap;
    extern HBITMAP m_plateHorizontalMask;
    extern HBITMAP m_plateVerticalBitmap;
    extern HBITMAP m_plateVerticalMask;
    extern HBITMAP m_mahexEndBitmap;
    extern HBITMAP m_mahexEndMask;
    extern HBITMAP m_husoEndBitmap;
    extern HBITMAP m_husoEndMask;
    
    struct BitmapLoader;
    extern BitmapLoader loader;
}

//class MechanicalTile : Tile {
//    public:
//    unsigned int m_id = 0;
//
//    MechanicalTile() = default;
//
//    MechanicalTile(int posX, int posY, int width, int height, TileType type, HBITMAP bitmap, HBITMAP mask, unsigned int id)
//        : m_id{id} {
//        m_posX = posX;
//        m_posY = posY;
//        m_width = width;
//        m_height = height;
//        m_type = type;
//        m_bitmap = bitmap;
//        m_mask = mask;
//    }
//};
//
//class KeydownPlatform : MechanicalTile {
//    public:
//    Orientation m_orientation;
//
//    KeydownPlatform(int posX, int posY, int width, int height, TileType type, HBITMAP bitmap, HBITMAP mask, 
//        unsigned int id, Orientation orientation)
//        : m_orientation{orientation} {
//        m_posX = posX;
//        m_posY = posY;
//        m_width = width;
//        m_height = height;
//        m_type = type;
//        m_bitmap = bitmap;
//        m_mask = mask;
//        m_id = id;
//    }
//};
//
//class PressurePlatform : MechanicalTile {
//    public:
//    int m_endPos;
//    bool m_active;
//    Orientation m_orientation;
//
//    PressurePlatform(int posX, int posY, int width, int height, int endPos, TileType type, HBITMAP bitmap, HBITMAP mask,
//        unsigned int id, Orientation orientation)
//        : m_endPos{endPos}, m_orientation{orientation}, m_active{false} {
//        m_posX = posX;
//        m_posY = posY;
//        m_width = width;
//        m_height = height;
//        m_type = type;
//        m_bitmap = bitmap;
//        m_mask = mask;
//        m_id = id;
//    }
//};
//
//class Coin : Tile {
//    public:
//    int m_counter;
//
//    Coin(int posX, int posY, int width, int height, TileType type, HBITMAP bitmap, HBITMAP mask)
//        : m_counter{0} {
//        m_posX = posX;
//        m_posY = posY;
//        m_width = width;
//        m_height = height;
//        m_type = type;
//        m_bitmap = bitmap;
//        m_mask = mask;
//    }
//};