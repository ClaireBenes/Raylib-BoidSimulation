#include "raylib.h"
#include <iostream>

#include "Boid.h"
#include "Obstacles.h"

std::vector<BoidGroup> allBoids;
std::vector<Obstacles*> allObstacles;

float borderSize = 100;

Obstacles* currentObstacle = nullptr;
Vector2 obstacleStart = { 0, 0 };
Vector2 obstacleEnd = { 0, 0 };

void InitSimulation();
void Update();
void Draw();
void EndScene();

int main() { 
    InitWindow(1920, 1080, "ALL BOIDS");
    SetTargetFPS(60);

    InitSimulation();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        Update();
        Draw();

        EndDrawing();
    }

    EndScene();

    CloseWindow();
    return 0;
}

void InitSimulation()
{
    //Obstacles
    Obstacles* newObstacle = new Obstacles(500, 700, 200, 300, false);
    allObstacles.push_back(newObstacle);

    Obstacles* newObstacle2 = new Obstacles(1300, 300, 100, 100, false);
    allObstacles.push_back(newObstacle2);

    Obstacles* mouseObstacle = new Obstacles(10, 10, 30, 50, true);
    allObstacles.push_back(mouseObstacle);

    //Fish
    Texture tex = LoadTexture("resources/WhiteFish.png");

    for( int i = 0; i < 10; i++ )
    {
        BoidGroup boidGroup {};

        Color color = Color{ (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), 255 };

        for( int j = 0; j < 70; j++ )
        {
            Boid* newBoid = new Boid(j, GetRandomValue(0 + borderSize, GetRenderWidth() - borderSize), GetRandomValue(0 + borderSize, GetRenderHeight() - borderSize), GetRandomValue(0, 360), tex, color);
            boidGroup.ourFishes.push_back(newBoid);
        }
        allBoids.push_back(boidGroup);
    }

    // Assign predator & prey groups
    for (int i = 0; i < allBoids.size(); i++)
    {
        BoidGroup& group = allBoids[i];

        BoidGroup* nextGroup = nullptr;
        if (i != allBoids.size() - 1)
        {
            nextGroup = &allBoids[i + 1];
        }
        else
        {
            nextGroup = &allBoids[0];
        }

        group.predatorGroup = nextGroup;
        nextGroup->preyGroup = &group;
    }
}

void Update() 
{
    for( int i = 0; i < allObstacles.size(); i++ )
    {
        allObstacles[i]->Update();
    }

    for( BoidGroup& group : allBoids )
    {
        for( Boid* fish : group.ourFishes )
        {
            fish->Move( group, allObstacles );
        }
    }

    //Create Obstacles by dragging mouse
    if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) )
    {
        obstacleStart = { (float) GetMouseX(), (float) GetMouseY() };
        currentObstacle = new Obstacles(obstacleStart.x, obstacleStart.y, 1, 1, false);
    }

    if( IsMouseButtonDown(MOUSE_LEFT_BUTTON) && currentObstacle )
    {
        Vector2 currentMouse = { (float) GetMouseX(), (float) GetMouseY() };

        float centerX = ( obstacleStart.x + currentMouse.x ) / 2.0f;
        float centerY = ( obstacleStart.y + currentMouse.y ) / 2.0f;

        float dx = currentMouse.x - obstacleStart.x;
        float dy = currentMouse.y - obstacleStart.y;
        float radius = sqrtf(dx * dx + dy * dy) / 2.0f;


        currentObstacle->SetPos(centerX, centerY);
        currentObstacle->SetSize(radius, radius);
    }

    if( IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentObstacle )
    {
        allObstacles.push_back(currentObstacle);
        currentObstacle = nullptr;
    }
}

void Draw() 
{
    for (BoidGroup& group : allBoids)
    {
        for (Boid* fish : group.ourFishes)
        {
            fish->Draw();
        }
    }

    for( int i = 0; i < allObstacles.size(); i++ )
    {
        allObstacles[i]->Draw();
    }

    if( currentObstacle )
    {
        currentObstacle->Draw();
    }
}

void EndScene() 
{
    for (BoidGroup& group : allBoids)
    {
        for (Boid* fish : group.ourFishes)
        {
            delete fish;
        }
    }
    allBoids.clear();

    for( Obstacles* obstacle : allObstacles )
    {
        delete obstacle;
    }

    allObstacles.clear();
}