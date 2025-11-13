
#include <iostream>
#include "KinematicModel.h"
#include "raylib.h" 
#include <vector>
#include <corecrt_math_defines.h>

double getDistance(Vector2 p1, Vector2 p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

int main() {
    

    // --- SETUP FINESTRA ---
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Progetto Driverless");
    SetTargetFPS(60);

    // --- CREAZIONE PATH ---  
    std::vector<Vector2> path; //creazione vettore per Waypoints
    double radius = 200.0;
    double center_x = screenWidth / 2.0;
    double center_y = screenHeight / 2.0;
    int num_points = 200; // Più punti =  percorso più fluido

    
    for (int i = 0; i <= num_points; i++) {
        // t è l'angolo:  [0; 2PI]
        double t = (double)i / num_points * (2.0 * M_PI); 

        double x = center_x + radius * cos(t);
        double y = center_y + radius * sin(t);

        path.push_back(Vector2{(float)x, (float)y});
    }

    double Ld = 100.0; //Lookahead Distance


    // --- SETUP AUTO ---
    double L = 2.5; 
    double start_x = path[0].x;
    double start_y = path[0].y;
    double start_theta = 0.0; 
    double start_velocity = 0.0;
    
    KinematicModel car(start_x, start_y, start_theta, start_velocity, L);


    // --- SETUP SIMULAZIONE ---
    double Kp_t = 0.25;

    double error_integral = 0.0;
    double Ki = 0.1;

    double acceleration_command = 0.5; 
    double steer_command = 0.0;

    
    double max_steering_control = 0.5;


      
   
    // --- LOOP SIMULAZIONE ---
    while (!WindowShouldClose()) {
        
        //trovo il Waypoint più vicino a me
        Vector2 current_point = {(float)car.getX(), (float)car.getY()};
        double min_distance = 100000.0;
        int min_index = 0;
        for (int i = 0; i < path.size(); i++) {
            double d = getDistance(current_point, path[i]); 
            if (d < min_distance) {
                min_distance = d;
                min_index = i;
            }
        }
        

        //trovo il waypoint più vicino al mio lookahead distance
        int target_index = min_index; 

        for (int i = min_index; i < path.size(); i++) {
            double d = getDistance(current_point, path[i]);
            if (d > Ld) {
                target_index = i;
                break; 
            }
            target_index = i; 
        }

        //caso in cui sono alla fine del percorso
        if (target_index >= path.size()) {
            target_index = path.size() - 1;
        }

        Vector2 target_point = path[target_index];

        //trovo l'angolo target
        double dx = target_point.x - current_point.x;
        double dy = target_point.y - current_point.y;

        double target_theta = atan2(dy, dx);



        
        double dt = GetFrameTime();

        //calcolo errore e desired steer
        double current_theta = car.getTheta();

        double error_theta = target_theta - current_theta;

        // normalizzazione 
        while (error_theta > M_PI) {
            error_theta -= 2.0 * M_PI; 
        }
        while (error_theta < -M_PI) {
            error_theta += 2.0 * M_PI; 
        }

        //calcolo errore integrale
        error_integral += error_theta * dt;

        //nuovo calcolo desired_steer
        double desired_steer = (Kp_t * error_theta) + (Ki * error_integral);


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
            DrawText(TextFormat("V: %.1f", car.getVelocity()), 10, 70, 20, LIGHTGRAY);

            for (size_t i = 0; i < path.size() - 1; i++) {
                DrawLineV(path[i], path[i+1], LIME);
            }
            // Connettere l'ultimo punto al primo 
            DrawLineV(path.back(), path.front(), LIME);

        EndDrawing(); 
    }

    // --- PULIZIA ---
    CloseWindow();

    return 0;
}