#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// Function Prototypes//////////////////////////////////////////////////
void init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void timer(int value);
void drawScene();
void animateBall();
void drawMan();
void resetBall();
///////////////////////////////////////////////////////////////////////

// Global Variables //////////////////////////////////////////////////
bool animate = false;
bool ballThrown = false;
bool paused = false;
float ballPositionX = -2.0f; // Start near the man's hand
float ballPositionY = 1.5f;
float ballPositionZ = 0.0f;
float ballVelocityX = 8.0f;
float ballVelocityY = 10.0f;
float ballVelocityZ = 2.0f;
float gravity = -9.8f;
float deltaTime = 0.016f; // Assuming 60 FPS

// Rotation Variables
float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleZ = 0.0f;

// Arm Swing Variables
float armAngle = 0.0f;
float armSpeed = 3.0f;
bool armSwingingBack = true;

///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("A Man Throwing A Ball Like A Boss!");
    glewInit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);
    init();

    glutMainLoop();
    return 0;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        if (!animate && !paused) {
            // Start or replay the animation
            animate = true;
            paused = false;
            resetBall(); // Reset the scene if it's completely stopped
        } else if (animate && !paused) {
            // Pause the animation
            paused = true;
        } else if (paused) {
            // Resume the animation
            paused = false;
        }
    }

    // Rotation controls
    if (key == 'x')
        rotationAngleX += 5.0f;
    if (key == 'X')
        rotationAngleX -= 5.0f;
    if (key == 'y')
        rotationAngleY += 5.0f;
    if (key == 'Y')
        rotationAngleY -= 5.0f;
    if (key == 'z')
        rotationAngleZ += 5.0f;
    if (key == 'Z')
        rotationAngleZ -= 5.0f;

    glutPostRedisplay();
}

void timer(int value) {
    if (animate && !paused) {
        if (!ballThrown) {
            // Swing arm back and forth before throwing the ball
            if (armSwingingBack) {
                armAngle += armSpeed;
                if (armAngle >= 45.0f) {
                    armSwingingBack = false;
                    ballThrown = true;
                }
            }
        } else {
            animateBall();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void drawMan() {
    // Draw the torso
    glPushMatrix();
    glColor3f(0.8f, 0.6f, 0.4f);
    glTranslatef(-2.0f, 0.5f, 0.0f); // Move torso up
    glScalef(0.4f, 1.0f, 0.5f);
    glutSolidCube(1.0); // Torso
    glPopMatrix();

    // Draw the head
    glPushMatrix();
    glColor3f(1.0f, 0.8f, 0.6f);
    glTranslatef(-2.0f, 1.5f, 0.0f); // Move head above torso
    glutSolidSphere(0.3, 16, 16); // Head
    glPopMatrix();

    // Draw left arm
    glPushMatrix();
    glColor3f(0.8f, 0.6f, 0.4f);
    glTranslatef(-2.4f, 0.9f, 0.0f); // Position relative to torso
    glRotatef(armAngle, 1.0f, 0.0f, 0.0f); // Swing arm
    glTranslatef(0.0f, -0.4f, 0.0f);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0); // Left Arm
    glPopMatrix();

    // Draw right arm
    glPushMatrix();
    glColor3f(0.8f, 0.6f, 0.4f);
    glTranslatef(-1.6f, 0.9f, 0.0f); // Position relative to torso
    glRotatef(-armAngle, 1.0f, 0.0f, 0.0f); // Swing arm
    glTranslatef(0.0f, -0.4f, 0.0f);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0); // Right Arm
    glPopMatrix();

    // Draw left leg
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.8f);
    glTranslatef(-2.2f, -0.5f, 0.1f); // Position relative to torso
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0); // Left Leg
    glPopMatrix();

    // Draw right leg
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.8f);
    glTranslatef(-1.8f, -0.5f, 0.1f); // Position relative to torso
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0); // Right Leg
    glPopMatrix();
}


void drawScene() {
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Apply scene rotation
    glRotatef(rotationAngleX, 1.0, 0.0, 0.0);
    glRotatef(rotationAngleY, 0.0, 1.0, 0.0);
    glRotatef(rotationAngleZ, 0.0, 0.0, 1.0);

    drawMan();

    // Draw the ball
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(ballPositionX, ballPositionY, ballPositionZ);
    glutSolidSphere(0.3, 16, 16);
    glPopMatrix();

    // Draw walls, ceiling, and ground
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    // Left Wall
    glVertex3f(-5.0f, -5.0f, -5.0f);
    glVertex3f(-5.0f, -5.0f, 5.0f);
    glVertex3f(-5.0f, 5.0f, 5.0f);
    glVertex3f(-5.0f, 5.0f, -5.0f);
    // Right Wall
    glVertex3f(5.0f, -5.0f, -5.0f);
    glVertex3f(5.0f, -5.0f, 5.0f);
    glVertex3f(5.0f, 5.0f, 5.0f);
    glVertex3f(5.0f, 5.0f, -5.0f);
    // Ceiling
    glVertex3f(-5.0f, 5.0f, -5.0f);
    glVertex3f(-5.0f, 5.0f, 5.0f);
    glVertex3f(5.0f, 5.0f, 5.0f);
    glVertex3f(5.0f, 5.0f, -5.0f);
    // Ground
    glVertex3f(-5.0f, -5.0f, -5.0f);
    glVertex3f(-5.0f, -5.0f, 5.0f);
    glVertex3f(5.0f, -5.0f, 5.0f);
    glVertex3f(5.0f, -5.0f, -5.0f);
    glEnd();
}

void animateBall() {
    ballPositionX += ballVelocityX * deltaTime;
    ballPositionY += ballVelocityY * deltaTime;
    ballPositionZ += ballVelocityZ * deltaTime;
    ballVelocityY += gravity * deltaTime;

    // Check for collisions
    if (ballPositionX >= 5.0f || ballPositionX <= -5.0f) {
        ballVelocityX = -ballVelocityX;
    }
    if (ballPositionY <= 0.3f) {
        ballVelocityY = -ballVelocityY * 0.8f;
    }
    if (ballPositionZ >= 5.0f || ballPositionZ <= -5.0f) {
        ballVelocityZ = -ballVelocityZ;
    }
}
void resetBall() {
    ballPositionX = -2.0f;
    ballPositionY = 1.5f;
    ballPositionZ = 0.0f;
    ballVelocityX = 8.0f;
    ballVelocityY = 10.0f;
    ballVelocityZ = 2.0f;
    ballThrown = false;
}
