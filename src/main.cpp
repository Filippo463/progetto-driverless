
#include <iostream>
#include "KinematicModel.h"
#include "raylib.h" 

int main() {
    // --- 1. SETUP FINESTRA ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Progetto Driverless");
    SetTargetFPS(60);

    // --- 2. SETUP AUTO ---
    double L = 2.5; 
    double start_x = screenWidth / 2.0;
    double start_y = screenHeight / 2.0;
    double start_theta = 0.0; 
    double start_velocity = 0.0;
    
    KinematicModel car(start_x, start_y, start_theta, start_velocity, L);


    // --- 3. SETUP SIMULAZIONE ---
    double acceleration_command = 10.0; 
    double steer_command = 0.5;     
   
    // --- 4. LOOP SIMULAZIONE ---
    while (!WindowShouldClose()) {
        
       
        double dt = GetFrameTime();

    
        car.update(acceleration_command, steer_command, dt);

        BeginDrawing(); 

            ClearBackground(DARKGRAY);

          
            DrawCircle((int)car.getX(), (int)car.getY(), 8.0f, RED);
            
            DrawText("Stai eseguendo il Livello 1!", 10, 10, 20, LIGHTGRAY);
            DrawText(TextFormat("X: %.1f", car.getX()), 10, 30, 20, LIGHTGRAY);
            DrawText(TextFormat("Y: %.1f", car.getY()), 10, 50, 20, LIGHTGRAY);

        EndDrawing(); 
    }

    // --- 5. PULIZIA ---
    CloseWindow();

    return 0;
}