#include "planet.h"
#include <SOIL/SOIL.h>
#include <cmath>

Planet::Planet(std::string name, float radius, float orbitRadius, float orbitSpeed, std::string textureFile)
    : name(name), radius(radius), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), orbitAngle(0.0f), textureFile(textureFile) {}

void Planet::loadTexture() {
    textureID = SOIL_load_OGL_texture(
        textureFile.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
}

void Planet::updatePosition() {
    orbitAngle += orbitSpeed;
    if (orbitAngle > 360.0f) orbitAngle -= 360.0f;
}

void Planet::draw() {
    glPushMatrix();
    glRotatef(orbitAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(orbitRadius, 0.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, textureID);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluSphere(quad, radius, 50, 50);
    gluDeleteQuadric(quad);

    glPopMatrix();
}

