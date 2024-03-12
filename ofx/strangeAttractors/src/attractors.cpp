#include "attractors.h"


glm::vec3 AbstractAttractor::tangent() {
    return glm::vec3{ dx(), dy(), dz() };
}

void AbstractAttractor::step() {
    position += step_size * tangent();
}

double LorenzAttractor::dx() {
    return sigma * (position.y - position.x);
}

double LorenzAttractor::dy() {
    return position.x * (rho - position.z) - position.y;
}

double LorenzAttractor::dz() {
    return position.x*position.y - beta*position.z;
}
