#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <GL/freeglut.h>

class Planet {
public:
    std::string name;
    float radius;
    float orbitRadius;
    float orbitSpeed;
    float orbitAngle;
    std::string textureFile;
    GLuint textureID;

    Planet(std::string name, float radius, float orbitRadius, float orbitSpeed, std::string textureFile);
    void loadTexture();
    void updatePosition();
    void draw();
};

#endif // PLANET_H

