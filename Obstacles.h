#pragma once

#include "raylib.h"

class Obstacles
{
public:
	Obstacles(float posX, float posY, float width, float height);

	void Draw();

	Vector2 GetPos();
	float GetWidth();
	float GetHeight();
	
private:
	Vector2 position = { 100, 100 };
	float width = 200;
	float height = 300;
};

