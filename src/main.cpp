#include <iostream>
#include "KinematicModel.h"
#include "raylib.h" 
#include <vector>
#include <corecrt_math_defines.h>

//Funzione per calcolare la distanza tra due punti
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



    // --- SETUP AUTO ---
    double L = 2.5; 
    double start_x = path[0].x;
    double start_y = path[0].y;
    double start_velocity = 0.0;


    //calcolo l'angolo di partenza corretto
    double dx_start = path[1].x - path[0].x;
    double dy_start = path[1].y - path[0].y;
    double start_theta = atan2(dy_start, dx_start);
    
    KinematicModel car(start_x, start_y, start_theta, start_velocity, L);


    // --- SETUP SIMULAZIONE (Guadagni per Stanley) ---
    
    // Kp_t ora è il guadagno per l'ERRORE DI ANGOLO (theta_e)
    double Kp_t = 0.25; 
    // K_stanley è il guadagno per l'ERRORE LATERALE (e_y)
    double K_stanley = 0.5; // (Da tarare!)

    

    double acceleration_command = 0.5; // Teniamo l'accelerazione bassa per il tuning
    double steer_command = 0.0;

    
    double max_steering_control = 0.5;


      
   
    // --- LOOP SIMULAZIONE ---
    while (!WindowShouldClose()) {
        
        // --- LOGICA STANLEY CONTROLLER ---
        

        //trovo il Waypoint più vicino
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
        
        //calcolo errore laterale (e_y) con segno
        Vector2 center_point = {(float)center_x, (float)center_y};
        double dist_from_center = getDistance(current_point, center_point);

        double e_y = dist_from_center - radius; 
        // e_y > 0  => FUORI dal cerchio
        // e_y < 0  => DENTRO il cerchio
        

        //calcolo errore di angolo
        
        //trovo angolo del percorso nel punto 'min_index'
        int next_index = min_index + 1;
        //wrap-around
        if (next_index >= path.size()) {
            next_index = 0;
        }
        Vector2 path_point_start = path[min_index];
        Vector2 path_point_end = path[next_index];

        double dx_path = path_point_end.x - path_point_start.x;
        double dy_path = path_point_end.y - path_point_start.y;
        double path_theta = atan2(dy_path, dx_path); // Angolo del percorso

        // Calcolo errore di angolo
        double current_theta = car.getTheta();
        double error_theta = path_theta - current_theta;

        // normalizzazione
        while (error_theta > M_PI) {
            error_theta -= 2.0 * M_PI; 
        }
        while (error_theta < -M_PI) {
            error_theta += 2.0 * M_PI; 
        }

        //calcolo formula Stanley
        
        double current_v = car.getVelocity();
        //aggiungo 'epsilon' per evitare divisione per zero quando v=0
        double v_stable = current_v + 0.1; 

        // angolo
        double term_heading = Kp_t * error_theta; 
        // posizione
        double term_position = atan(K_stanley * e_y / v_stable);

        double desired_steer = term_heading + term_position;
        
        
        double dt = GetFrameTime();


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
            
            DrawText("Stai eseguendo il Livello 5 (Stanley)!", 10, 10, 20, LIGHTGRAY);
            DrawText(TextFormat("X: %.1f", car.getX()), 10, 30, 20, LIGHTGRAY);
            DrawText(TextFormat("Y: %.1f", car.getY()), 10, 50, 20, LIGHTGRAY);
            DrawText(TextFormat("V: %.1f", car.getVelocity()), 10, 70, 20, LIGHTGRAY);
            DrawText(TextFormat("e_y: %.1f", e_y), 10, 90, 20, LIGHTGRAY); // Debug Errore Laterale
            DrawText(TextFormat("e_th: %.1f", error_theta), 10, 110, 20, LIGHTGRAY); // Debug Errore Angolo

            // Disegno del percorso
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