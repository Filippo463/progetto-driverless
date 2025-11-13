#include "KinematicModel.h"

KinematicModel::KinematicModel(double initial_x, double initial_y, double initial_theta, double initial_velocity, double L) {
    this->state_x = initial_x;
    this->state_y = initial_y;
    this->state_theta = initial_theta;
    this->current_velocity = initial_velocity;
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

double KinematicModel::getVelocity() const{
    return this->current_velocity;
}


void KinematicModel::update(double a, double delta, double dt) {
    
    double old_theta = this->state_theta;
    

    this->current_velocity = this->current_velocity + a * dt;

    
    this->state_x = this->state_x + this->current_velocity * cos(old_theta) * dt;

 
    this->state_y = this->state_y + this->current_velocity * sin(old_theta) * dt;
    
 
    this->state_theta = old_theta + (this->current_velocity / this->L) * tan(delta) * dt;

    
}
