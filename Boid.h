#pragma once

#include "raylib.h"
#include <vector>

class Obstacles;

class Boid
{
public:
	Boid(float id, float posX, float poY, float rotation, Texture texture, Color color);
	~Boid();

	Vector2 Separates(std::vector<Boid*>& other);
	Vector2 Align(std::vector<Boid*>& other);
	Vector2 Group(std::vector<Boid*>& other);

	Vector2 KeepWithinBorder();
	Vector2 AvoidObstacles(std::vector<Obstacles*>& obstacles);

	void Move(std::vector<Boid*>& other, std::vector<Obstacles*>& obstacles);
	void Draw();

	Vector2 Normalized(Vector2 vec2);
	Vector2 Distance(Boid* boid);

	float DistanceSqrt(const Vector2& a, const Vector2& b);
	float Clamp(float& angle);

private:
	//Fish Info
	float id = 0;
	float speed = 250;
	float rotation = 10;
	float size = 25;
	Vector2 position = { 10,10 };
	Vector2 velocity = { 0,0 };

	Color color = Color{ (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), 255 };
	Texture fishTexture;

	//Boid Info
	float maxDistance = 40;
	float maxSteer = 20;
};

