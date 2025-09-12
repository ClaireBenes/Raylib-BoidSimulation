#include "raylib.h"
#include <iostream>

#include "Boid.h"
#include "Obstacles.h"

std::vector<Boid*> boids;
std::vector<Boid*> boids2;

std::vector<std::vector<Boid*>> allBoids;
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
    Obstacles* newObstacle = new Obstacles(500, 500, 200, 300);
    allObstacles.push_back(newObstacle);

    Obstacles* newObstacle2 = new Obstacles(1000, 800, 100, 50);
    allObstacles.push_back(newObstacle2);

    Obstacles* newObstacle3 = new Obstacles(1400, 300, 300, 200);
    allObstacles.push_back(newObstacle3);

    Texture tex = LoadTexture("resources/WhiteFish.png");

    for( int i = 0; i < 8; i++ )
    {
        std::vector<Boid*> boidGroup;

        Color color = Color{ (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), (unsigned char) GetRandomValue(0, 255), 255 };

        for( int j = 0; j < 50; j++ )
        {
            Boid* newBoid = new Boid(j, GetRandomValue(100, 1080 - 100), GetRandomValue(100, 720 - 100), GetRandomValue(0, 360), tex, color);
            boidGroup.push_back(newBoid);
        }
        allBoids.push_back(boidGroup);
    }
}

void Update() 
{
    for( int i = 0; i < allBoids.size(); i++ )
    {
        for( int j = 0; j < allBoids[i].size(); j++ )
        {
            allBoids[i][j]->Move(allBoids[i], allObstacles);
        }
    }
}

void Draw() 
{
    for( int i = 0; i < allObstacles.size(); i++ )
    {
        allObstacles[i]->Draw();
    }

    for( int i = 0; i < allBoids.size(); i++ )
    {
        for( int j = 0; j < allBoids[i].size(); j++ )
        {
            allBoids[i][j]->Draw();
        }
    }
}

void EndScene() 
{
    for( int i = 0; i < allBoids.size(); i++ )
    {
        for( int j = 0; j < allBoids[i].size(); j++ )
        {
            delete allBoids[i][j];
        }
    }

    allBoids.clear();
}