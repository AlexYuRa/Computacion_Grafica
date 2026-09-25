#include <GL/glut.h>
#include <cmath>

// Variables para controlar la rotación y posición de la tetera
float rotacionX = 0.0f;
float rotacionY = 0.0f;
float posicionX = 0.0f;
float posicionY = 0.0f;
float posicionZ = 0.0f;

float esc = 1.5f; // Variable para controlar la escala

// Variables para la física de caída
float velocidadY = 0.0f;     // Velocidad de caída en Y
float velocidadX = 0.0f;     // Velocidad de caída en X
float velocidadZ = 0.0f;     // Velocidad de caída en Z
float gravedad = -0.3f;      // Fuerza de gravedad
bool cayendo = false;        // Estado de caída
bool enElPiso = true;        // Si la tetera está sobre el piso

// Límites del piso
const float LIMITE_PISO = 25.0f;
const float ALTURA_PISO = -5.0f;
const float RADIO_TETERA = 3.0f; // Radio aproximado de la tetera

void Inicializador()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Cor de fundo blanca
    glEnable(GL_DEPTH_TEST);          // Habilita o teste de profundidade
}

void graficarejes()
{
    glBegin(GL_LINES);
    // Eje X (rojo)
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(30.0, 0.0, 0.0);
    // Eje Y (verde)
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);
    // Eje Z (azul)
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -30.0);
    glVertex3f(0.0, 0.0, 30.0);
    glEnd();
}

void dibujarPiso()
{
    // Piso principal (plano sólido)
    glColor3f(0.3, 0.3, 0.3); // Color gris oscuro
    glBegin(GL_QUADS);
    glVertex3f(-LIMITE_PISO, ALTURA_PISO, -LIMITE_PISO);
    glVertex3f(LIMITE_PISO, ALTURA_PISO, -LIMITE_PISO);
    glVertex3f(LIMITE_PISO, ALTURA_PISO, LIMITE_PISO);
    glVertex3f(-LIMITE_PISO, ALTURA_PISO, LIMITE_PISO);
    glEnd();

    // Líneas de la cuadrícula para el piso
    glColor3f(0.5, 0.5, 0.5); // Color gris más claro para las líneas
    glBegin(GL_LINES);

    // Líneas paralelas al eje X
    for (int i = -25; i <= 25; i += 5) {
        glVertex3f(-25.0, -4.9, i);
        glVertex3f(25.0, -4.9, i);
    }

    // Líneas paralelas al eje Z
    for (int i = -25; i <= 25; i += 5) {
        glVertex3f(i, -4.9, -25.0);
        glVertex3f(i, -4.9, 25.0);
    }

    glEnd();
}

// Función para verificar si la tetera está fuera del piso
bool fueraDelPiso()
{
    float radioEscalado = RADIO_TETERA * esc;
    return (posicionX + radioEscalado > LIMITE_PISO ||
        posicionX - radioEscalado < -LIMITE_PISO ||
        posicionZ + radioEscalado > LIMITE_PISO ||
        posicionZ - radioEscalado < -LIMITE_PISO);
}

// Función para aplicar física de caída
void aplicarFisica()
{
    if (fueraDelPiso() && !cayendo) {
        // Comenzar a caer
        cayendo = true;
        enElPiso = false;

        // Dar un pequeño impulso horizontal aleatorio
        velocidadX = (posicionX > 0) ? 0.2f : -0.2f;
        velocidadZ = (posicionZ > 0) ? 0.2f : -0.2f;
    }

    if (cayendo) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Actualizar posiciones
        posicionY += velocidadY;
        posicionX += velocidadX;
        posicionZ += velocidadZ;

        // Agregar rotación mientras cae para efecto dramático
        rotacionX += 3.0f;
        rotacionY += 2.0f;

        // Si cae muy bajo, resetear posición
        if (posicionY < -50.0f) {
            posicionX = 0.0f;
            posicionY = 0.0f;
            posicionZ = 0.0f;
            velocidadX = 0.0f;
            velocidadY = 0.0f;
            velocidadZ = 0.0f;
            rotacionX = 0.0f;
            rotacionY = 0.0f;
            cayendo = false;
            enElPiso = true;
        }
    }
    else if (enElPiso) {
        // Mantener la tetera sobre el piso
        if (posicionY < ALTURA_PISO + RADIO_TETERA * esc) {
            posicionY = ALTURA_PISO + RADIO_TETERA * esc;
        }
    }
}

