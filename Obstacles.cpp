#include "Obstacles.h"

Obstacles::Obstacles(float posX, float posY, float width, float height, bool useMouse)
{
	position.x = posX;
	position.y = posY;

	this->width = width;
	this->height = height;

	isMouse = useMouse;
}

void Obstacles::Draw()
{
	if( !isMouse )
	{
		//DrawRectanglePro({ position.x, position.y, width, height }, { width / 2, height / 2 }, 0, BLACK);
		//DrawRectangleLinesEx({ position.x - width / 2, position.y - height / 2, width, height }, 4, WHITE);

		DrawCircle(position.x, position.y, width, BLACK);

		DrawCircleLines(position.x, position.y, width, WHITE);
		DrawCircleLines(position.x, position.y, width + 1, WHITE);
	}
}

void Obstacles::Update()
{
	if( isMouse )
	{
		position.x = GetMouseX();
		position.y = GetMouseY() + 20;
	}
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
