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
    int m_width;
    int m_height;
    int m_id;
    Orientation m_orientation;
    int m_endPos;
    int m_active;
    int m_counter;

    Tile() = default;

    Tile(int posX, int posY, int width, int height, TileType type, HBITMAP bitmap, HBITMAP mask)
        : m_width{width}, m_height{height}, m_type{type} {
        m_posX = posX;
        m_posY = posY;
        m_bitmap = bitmap;
        m_mask = mask;
        m_id = 0;
        m_endPos = 0;
        m_active = false;
        m_counter = 0;
        m_orientation = Orientation::NONE;
    }
};

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