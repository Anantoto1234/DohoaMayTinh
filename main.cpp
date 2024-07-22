#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "planet.h"

struct Camera {
    float angleX, angleY, distance;
    Camera() : angleX(0), angleY(0), distance(10) {}
} camera;

std::vector<Planet> planets;

void initPlanets() {
    planets.push_back(Planet("Sun", 0.5f, 0.0f, 0.0f, "sun.jpg"));
    planets.push_back(Planet("Earth", 0.1f, 3.0f, 0.01f, "earth.jpg"));
    planets.push_back(Planet("Mars", 0.07f, 4.5f, 0.008f, "mars.jpg"));
}

void loadTextures() {
    for (auto& planet : planets) {
        planet.loadTexture();
    }
}

void drawPlanets() {
    for (auto& planet : planets) {
        planet.updatePosition();
        planet.draw();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    gluLookAt(camera.distance * sin(camera.angleY) * cos(camera.angleX),
              camera.distance * sin(camera.angleX),
              camera.distance * cos(camera.angleY) * cos(camera.angleX),
              0, 0, 0, 0, 1, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    drawPlanets();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void mouseMotion(int x, int y) {
    static int lastX = -1, lastY = -1;
    if (lastX != -1 && lastY != -1) {
        camera.angleY += (x - lastX) * 0.005f;
        camera.angleX += (y - lastY) * 0.005f;
    }
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) camera.distance -= 0.5f;
    else camera.distance += 0.5f;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interactive Solar System");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    initPlanets();
    loadTextures();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);

    glutMainLoop();
    return 0;
}