// Función para manejar las teclas del teclado
void teclado(unsigned char key, int x, int y)
{
    // Solo permitir movimiento si no está cayendo
    if (!cayendo) {
        switch (key) {
        case 'w': case 'W':
            posicionY += 1.0f; // Mover hacia arriba
            break;
        case 's': case 'S':
            posicionY -= 1.0f; // Mover hacia abajo
            break;
        case 'a': case 'A':
            posicionX -= 1.0f; // Mover hacia la izquierda
            break;
        case 'd': case 'D':
            posicionX += 1.0f; // Mover hacia la derecha
            break;
        case 'z': case 'Z':
            posicionZ -= 1.0f; // Mover hacia adelante
            break;
        case 'x': case 'X':
            posicionZ += 1.0f; // Mover hacia atrás
            break;
        case '+': case '=':
            if (esc < 3.0f) esc += 0.1f; // Aumentar escala (máximo 3.0)
            break;
        case '-': case '_':
            if (esc > 0.2f) esc -= 0.1f; // Disminuir escala (mínimo 0.2)
            break;
        case 'q': case 'Q':
            rotacionY -= 5.0f; // Rotar hacia la izquierda
            break;
        case 'e': case 'E':
            rotacionY += 5.0f; // Rotar hacia la derecha
            break;
        case 'r': case 'R':
            rotacionX -= 5.0f; // Rotar hacia arriba
            break;
        case 'f': case 'F':
            rotacionX += 5.0f; // Rotar hacia abajo
            break;
        }
    }

    switch (key) {
    case ' ': // Espacebar para resetear
        posicionX = 0.0f;
        posicionY = 0.0f;
        posicionZ = 0.0f;
        velocidadX = 0.0f;
        velocidadY = 0.0f;
        velocidadZ = 0.0f;
        rotacionX = 0.0f;
        rotacionY = 0.0f;
        cayendo = false;
        enElPiso = true;
        esc = 1.5f;
        break;
    case 27: // Tecla ESC
        exit(0);
        break;
    }
    glutPostRedisplay(); // Forzar redibujado
}

// Función de animación continua
void actualizar(int valor)
{
    aplicarFisica();
    glutPostRedisplay();
    glutTimerFunc(16, actualizar, 0); // ~60 FPS
}

void graficar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a tela e o buffer de profundidade
    glMatrixMode(GL_MODELVIEW);                     // Seleciona a matriz de modelagem
    glLoadIdentity();     // Reseta a matriz de modelagem
    gluLookAt(30.0, 30.0, 30.0,  // Posição da câmera
        0.0, 0.0, 0.0,  // Ponto para onde a câmera está olhando
        0.0, 1.0, 0.0); // Vetor "up" da câmera

    graficarejes();
    dibujarPiso();

    // Aplicar transformaciones a la tetera
    glPushMatrix(); // Guardar la matriz actual

    // Aplicar traslación
    glTranslatef(posicionX, posicionY, posicionZ);

    // Aplicar rotaciones
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f); // Rotación en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f); // Rotación en Y

    // Aplicar escalamiento
    glScalef(esc, esc, esc); // Escalar uniformemente en todos los ejes

    // Cambiar color según el estado
    if (cayendo) {
        glColor3f(1.0, 0.0, 0.0); // Rojo cuando está cayendo
    }
    else if (fueraDelPiso()) {
        glColor3f(1.0, 1.0, 0.0); // Amarillo cuando está en el borde
    }
    else {
        glColor3f(1.0, 0.5, 0.0); // Naranja normal
    }

    glutWireTeapot(RADIO_TETERA); // Tetera

    glPopMatrix(); // Restaurar la matriz

    glFlush();        // Ejecuta los comandos OpenGL
    glutSwapBuffers();        // Troca os buffers (duplo buffer)
}

void redimensionar(int w, int h)
{
    glViewport(0, 0, w, h); // Define a viewport para cobrir toda a janela
    glMatrixMode(GL_PROJECTION); // Seleciona a matriz de projeção
    glLoadIdentity();            // Reseta a matriz de projeção
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0); // Define a perspectiva
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o modo de exibição
    glutInitWindowSize(800, 600);                              // Define o tamanho da janela
    glutCreateWindow("Tetera 3D con Fisica de Caida");        // Cria a janela com um título

    Inicializador();                                           // Chama a função de inicialização

    glMatrixMode(GL_MODELVIEW);                                // Seleciona a matriz de modelagem
    glLoadIdentity();                                          // Reseta a matriz de modelagem

    glutDisplayFunc(graficar);                                 // Registra a função de exibição
    glutReshapeFunc(redimensionar);                            // Registra a função de redimensionamento
    glutKeyboardFunc(teclado);                                 // Registra la función de teclado
    glutTimerFunc(0, actualizar, 0);                          // Inicia el timer de animación

    glutMainLoop();                                            // Entra no loop principal do GLUT
    return 0;
}