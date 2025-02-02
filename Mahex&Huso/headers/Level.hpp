#pragma once

#include <windows.h>
#include <vector>

#include "Object.hpp"
#include "Tiles.hpp"

struct Level {
	std::vector<Tile> m_tiles;
	POINT m_startingPoint;

	Level() = default;
};