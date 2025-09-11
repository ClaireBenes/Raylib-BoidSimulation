#include "Boid.h"

Boid::Boid(float id, float x, float y, float rotation, Texture texture)
{
	velocity.x = speed * cos(rotation);
	velocity.y = speed * sin(rotation);

	this->id = id;
	position.x = x;
	position.y = y;

	this->rotation = rotation;

	fishTexture = texture;
	fishTexture.width = size;
	fishTexture.height = size;
}

Boid::~Boid()
{
	UnloadTexture(fishTexture);
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

		if (distanceSqrt < 50)
		{ 
			alldistance.x -= dist.x;
			alldistance.y -= dist.y;
		}
	}

	if ((alldistance.x * alldistance.x + alldistance.y * alldistance.y) > FLT_EPSILON) {
		Normalized(alldistance);
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

		overallVelocity.x += others[i]->velocity.x;
		overallVelocity.y += others[i]->velocity.y;

		count += 1;
	}

	overallVelocity.x = overallVelocity.x / count;
	overallVelocity.y = overallVelocity.y / count;

	overallVelocity.x = (overallVelocity.x - velocity.x) / 8;
	overallVelocity.y = (overallVelocity.y - velocity.y) / 8;

	return overallVelocity;
}

Vector2 Boid::Group(std::vector<Boid*>& other)
{
	Vector2 overallPos = { 0, 0 };
	int count = 0;

	for (int i = 0; i < other.size(); i++) 
	{
		if ((other[i]->id == id)) {
			continue;
		}

		overallPos.x += other[i]->position.x;
		overallPos.y += other[i]->position.y;

		count += 1;
	}

	overallPos.x = overallPos.x / count;
	overallPos.y = overallPos.y / count;

	overallPos.x = (overallPos.x - position.x) / 100;
	overallPos.y = (overallPos.y - position.y) / 100;

	return overallPos;
}

Vector2 Boid::KeepWithinBorder()
{
	Vector2 newPos = { 0, 0 };

	if (position.x < 0 + 100) {
		newPos.x = 40;
	} 
	else if (position.x > 1080-100) {
		newPos.x = -40;
	}
	if (position.y > 720 - 100) {
		newPos.y = -40;
	}
	else if (position.y < 0 + 100) {
		newPos.y = 40;
	}

	return newPos;
}

void Boid::Move(std::vector<Boid*>& other)
{
	velocity.x += (Separates(other).x * 0.8 + KeepWithinBorder().x * 0.2) + Group(other).x + Align(other).x;
	velocity.y += (Separates(other).y * 0.8 + KeepWithinBorder().y * 0.2) + Group(other).y + Align(other).y;

	Vector2 normVelocity = Normalized(velocity);

	float targetRot = atan2(normVelocity.y, normVelocity.x);
	float angleDiff = targetRot - rotation;
	Clamp(angleDiff);

	float turnFactor = 0.05;
	if (angleDiff > turnFactor) 
	{
		angleDiff = turnFactor;
	}
	if (angleDiff < -turnFactor) 
	{
		angleDiff  = -turnFactor;
	}

	rotation += angleDiff;

	velocity.x = cos(rotation) * speed;
	velocity.y = sin(rotation) * speed;

	position.x += velocity.x * GetFrameTime();
	position.y += velocity.y * GetFrameTime();
}

void Boid::Draw()
{
	float origin = size / 2;
	DrawTexturePro(fishTexture, { 0,0,(float)fishTexture.width, (float)fishTexture.height }, { position.x,position.y, size, size }, { origin,origin }, rotation * RAD2DEG + 180, WHITE);
}

Vector2 Boid::Normalized(Vector2 vec2)
{
	if (vec2.x == 0 && vec2.y == 0) return { vec2 };
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

float Boid::Clamp(float& angle)
{
	while (angle > PI) angle -= 2 * PI;
	while (angle < -PI) angle += 2 * PI;
	return angle;
}
