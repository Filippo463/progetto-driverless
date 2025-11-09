#pragma once 
#include <cmath>


class KinematicModel {

public:
    KinematicModel(double initial_x, double initial_y, double initial_theta, double initial_velocity, double L);

    void update(double a, double delta, double dt);

    double getX() const;
    double getY() const;
    double getTheta() const;

private:
    double L;
    double state_x;
    double state_y;
    double state_theta;
    double current_velocity;
};