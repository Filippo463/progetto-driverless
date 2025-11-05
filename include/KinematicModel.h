#pragma once // Comando standard per evitare inclusioni multiple
#include <cmath>


class KinematicModel {

public:
    KinematicModel(double initial_x, double initial_y, double initial_theta, double L);

    void update(double v, double delta, double dt);

    double getX() const;
    double getY() const;
    double getTheta() const;

private:
    double L;
    double state_x;
    double state_y;
    double state_theta;
};