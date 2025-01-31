#pragma once

#include "Object.hpp"

struct Tile : Object {
	bool m_safe;
	bool m_collideable;

    Tile(int posX, int posY, HBITMAP bitmap, HBITMAP mask, bool safe, bool collideable)
        : m_safe(safe), m_collideable(collideable) {
        m_posX = posX;
        m_posY = posY;
        m_bitmap = bitmap;
        m_mask = mask;
    }
};

//namespace Tiles {
//	Tile LevelOne_RegularTile;
//}