#pragma once

#include "raylib.h"
#include <vector>

class Obstacles;
class Boid;

struct BoidGroup
{
	std::vector<Boid*> ourFishes {};

	BoidGroup* predatorGroup = nullptr;
	BoidGroup* preyGroup = nullptr;
};

class Boid
{
public:
	Boid(float id, float x, float y, float rotation, Texture texture, Color color);
	~Boid();

	Vector2 Separates(std::vector<Boid*>& other);
	Vector2 Align(std::vector<Boid*>& other);
	Vector2 Group(std::vector<Boid*>& other);

	Vector2 KeepWithinBorder();
	Vector2 AvoidObstacles(std::vector<Obstacles*>& obstacles);
	Vector2 AvoidBoid(std::vector<Boid*>& other);
	Vector2 ChaseBoid(std::vector<Boid*>& other);

	void Move(BoidGroup& group, std::vector<Obstacles*>& obstacles);
	void Draw();

	Vector2 Normalized(Vector2 vec2);
	Vector2 Distance(Boid* boid);

	float DistanceSqrt(const Vector2& a, const Vector2& b);
	float Clamp(float& angle);

	Vector2 GetPos();
	float GetSize();

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

