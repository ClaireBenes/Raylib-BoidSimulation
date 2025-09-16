#include "raylib.h"
#include <iostream>

#include "Boid.h"
#include "Obstacles.h"

std::vector<BoidGroup> allBoids;
std::vector<Obstacles*> allObstacles;

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

        for( int j = 0; j < 50; j++ )
        {
            Boid* newBoid = new Boid(j, GetRandomValue(100, GetRenderWidth() - 100), GetRandomValue(100, GetRenderHeight() - 100), GetRandomValue(0, 360), tex, color);
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
}