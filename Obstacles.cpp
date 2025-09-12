#include "Obstacles.h"

Obstacles::Obstacles(float posX, float posY, float width, float height)
{
	position.x = posX;
	position.y = posY;

	this->width = width;
	this->height = height;
}

void Obstacles::Draw()
{
	DrawRectanglePro({ position.x, position.y, width, height }, { width / 2, height / 2 }, 0, WHITE);
}

Vector2 Obstacles::GetPos()
{
	return position;
}

float Obstacles::GetWidth()
{
	return width;
}

float Obstacles::GetHeight()
{
	return height;
}
