#pragma once

#include <windows.h>

#include "Object.hpp"

enum Direction { DIRECTION_LEFT = 1, DIRECTION_RIGHT, IDLE };

struct Player : Object {
	float m_velX = 0;
	float m_velY = 0;
	bool m_grounded = true;

	int m_spriteCounter = 0;

	Direction m_direction;
};