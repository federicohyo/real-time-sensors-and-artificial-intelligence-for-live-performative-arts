#pragma once
#include "glm/glm.hpp"


class AbstractAttractor {
public:
    float step_size{0.02};
    glm::vec3 position{ 0.1, 0.0, 0.0 };
    
    void step();
    glm::vec3 tangent();
    
private:
    virtual double dx() { return 0.0; };
    virtual double dy() { return 0.0; };
    virtual double dz() { return 0.0; };
};


class LorenzAttractor : public AbstractAttractor {
private:
    double sigma{0.0};
    double rho{0.0};
    double beta{0.0};
    
public:
    LorenzAttractor(double sigma=0.0, double rho=0.0, double beta=0.0): sigma(sigma), rho(rho), beta(beta) {};
    virtual double dx();
    virtual double dy();
    virtual double dz();
};

