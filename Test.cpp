#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdlib>

// S? l�?ng h�nh tinh
const int NUM_PLANETS = 8;
float planetAngles[NUM_PLANETS] = {0.0f};
float cameraDistance = 20.0f;
float cameraAngleY = 0.0f;
float cameraAngleX = 0.0f;
bool isDay = true;
float planetSpeeds[NUM_PLANETS] = {0.1f, 0.08f, 0.06f, 0.05f, 0.04f, 0.03f, 0.02f, 0.01f};
float cometSpeed = 0.1f;
bool showOrbits = true;

// L�u tr? v? tr� c�c h�nh tinh
std::vector<std::pair<float, float > > planetTrails[NUM_PLANETS];

// Bi?n cho sao b�ng
float cometPosition[3] = {20.0f, 0.0f, 0.0f};

// V? h?nh c?u
void drawSphere(float radius) {
    glutSolidSphere(radius, 20, 20);
}

// V? ��?ng �i c?a c�c h�nh tinh
void drawTrail() {
    glColor3f(1.0f, 1.0f, 1.0f); // M�u ��?ng �i
    for (int i = 0; i < NUM_PLANETS; i++) {
        glBegin(GL_LINE_STRIP);
        for (size_t j = 0; j < planetTrails[i].size(); j++) {
            glVertex3f(planetTrails[i][j].first, planetTrails[i][j].second, 0.0f);
        }
        glEnd();
    }
}

// V? sao b�ng
void drawComet() {
    glColor3f(1.0f, 1.0f, 0.0f); // M�u sao b�ng
    glPushMatrix();
    glTranslatef(cometPosition[0], cometPosition[1], cometPosition[2]);
    drawSphere(0.2f); // K�ch th�?c sao b�ng
    glPopMatrix();
}

// V? c�c ng�i sao n?n
void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) {
        glVertex3f(rand() % 40 - 20, rand() % 40 - 20, rand() % 40 - 20);
    }
    glEnd();
}

// V? qu? �?o c?a c�c h�nh tinh
void drawOrbit(float radius) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex3f(cos(angle) * radius, sin(angle) * radius, 0.0f);
    }
    glEnd();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_position[] = { 0.0f, 0.0f, 10.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Thay �?i m�u n?n theo ng�y v� ��m
    if (isDay) {
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // �?t v? tr� camera
    float cameraX = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    float cameraY = cameraDistance * sin(cameraAngleX);
    float cameraZ = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // V? M?t Tr?i
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);
    drawSphere(2.0f);
    glPopMatrix();

    // V? c�c ng�i sao n?n
    if (!isDay) {
        drawStars();
    }

    // Kho?ng c�ch v� m�u s?c cho c�c h�nh tinh
    float distances[NUM_PLANETS] = {3.0f, 5.0f, 7.0f, 9.0f, 11.0f, 13.0f, 15.0f, 17.0f};
    float colors[NUM_PLANETS][3] = {
        {0.5f, 0.5f, 0.5f}, // Sao Th?y
        {1.0f, 0.5f, 0.0f}, // Sao Kim
        {0.0f, 0.0f, 1.0f}, // Tr�i �?t
        {1.0f, 0.0f, 0.0f}, // Sao H?a
        {1.0f, 0.5f, 0.0f}, // Sao M?c
        {1.0f, 0.8f, 0.0f}, // Sao Th?
        {0.0f, 0.5f, 0.5f}, // Sao Thi�n V��ng
        {0.0f, 0.0f, 1.0f}  // Sao H?i V��ng
    };

    for (int i = 0; i < NUM_PLANETS; i++) {
        glPushMatrix();
        planetAngles[i] += planetSpeeds[i]; // T�ng t?c �? quay cho c�c h�nh tinh
        if (planetAngles[i] > 360.0f) planetAngles[i] -= 360.0f;
        glRotatef(planetAngles[i], 0.0f, 0.0f, 1.0f);
        glTranslatef(distances[i], 0.0f, 0.0f);
        glColor3f(colors[i][0], colors[i][1], colors[i][2]);
        drawSphere(0.5f); // K�ch th�?c h�nh tinh

        // L�u v? tr� h�nh tinh �? v? ��?ng �i
        planetTrails[i].push_back(std::make_pair(
            distances[i] * cos(planetAngles[i] * M_PI / 180.0),
            distances[i] * sin(planetAngles[i] * M_PI / 180.0)
        ));
        glPopMatrix();
    }

    // V? c�c ��?ng �i c?a c�c h�nh tinh
    drawTrail();

    // V? qu? �?o c?a c�c h�nh tinh
    if (showOrbits) {
        for (int i = 0; i < NUM_PLANETS; i++) {
            drawOrbit(distances[i]);
        }
    }

    // C?p nh?t v? tr� sao b�ng
    cometPosition[0] -= cometSpeed; // Di chuy?n sao b�ng
    if (cometPosition[0] < -20.0f) { // N?u ra ngo�i m�n h?nh, reset l?i
        cometPosition[0] = 20.0f;
        cometPosition[1] = (rand() % 100 / 50.0f) - 1.0f; // Di chuy?n ng?u nhi�n tr�n tr?c y
    }

    drawComet();
    glutSwapBuffers();
}

void timer(int value) {
    static int dayNightTimer = 0;
    dayNightTimer++;
    if (dayNightTimer > 300) {
        isDay = !isDay;
        dayNightTimer = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            cameraDistance -= 1.0f; // Ph�ng to
            break;
        case '-':
            cameraDistance += 1.0f; // Thu nh?
            break;
        case 'w':
            cameraAngleX += 0.1f; // Xoay camera l�n
            break;
        case 's':
            cameraAngleX -= 0.1f; // Xoay camera xu?ng
            break;
        case 'a':
            cameraAngleY -= 0.1f; // Xoay camera sang tr�i
            break;
        case 'd':
            cameraAngleY += 0.1f; // Xoay camera sang ph?i
            break;
        case 't': // T?nh ti?n
            for (int i = 0; i < NUM_PLANETS; i++) {
                planetTrails[i].clear(); // X�a ��?ng �i c?
                planetAngles[i] += 10.0f; // T?nh ti?n m?i h�nh tinh
            }
            break;
        case 'r': // Quay
            for (int i = 0; i < NUM_PLANETS; i++) {
                planetTrails[i].clear(); // X�a ��?ng �i c?
                planetAngles[i] += 45.0f; // Quay m?i h�nh tinh
            }
            break;
        case 'z': // T? l?
            for (int i = 0; i < NUM_PLANETS; i++) {
                planetTrails[i].clear(); // X�a ��?ng �i c?
                planetAngles[i] *= 1.5f; // T�ng t? l? m?i h�nh tinh
            }
            break;
        case 'x': // �?i x?ng qua tr?c x
            for (int i = 0; i < NUM_PLANETS; i++) {
                for (size_t j = 0; j < planetTrails[i].size(); j++) {
                    planetTrails[i][j].second = -planetTrails[i][j].second; // �?i x?ng qua tr?c x
                }
            }
            break;
        case 'y': // �?i x?ng qua tr?c y
            for (int i = 0; i < NUM_PLANETS; i++) {
                for (size_t j = 0; j < planetTrails[i].size(); j++) {
                    planetTrails[i][j].first = -planetTrails[i][j].first; // �?i x?ng qua tr?c y
                }
            }
            break;
        case 27: // Ph�m Esc �? tho�t
            exit(0);
            break;
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Solar System Simulation");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

