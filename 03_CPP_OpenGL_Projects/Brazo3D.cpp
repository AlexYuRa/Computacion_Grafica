#include <GL/glut.h>
#include <iostream>

using namespace std;

float angTronco = 0, angBrazo1 = 0, angAnteBrazo1 = 0;

// Quadric used to draw primitives if needed
GLUquadricObj *p = nullptr;

void inicializar() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // Create the quadric after an OpenGL context exists
    if (!p) p = gluNewQuadric();
    if (p) gluQuadricNormals(p, GLU_SMOOTH);
    glEnable(GL_NORMALIZE);
}

void graficarEjes ()
{
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); //Eje X en rojo
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(20.0, 0.0, 0.0);

        glColor3f(0.0, 1.0, 0.0); //Eje Y en verde
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 20.0, 0.0);

        glColor3f(0.0, 0.0, 1.0); //Eje Z en azul
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 20.0);
    glEnd();
}

void graficar() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    graficarEjes();

    // Tronco y partes
    glPushMatrix();
        // Rotación global del tronco
        glRotatef(angTronco, 0.0f, 1.0f, 0.0f);

        // Tronco (caja escalada)
        glPushMatrix();
            glTranslatef(0.0f, 5.0f, 0.0f);
            glScalef(1.0f, 5.0f, 1.0f);
            glColor3f(0.6f, 0.6f, 0.6f);
            glutWireCube(2.0);
        glPopMatrix();

        // Cabeza (opcional)
        glPushMatrix();
            glColor3f(1.0f, 1.0f, 0.0f);
            glTranslatef(0.0f, 11.0f, 0.0f);
            glutWireSphere(1.5, 16, 16);
        glPopMatrix();

        // Brazo 1
        glPushMatrix();
            glColor3f(1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, 3.0f);
            glRotatef(angBrazo1, 1.0f, 0.0f, 0.0f);
            gluCylinder(p, 2, 1, 6, 16, 16);
            glPushMatrix();
                glColor3f(0.0f, 0.0f, 1.0f);
                glTranslatef(0.0f, 6.0f, 0.0f);
                glColor3f(0.8f, 0.8f, 0.0f);
                glutWireCube(2.0);
            glPopMatrix();

            // Antebrazo 1
            glPushMatrix();
                glTranslatef(0.0f, 3.0f, 0.0f);
                glRotatef(angAnteBrazo1, 1.0f, 0.0f, 0.0f);
                glTranslatef(0.0f, 2.5f, 0.0f);
                glPushMatrix();
                    glScalef(1.0f, 2.5f, 1.0f);
                    glColor3f(0.8f, 0.5f, 0.2f);
                    glutWireCube(2.0);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void rotarAnteBrazo1(int i) {
    angAnteBrazo1 = angAnteBrazo1 +5;
    glutPostRedisplay();
    glutTimerFunc(100, rotarAnteBrazo1, 1);
};   

void rotarBrazo1(int i) {
    angBrazo1 = angBrazo1 +5;
    glutPostRedisplay();
    glutTimerFunc(100, rotarBrazo1, 1);
};

void rotarTronco(int i) {
    angTronco = angTronco +5;
    glutPostRedisplay();
    glutTimerFunc(100, rotarTronco, 1);
};

// Ventana reshape
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspective projection
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Teclado simple: ESC para salir
void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // ESC
            // cleanup
            if (p) { gluDeleteQuadric(p); p = nullptr; }
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Brazo 3D - Animado");

    inicializar();

    glutDisplayFunc(graficar);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);

    // Start the timers
    glutTimerFunc(100, rotarTronco, 1);
    glutTimerFunc(200, rotarBrazo1, 1);
    glutTimerFunc(300, rotarAnteBrazo1, 1);

    glutMainLoop();
    return 0;
}


