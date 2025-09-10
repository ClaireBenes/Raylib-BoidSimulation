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
    cout << "Hello World" << endl;

    InitWindow(1080, 720, "My first Raylib window!");
    SetTargetFPS(60);

    InitSimulation();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLUE);

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
    for (int i = 0; i < 20; i++) 
    {
        Boid* newBoid = new Boid(i, GetRandomValue(10, 1080), GetRandomValue(10, 720), GetRandomValue(0, 360));
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