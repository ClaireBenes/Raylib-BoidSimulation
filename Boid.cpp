#include "Boid.h"

#include "Obstacles.h"

Boid::Boid(float id, float x, float y, float rotation, Texture texture, Color color)
{
	this->id = id;
	position.x = x;
	position.y = y;

	this->rotation = rotation;
	this->color = color;

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

		if (distanceSqrt < maxDistance)
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
	else if (position.y < 0 + 200) {
		newPos.y = 40;
	}

	return newPos;
}

Vector2 Boid::AvoidObstacles(std::vector<Obstacles*>& obstacles)
{
	Vector2 avoidance = { 0, 0 };
	float avoidanceRadius = speed * 0.3;

	for( int i = 0; i < obstacles.size(); i++ )
	{
		Vector2 obstaclePos = obstacles[i]->GetPos();
		float obstacleWidth = obstacles[i]->GetWidth();
		float obstacleHeight = obstacles[i]->GetHeight();

		Vector2 diff = { position.x - obstaclePos.x, position.y - obstaclePos.y };

		float distSqrt = diff.x * diff.x + diff.y * diff.y;

		//Diagonal radius
		float obstacleRadius = sqrtf(( obstacleWidth * 0.5f ) * ( obstacleWidth * 0.5f ) +
			( obstacleHeight * 0.5f ) * ( obstacleHeight * 0.5f ));

		float safeRadius = avoidanceRadius + obstacleRadius;
		float safeRadiusSqrt = safeRadius * safeRadius;

		if( distSqrt < safeRadiusSqrt && distSqrt > FLT_EPSILON )
		{
			// Normalize the direction vector
			float dist = sqrtf(distSqrt);
			diff.x /= dist;
			diff.y /= dist;

			// Compute avoidance strength: stronger the closer you are
			float strength = ( safeRadius - dist ) / safeRadius;

			// Scale repulsion by strength
			avoidance.x += diff.x * strength * 5000.0f;
			avoidance.y += diff.y * strength * 5000.0f;
		}
	}

	return avoidance;
}

Vector2 Boid::AvoidBoid(std::vector<Boid*>& other)
{
	Vector2 avoidance = { 0, 0 };
	float avoidanceRadius = speed * 0.3;

	for (Boid* fish : other)
	{
		Vector2 fishPos = fish->GetPos();
		float fishSize = fish->GetSize();

		Vector2 diff = { position.x - fishPos.x, position.y - fishPos.y };

		float distSqrt = diff.x * diff.x + diff.y * diff.y;

		float safeRadius = avoidanceRadius + fishSize;
		float safeRadiusSqrt = safeRadius * safeRadius;

		if (distSqrt < safeRadiusSqrt && distSqrt > FLT_EPSILON)
		{
			// Normalize the direction vector
			float dist = sqrtf(distSqrt);
			diff.x /= dist;
			diff.y /= dist;

			// Compute avoidance strength: stronger the closer you are
			float strength = (safeRadius - dist) / safeRadius;

			// Scale repulsion by strength
			avoidance.x += diff.x * strength * 5000.0f;
			avoidance.y += diff.y * strength * 5000.0f;
		}
	}

	return avoidance;
}

Vector2 Boid::ChaseBoid(std::vector<Boid*>& other)
{
	Vector2 avoidance = { 0, 0 };
	float avoidanceRadius = speed * 2.0;

	for (Boid* fish : other)
	{
		Vector2 fishPos = fish->GetPos();
		float fishSize = fish->GetSize();

		Vector2 diff = { position.x - fishPos.x, position.y - fishPos.y };

		float distSqrt = diff.x * diff.x + diff.y * diff.y;

		float safeRadius = avoidanceRadius + fishSize;
		float safeRadiusSqrt = safeRadius * safeRadius;

		if (distSqrt < safeRadiusSqrt && distSqrt > FLT_EPSILON)
		{
			// Normalize the direction vector
			float dist = sqrtf(distSqrt);
			diff.x /= dist;
			diff.y /= dist;

			// Compute avoidance strength: stronger the closer you are
			float strength = (safeRadius - dist) / safeRadius;

			// Scale repulsion by strength
			avoidance.x -= diff.x * strength * 500.0f;
			avoidance.y -= diff.y * strength * 500.0f;
		}
	}

	return avoidance;
}

void Boid::Move(BoidGroup& group, std::vector<Obstacles*>& obstacles)
{
	constexpr float RULE_SEPARATION_MULTIPLIER = 0.8f;
	constexpr float RULE_OBSTACLES_MULTIPLIER = 1.5f;
	constexpr float RULE_KEEP_WITHIN_BORDER_MULTIPLIER = 0.2f; // 1.5f;
	constexpr float RULE_FLEE_MULTIPLIER = 0.0f; //0.2f;
	constexpr float RULE_CHASE_MULTIPLIER = 0.0f; //0.02f;

	velocity.x += (Separates(group.ourFishes).x * RULE_SEPARATION_MULTIPLIER +
		KeepWithinBorder().x * RULE_KEEP_WITHIN_BORDER_MULTIPLIER + 
		AvoidObstacles(obstacles).x * RULE_OBSTACLES_MULTIPLIER +
		AvoidBoid(group.predatorGroup->ourFishes).x * RULE_FLEE_MULTIPLIER +
		ChaseBoid(group.preyGroup->ourFishes).x * RULE_CHASE_MULTIPLIER) +
		Group(group.ourFishes).x + 
		Align(group.ourFishes).x;
	velocity.y += (Separates(group.ourFishes).y * RULE_SEPARATION_MULTIPLIER +
		KeepWithinBorder().y * RULE_KEEP_WITHIN_BORDER_MULTIPLIER +
		AvoidObstacles(obstacles).y * RULE_OBSTACLES_MULTIPLIER +
		AvoidBoid(group.predatorGroup->ourFishes).y * RULE_FLEE_MULTIPLIER +
		ChaseBoid(group.preyGroup->ourFishes).y * RULE_CHASE_MULTIPLIER) +
		Group(group.ourFishes).y + 
		Align(group.ourFishes).y;

	Vector2 normVelocity = Normalized(velocity);

	float targetRot = atan2(normVelocity.y, normVelocity.x);
	float angleDiff = targetRot - rotation;
	Clamp(angleDiff);

	float turnFactor = maxSteer * (1/speed);
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
	DrawTexturePro(fishTexture, { 0,0,(float)fishTexture.width, (float)fishTexture.height }, { position.x,position.y, size, size }, { origin,origin }, rotation * RAD2DEG + 180, color);
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

Vector2 Boid::GetPos()
{
	return position;
}

float Boid::GetSize()
{
	return size;
}
