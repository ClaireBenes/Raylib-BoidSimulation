#include "Boid.h"

Boid::Boid(float id, float x, float y, float rotation)
{
	this->id = id;
	position.x = x;
	position.y = y;

	this->rotation = rotation;
}

Vector2 Boid::Separates(std::vector<Boid*>& other)
{
	float boidCount = 0;
	Vector2 alldistance = { 0,0 };
	Vector2 dist = { 0,0 };

	for (int i = 0; i < other.size(); i++) 
	{
		if (other[i]->id == id) {
			continue;
		}

		float distanceX = other[i]->position.x - position.x;
		float distanceY = other[i]->position.y - position.y;

		float distanceSqrt = distanceX * distanceX + distanceY * distanceY;

		if (distanceSqrt < 2500)
		{ 
			alldistance.y += distanceX * -100;
			alldistance.x += distanceY * -100;
			boidCount += 1;
		}
	}
	return Normalized(alldistance);
}

Vector2 Boid::Align(std::vector<Boid*>& others)
{
	return Vector2();
}

Vector2 Boid::Group(std::vector<Boid*>& other)
{
	return Vector2();
}

void Boid::Move(std::vector<Boid*>& other)
{
	if (position.x >= 1080 - size / 2 || position.x <= 0 + size / 2) {
		velocity.x = -velocity.x; 
	}
	if (position.y <= 0 + size / 2 || position.y >= 720 - size / 2) {
		velocity.y = -velocity.y;
	}

	velocity.x += Separates(other).x *5;
	velocity.y += Separates(other).y*5;
	Vector2 normVelocity = Normalized(velocity);
	//Vector2 normSeperate = Normalized(Separates(other));

	position.x += (speed * normVelocity.x) * GetFrameTime();
	position.y += (speed * normVelocity.y) * GetFrameTime();
}

void Boid::Draw()
{
	float origin = size / 2;
	DrawRectanglePro({ position.x,position.y, size, size }, { origin,origin }, rotation, WHITE);
}

Vector2 Boid::Normalized(Vector2 vec2)
{
	if (vec2.x == 0 && vec2.y == 0) return { 0,0 };
	float newX = vec2.x / sqrt(vec2.x * vec2.x + vec2.y * vec2.y);
	float newY = vec2.y / sqrt(vec2.x * vec2.x + vec2.y * vec2.y);

	return Vector2({ newX,newY });
}
