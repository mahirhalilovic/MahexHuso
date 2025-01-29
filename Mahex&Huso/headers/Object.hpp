#pragma once

#include <Windows.h>

struct Object {
	int m_posX = 0;
	int m_posY = 0;

	HBITMAP m_bitmap;
	HBITMAP m_mask;
};