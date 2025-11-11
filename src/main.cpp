
#include <iostream>
#include "KinematicModel.h"
#include "raylib.h" 

double getPathTargetY(double x) {
        
    double ampiezza = 100.0;
    double frequenza = 0.01;
    double offset_y = 720 / 2.0; 
    
    return ampiezza * sin(frequenza * x) + offset_y;
}

int main() {
    // --- 1. SETUP FINESTRA ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Progetto Driverless");
    SetTargetFPS(60);

    // --- 2. SETUP AUTO ---
    double L = 2.5; 
    double start_x = 0;
    double start_y = getPathTargetY(start_x);
    double start_theta = 0.0; 
    double start_velocity = 0.0;
    
    KinematicModel car(start_x, start_y, start_theta, start_velocity, L);


    // --- 3. SETUP SIMULAZIONE ---
    double Kp_t = 0.25;
    double Kp_y = 0.01;
    double acceleration_command = 1.0; 
    double steer_command = 0.0;

    
    double max_steering_control = 0.5;


      
   
    // --- 4. LOOP SIMULAZIONE ---
    while (!WindowShouldClose()) {
        
       
        double dt = GetFrameTime();
        double current_theta = car.getTheta();
        double current_y = car.getY();
        double current_x = car.getX();

        double target_y = getPathTargetY(current_x);


        double error_y = target_y - current_y;

        //double steer_command = Kp_t * error_t;
    
        double desired_steer = Kp_y * error_y; 

        if (desired_steer > max_steering_control) {
            steer_command = max_steering_control;
        } else if (desired_steer < -max_steering_control) { 
            steer_command = -max_steering_control;
        } else {
            steer_command = desired_steer;
        }
        
        car.update(acceleration_command, steer_command, dt);

        BeginDrawing(); 

            ClearBackground(DARKGRAY);

          
            DrawCircle((int)car.getX(), (int)car.getY(), 8.0f, RED);
            
            DrawText("Stai eseguendo il Livello 1!", 10, 10, 20, LIGHTGRAY);
            DrawText(TextFormat("X: %.1f", car.getX()), 10, 30, 20, LIGHTGRAY);
            DrawText(TextFormat("Y: %.1f", car.getY()), 10, 50, 20, LIGHTGRAY);

           // --- DISEGNO PERCORSO ---
            

            for (int x = 0; x < screenWidth; x++) {
                double y = getPathTargetY(x);
                DrawCircle(x, (int)y, 2.0f, LIME);
}

        EndDrawing(); 
    }

    // --- 5. PULIZIA ---
    CloseWindow();

    return 0;
}