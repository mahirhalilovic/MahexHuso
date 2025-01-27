#include "Game.hpp"

Game::Game(HWND hwnd) : m_hwnd{hwnd} {}

void Game::Update() {

}

void Game::Render() {

}

void Game::CheckInput() {
	Mahex.m_direction = IDLE;

	if(IsKeyPressed(VK_LEFT)) {
		Mahex.m_direction = DIRECTION_LEFT;
		Mahex.m_velX = -MOVE_SPEED;

	} else if(IsKeyPressed(VK_RIGHT)) {
		Mahex.m_direction = DIRECTION_RIGHT;
		Mahex.m_velX = MOVE_SPEED;
	}

	if(IsKeyPressed(VK_UP)) {
		Mahex.m_velY = JUMP_HEIGHT;
	}
}