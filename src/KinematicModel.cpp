#include "KinematicModel.h"

KinematicModel::KinematicModel(double initial_x, double initial_y, double initial_theta, double L) {
    this->state_x = initial_x;
    this->state_y = initial_y;
    this->state_theta = initial_theta;
    this->L = L;
}


double KinematicModel::getX() const {
    return this->state_x;
}

double KinematicModel::getY() const {
    return this->state_y;
}

double KinematicModel::getTheta() const {
    return this->state_theta;
}


void KinematicModel::update(double v, double delta, double dt) {
    
    // NOTA: Le formule trigonometriche (sin, cos, tan) in C++
    // si aspettano angoli in RADIANTI.
    // Il tuo 'state_theta' DEVE essere in radianti.
    // Anche 'delta' (sterzo) deve essere in radianti.
    
    // Per sicurezza, salviamo i vecchi valori prima di cambiarli
    double old_theta = this->state_theta;

    // x_{t+1} = x_t + v * cos(theta) * dt
    this->state_x = this->state_x + v * cos(old_theta) * dt;

    // y_{t+1} = y_t + v * sin(theta) * dt
    this->state_y = this->state_y + v * sin(old_theta) * dt;
    
    // theta_{t+1} = theta_t + (v / L) * tan(delta) * dt
    this->state_theta = old_theta + (v / this->L) * tan(delta) * dt;
}
