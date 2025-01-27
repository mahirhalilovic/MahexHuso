#pragma once

#include "Object.hpp"

struct Tile : Object {
	bool m_safe;
	bool m_collideable;
};

namespace Tiles {
	Tile LevelOne_RegularTile;
}