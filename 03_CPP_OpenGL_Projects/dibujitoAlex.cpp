#include <GL/glut.h>

float tx = 0;            // Posición en el eje X
float ty = 0;            // Posición en el eje Y (para el salto)
int saltando = 0;        // Bandera para controlar el salto
float velocidadSalto = 0; // Velocidad del salto
float direccionSalto = 0; // Dirección del salto actual

// Estados del teclado
int teclaApretada[256] = {0}; // Array para rastrear si una tecla está presionada

void inicializar()
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
}

void Cuadrado(){
    // Dibuja un cuadrado simple en 2D
    glColor3f(1, 0, 0); // Color rojo
    glBegin(GL_QUADS);
        glVertex2f(-10, -10);
        glVertex2f(10, -10);
        glVertex2f(10, 10);
        glVertex2f(-10, 10);
    glEnd();
}

void graficar()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Traslación del cuadrado en los ejes X e Y
    glTranslatef(tx, ty, 0);
    // Dibujar el cuadrado
    Cuadrado();

    glFlush();
}

void redimensionar(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, -1, 1); // Proyección ortogonal 2D
}

void manejarTeclasPresionadas()
{
    if (teclaApretada['a'] && !saltando) // Tecla 'a' presionada - salto hacia la izquierda
    {
        saltando = 1;
        velocidadSalto = 10; // Velocidad inicial del salto
        direccionSalto = -0.2; // Dirección hacia la izquierda
    }
    if (teclaApretada['b'] && !saltando) // Tecla 'b' presionada - salto hacia la derecha
    {
        saltando = 1;
        velocidadSalto = 10; // Velocidad inicial del salto
        direccionSalto = 0.2; // Dirección hacia la derecha
    }
}

void trasladarCuadrado(int i)
{
    // Movimiento vertical y horizontal para simular un salto
    if (saltando)
    {
        ty += velocidadSalto;
        tx += direccionSalto * 2; // Movimiento horizontal según la dirección
        velocidadSalto -= 1; // Gravedad

        if (ty <= 0) // Cuando el cuadrado regrese al suelo
        {
            ty = 0;
            saltando = 0;
        }
    }

    // Manejar el estado de las teclas presionadas
    manejarTeclasPresionadas();
    glutPostRedisplay();
    glutTimerFunc(50, trasladarCuadrado, 1); // Llamar de nuevo a la función cada 50 ms
}

void manejarTeclado(unsigned char key, int x, int y)
{
    teclaApretada[key] = 1; // Registrar que la tecla está presionada
}

void manejarTecladoSoltar(unsigned char key, int x, int y)
{
    teclaApretada[key] = 0; // Registrar que la tecla fue soltada
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Modo 2D simplificado
    glutInitWindowSize(600, 400);
    glutInitWindowPosition(100, 200);
    glutCreateWindow("Cuadrado 2D - Saltos");
    gluLookAt(0, 0, 5, 
              0, 0, 0, 
              0, 1, 0);

    inicializar();

    glutDisplayFunc(graficar);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(manejarTeclado);
    glutKeyboardUpFunc(manejarTecladoSoltar);
    glutTimerFunc(50, trasladarCuadrado, 1);

    glutMainLoop();

    return 0;
}