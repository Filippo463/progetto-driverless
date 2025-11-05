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
    
    double old_theta = this->state_theta;

    
    this->state_x = this->state_x + v * cos(old_theta) * dt;

 
    this->state_y = this->state_y + v * sin(old_theta) * dt;
    
 
    this->state_theta = old_theta + (v / this->L) * tan(delta) * dt;
}
