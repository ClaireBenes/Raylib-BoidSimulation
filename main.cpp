#include "raylib.h"
#include <iostream>

#include "Boid.h"

using namespace std;

std::vector<Boid*> boids;

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
    Texture tex = LoadTexture("resources/WhiteFish.png");

    for (int i = 0; i < 50; i++) 
    {
        Boid* newBoid = new Boid(i, GetRandomValue(100, 1080 - 100), GetRandomValue(100, 720 - 100), GetRandomValue(0, 360), tex);
        boids.push_back(newBoid);
    }
}

void Update() 
{
    for (int i = 0; i < boids.size(); i++)
    {
        boids[i]->Move(boids);
    }
}

void Draw() 
{
    for (int i = 0; i < boids.size(); i++) 
    {
        boids[i]->Draw();
    }       
}

void EndScene() 
{
    for (int i = 0; i < boids.size(); i++) 
    {
        delete boids[i];
    }

    boids.clear();
}