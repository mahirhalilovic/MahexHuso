#pragma once

#include <windows.h>

#include "Object.hpp"

enum Direction { IDLE = 0, DIRECTION_LEFT, DIRECTION_RIGHT };

struct Player : Object {
	float m_velX;
	float m_velY;
	bool m_grounded;

	int m_spriteCounter;
	Direction m_direction;

	Player() = default;

	Player(int posX, int posY, HBITMAP bitmap, HBITMAP mask,
		float velX, float velY, bool grounded, int spriteCounter, Direction direction)
		: m_velX{velX}, m_velY{velY}, m_grounded{grounded}, m_spriteCounter{spriteCounter}, m_direction{direction} {
		m_posX = posX;
		m_posY = posY;
		m_bitmap = bitmap;
		m_mask = mask;
	}
};