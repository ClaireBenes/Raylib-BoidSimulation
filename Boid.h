#pragma once

#include "raylib.h"
#include <vector>

class Boid
{
public:
	Boid(float id, float x, float y, float rotation);

	Vector2 Separates(std::vector<Boid*>& other);
	Vector2 Align(std::vector<Boid*>& other);
	Vector2 Group(std::vector<Boid*>& other);

	void Move(std::vector<Boid*>& other);
	void Draw();

	Vector2 Normalized(Vector2 vec2);
	Vector2 Distance(Boid* boid);
	float DistanceSqrt(const Vector2& a, const Vector2& b);

	//Vector2 AvoidObstacles(std::vector<Obstacles> obstacle);

private:
	float id = 0;
	float speed = 70;
	float rotation = 10;
	float size = 20;
	Vector2 position = { 10,10 };
	Vector2 velocity = { 10,5 };

	Rectangle fishImage{ position.x,position.y, 10, 10 };
};

