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

Vector2 Boid::Separates(Boid* otherFish)
{
	Vector2 alldistance = { 0,0 };

	Vector2 dist = Distance(otherFish);
	float distanceSqrt = sqrt(dist.x * dist.x + dist.y * dist.y);

	if (distanceSqrt < maxDistance)
	{ 
		alldistance.x -= dist.x;
		alldistance.y -= dist.y;
	}

	if ((alldistance.x * alldistance.x + alldistance.y * alldistance.y) > FLT_EPSILON) {
		Normalize(alldistance);
	}

	return alldistance;
}

Vector2 Boid::Align(Boid* otherFish)
{
	Vector2 overallVelocity = { 0, 0 };

	overallVelocity.x += otherFish->velocity.x;
	overallVelocity.y += otherFish->velocity.y;

	overallVelocity.x = (overallVelocity.x - velocity.x) / 8;
	overallVelocity.y = (overallVelocity.y - velocity.y) / 8;

	return overallVelocity;
}

Vector2 Boid::Group(Boid* otherFish)
{
	Vector2 overallPos = { 0, 0 };

	overallPos.x += otherFish->position.x;
	overallPos.y += otherFish->position.y;

	overallPos.x = (overallPos.x - position.x) / 100;
	overallPos.y = (overallPos.y - position.y) / 100;

	return overallPos;
}

Vector2 Boid::KeepWithinBorder()
{
	Vector2 newPos = { 0, 0 };

	if (position.x < 0 + borderSize ) {
		newPos.x = 10;
	} 
	else if (position.x > GetRenderWidth() - borderSize ) {
		newPos.x = -10;
	}
	if (position.y > GetRenderHeight() - borderSize ) {
		newPos.y = -10;
	}
	else if (position.y < 0 + borderSize ) {
		newPos.y = 10;
	}

	return newPos;
}

Vector2 Boid::AvoidObstacles(std::vector<Obstacles*>& obstacles)
{
	Vector2 avoidance = { 0, 0 };
	float avoidanceRadius = speed * avoidObstacleFactor;

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
	//Rules like Speration, Alignment or Group
	Vector2 alignmentRule{}, seperationRule{}, borderRule{}, obstaclesRule{}, groupRule{};

	for( Boid* fish : group.ourFishes )
	{
		if( ( fish->id == id ) )
		{
			continue;
		}
		seperationRule.x += Normalize(Separates(fish)).x;
		seperationRule.y += Normalize(Separates(fish)).y;

		borderRule.x += Normalize(KeepWithinBorder()).x;
		borderRule.y += Normalize(KeepWithinBorder()).y;

		obstaclesRule.x += Normalize(AvoidObstacles(obstacles)).x;
		obstaclesRule.y += Normalize(AvoidObstacles(obstacles)).y;

		groupRule.x += Normalize(Group(fish)).x;
		groupRule.y += Normalize(Group(fish)).y;

		Vector2 difference = { fish->position.x - position.x, fish->position.y - position.y };
		if( Dot(velocity, difference) > 0)
		{
			alignmentRule.x += Normalize(Align(fish)).x;
			alignmentRule.y += Normalize(Align(fish)).y;
		}
	}

	//Normalize all rules
	Normalize(seperationRule);
	Normalize(borderRule);
	Normalize(obstaclesRule);
	Normalize(groupRule);
	Normalize(alignmentRule);

	//Add Rules to velocity
	velocity.x += seperationRule.x * RULE_SEPARATION_MULTIPLIER +
		borderRule.x * RULE_KEEP_WITHIN_BORDER_MULTIPLIER +
		obstaclesRule.x * RULE_OBSTACLES_MULTIPLIER +
		groupRule.x * RULE_GROUP_MULTIPLIER +
		alignmentRule.x * RULE_ALIGNMENT_MULTIPLIER;
	velocity.y += seperationRule.y * RULE_SEPARATION_MULTIPLIER +
		borderRule.y * RULE_KEEP_WITHIN_BORDER_MULTIPLIER +
		obstaclesRule.y * RULE_OBSTACLES_MULTIPLIER +
		groupRule.y * RULE_GROUP_MULTIPLIER +
		alignmentRule.y * RULE_ALIGNMENT_MULTIPLIER;

	//Prey and predators rules
	velocity.x += AvoidBoid(group.predatorGroup->ourFishes).x * RULE_FLEE_MULTIPLIER +
		ChaseBoid(group.preyGroup->ourFishes).x * RULE_CHASE_MULTIPLIER;

	velocity.y += AvoidBoid(group.predatorGroup->ourFishes).y * RULE_FLEE_MULTIPLIER +
		ChaseBoid(group.preyGroup->ourFishes).y * RULE_CHASE_MULTIPLIER;


	//Velocity based on rotation
	Vector2 normVelocity = Normalize(velocity);

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

Vector2 Boid::Normalize(Vector2 vec2)
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


float Boid::Dot(const Vector2& left, const Vector2& right) const
{
	return left.x * right.x + left.y * right.y;
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
