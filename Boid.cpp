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
	Vector2 alldistance = { 0,0 };

	for (int i = 0; i < other.size(); i++) 
	{
		if (other[i]->id == id) {
			continue;
		}

		Vector2 dist = Distance(other[i]);
		float distanceSqrt = sqrt(dist.x * dist.x + dist.y * dist.y);

		if (distanceSqrt < 40)
		{ 
			alldistance.x -= dist.x;
			alldistance.y -= dist.y;
		}
	}

	return alldistance;
}

Vector2 Boid::Align(std::vector<Boid*>& others)
{
	Vector2 overallVelocity = { 0, 0 };
	int count = 0;

	for (int i = 0; i < others.size(); i++)
	{
		if ((others[i]->id == id)) {
			continue;
		}

		if ((DistanceSqrt(this->position, others[i]->position) > 200)) {
			break;
		}

		overallVelocity.x += others[i]->velocity.x;
		overallVelocity.y += others[i]->velocity.y;

		count += 1;
	}

	overallVelocity.x = overallVelocity.x / count;
	overallVelocity.y = overallVelocity.y / count;

	overallVelocity.x = (overallVelocity.x - velocity.x) / 8;
	overallVelocity.y = (overallVelocity.y - velocity.y) / 8;

	return count > 0 ? overallVelocity : Vector2{0, 0};
}

Vector2 Boid::Group(std::vector<Boid*>& other)
{
	return Vector2();
}

void Boid::Move(std::vector<Boid*>& other)
{
	if ( position.x < 0 + 100) {
		velocity.x = 10; 
	}
	else if (position.x > 1080 - 100) {
		velocity.x = -10;
	}
	if ( position.y > 720 - 100) {
		velocity.y = -10;
	}
	else if (position.y < 0 + 100) {
		velocity.y = 10;
	}

	velocity.x += Align(other).x + Separates(other).x;
	velocity.y += Align(other).y + Separates(other).y;
	Vector2 normVelocity = Normalized(velocity);

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

Vector2 Boid::Distance(Boid* boid)
{
	float distanceX = boid->position.x - position.x;
	float distanceY = boid->position.y - position.y;

	return Vector2({ distanceX,distanceY });
}

float Boid::DistanceSqrt(const Vector2& a, const Vector2& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;

	return sqrt(dx * dx + dy * dy);
}
