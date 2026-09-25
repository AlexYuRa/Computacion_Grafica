#include <GL/glut.h>
#include <cmath>
#include <cstdlib>  // Para rand()
#include <iostream> // Para std::cout
#include <string>   // Para std::string

// Variables para controlar la rotación y posición de la tetera
float rotacionX = 0.0f;
float rotacionY = 0.0f;
float rotacionZ = 0.0f;  // Nueva rotación en eje Z
float posicionX = 0.0f;
float posicionY = 0.0f;
float posicionZ = 0.0f;

float esc = 1.5f; // Variable para controlar la escala

// Variables para la física de caída (más lenta)
float velocidadY = 0.0f;     // Velocidad de caída en Y
float velocidadX = 0.0f;     // Velocidad de caída en X
float velocidadZ = 0.0f;     // Velocidad de caída en Z
float gravedad = -0.2f;      // Gravedad más suave (era -0.8f)
bool cayendo = false;        // Estado de caída
bool enElPiso = true;        // Si la tetera está sobre el piso

// Variables para efectos de caída
float escalaInicial = 1.5f;  // Escala inicial cuando comienza a caer
float velocidadRotacionX = 0.0f; // Velocidad de rotación en X durante caída
float velocidadRotacionY = 0.0f; // Velocidad de rotación en Y durante caída
float velocidadRotacionZ = 0.0f; // Velocidad de rotación en Z durante caída

// Variables para las frases
bool fraseMostrada = false;  // Para mostrar la frase solo una vez por caída
std::string frases[] = {
    "¡Ayudaaa, me caigo!",
    "¡No quiero caer!",
    "¡Esto no estaba en mis planes!",
    "¡Maldita gravedad!",
    "¡Hasta la vista, baby!",
    "¡Que alguien me salve!",
    "¡Esta no es mi mejor cara!",
    "¡Houston, tenemos un problema!"
};
const int NUM_FRASES = 8;

// Límites del piso
const float LIMITE_PISO = 25.0f;
const float ALTURA_PISO = -5.0f;
const float RADIO_TETERA = 3.0f; // Radio aproximado de la tetera

void Inicializador()
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // Cor de fundo blanca
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

// Función para mostrar una frase aleatoria
void mostrarFrase()
{
    if (!fraseMostrada) {
        int indice = rand() % NUM_FRASES;
        std::cout << "\n🫖 TETERA: " << frases[indice] << std::endl;
        fraseMostrada = true;
    }
}

// Función para aplicar física de caída (más lenta)
void aplicarFisica()
{
    if (fueraDelPiso() && !cayendo) {
        // Comenzar a caer
        cayendo = true;
        enElPiso = false;
        escalaInicial = esc; // Guardar escala inicial
        fraseMostrada = false; // Resetear para mostrar nueva frase

        // Dar un impulso horizontal más suave
        velocidadX = (posicionX > 0) ? 0.2f : -0.2f;  // Reducido de 0.5f
        velocidadZ = (posicionZ > 0) ? 0.2f : -0.2f;  // Reducido de 0.5f

        // Velocidades de rotación más suaves
        velocidadRotacionX = (rand() % 10 - 5) * 0.8f; // Entre -4 y 4 (era -20 a 20)
        velocidadRotacionY = (rand() % 10 - 5) * 0.8f;
        velocidadRotacionZ = (rand() % 10 - 5) * 0.8f;

        // Mostrar frase al comenzar a caer
        mostrarFrase();
    }

    if (cayendo) {
        // Aplicar gravedad más suave
        velocidadY += gravedad;

        // Actualizar posiciones
        posicionY += velocidadY;
        posicionX += velocidadX * 0.8f;  // Movimiento horizontal más lento
        posicionZ += velocidadZ * 0.8f;  // Movimiento horizontal más lento

        // Rotaciones más suaves en todos los ejes
        rotacionX += velocidadRotacionX;
        rotacionY += velocidadRotacionY;
        rotacionZ += velocidadRotacionZ;

        // Acelerar las rotaciones muy lentamente (era 1.02f)
        velocidadRotacionX *= 1.005f;
        velocidadRotacionY *= 1.005f;
        velocidadRotacionZ *= 1.005f;

        // Efecto de encogimiento más lento
        float factorEncogimiento = 0.998f; // Se encoge 0.2% cada frame (era 0.5%)
        esc *= factorEncogimiento;
        if (esc < 0.1f) esc = 0.1f; // Mínimo para que no desaparezca completamente

        // Si cae muy bajo, resetear posición
        if (posicionY < -50.0f) { // Menos profundo que antes (-80.0f)
            std::cout << "💥 ¡La tetera ha tocado fondo! Regresando al origen..." << std::endl;
            posicionX = 0.0f;
            posicionY = 0.0f;
            posicionZ = 0.0f;
            velocidadX = 0.0f;
            velocidadY = 0.0f;
            velocidadZ = 0.0f;
            rotacionX = 0.0f;
            rotacionY = 0.0f;
            rotacionZ = 0.0f;
            velocidadRotacionX = 0.0f;
            velocidadRotacionY = 0.0f;
            velocidadRotacionZ = 0.0f;
            cayendo = false;
            enElPiso = true;
            esc = escalaInicial; // Restaurar escala original
            fraseMostrada = false; // Permitir nueva frase en próxima caída
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
        std::cout << "🔄 Reiniciando tetera..." << std::endl;
        posicionX = 0.0f;
        posicionY = 0.0f;
        posicionZ = 0.0f;
        velocidadX = 0.0f;
        velocidadY = 0.0f;
        velocidadZ = 0.0f;
        rotacionX = 0.0f;
        rotacionY = 0.0f;
        rotacionZ = 0.0f;
        velocidadRotacionX = 0.0f;
        velocidadRotacionY = 0.0f;
        velocidadRotacionZ = 0.0f;
        cayendo = false;
        enElPiso = true;
        esc = 1.5f;
        fraseMostrada = false;
        break;
    case 27: // Tecla ESC
        std::cout << "👋 ¡Hasta luego!" << std::endl;
        exit(0);
        break;
    }
    glutPostRedisplay(); // Forzar redibujado
}

// Función de animación continua (más lenta)
void actualizar(int valor)
{
    aplicarFisica();
    glutPostRedisplay();
    glutTimerFunc(25, actualizar, 0); // ~50 FPS (era 16ms = 60 FPS)
}

// Función para dibujar texto 3D
void dibujarTexto3D(const char* texto, float x, float y, float z)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.02f, 0.02f, 0.02f); // Escalar el texto

    // Color del texto (blanco brillante)
    glColor3f(1.0f, 1.0f, 1.0f);

    // Dibujar cada carácter
    for (const char* c = texto; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }

    glPopMatrix();
}

void graficar()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a tela e o buffer de profundidade
    glMatrixMode(GL_MODELVIEW);                     // Seleciona a matriz de modelagem
    glLoadIdentity();     // Reseta a matriz de modelagem
    gluLookAt(50.0, 50.0, 50.0,  // Posição da câmera
        0.0, 0.0, 0.0,  // Ponto para onde a câmera está olhando
        0.0, 1.0, 0.0); // Vetor "up" da câmera

    graficarejes();
    dibujarPiso();

    // Aplicar transformaciones a la tetera
    glPushMatrix(); // Guardar la matriz actual

    // Aplicar traslación
    glTranslatef(posicionX, posicionY, posicionZ);

    // Aplicar rotaciones en los tres ejes
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f); // Rotación en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f); // Rotación en Y
    glRotatef(rotacionZ, 0.0f, 0.0f, 1.0f); // Rotación en Z

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

    // Mostrar "Tren ucv" en 3D cuando está cayendo
    if (cayendo) {
        // Texto que sigue a la tetera con un offset
        float offsetX = posicionX + 8.0f;
        float offsetY = posicionY + 5.0f;
        float offsetZ = posicionZ;

        dibujarTexto3D("Tren UCV", offsetX, offsetY, offsetZ);

        // Texto adicional que rota alrededor de la tetera
        float tiempo = glutGet(GLUT_ELAPSED_TIME) * 0.01f; // Tiempo en segundos
        float radioTexto = 12.0f;
        float xRotante = posicionX + cos(tiempo * 2.0f) * radioTexto;
        float zRotante = posicionZ + sin(tiempo * 2.0f) * radioTexto;

        // dibujarTexto3D("Tren UCV", xRotante, posicionY + 3.0f, zRotante);
    }

    glFlush();        // Ejecuta los comandos OpenGL
    glutSwapBuffers();        // Troca os buffers (duplo buffer)
}

void redimensionar(int w, int h)
{
    glViewport(0, 0, w, h); // Define a viewport para cobrir toda a janela
    glMatrixMode(GL_PROJECTION); // Seleciona a matriz de projeção
    glLoadIdentity();            // Reseta a matriz de projeção
    gluPerspective(60.0, (float)w / (float)h, 0.1, 200.0); // Define a perspectiva
}

int main(int argc, char** argv)
{
    // Inicializar semilla para números aleatorios
    srand(time(NULL));

    std::cout << "🫖 ¡Bienvenido al simulador de tetera con física!" << std::endl;
    std::cout << "📝 Controles:" << std::endl;
    std::cout << "   WASD: Mover tetera" << std::endl;
    std::cout << "   ZX: Mover adelante/atrás" << std::endl;
    std::cout << "   QE: Rotar izquierda/derecha" << std::endl;
    std::cout << "   RF: Rotar arriba/abajo" << std::endl;
    std::cout << "   +/-: Cambiar tamaño" << std::endl;
    std::cout << "   ESPACIO: Resetear" << std::endl;
    std::cout << "   ESC: Salir" << std::endl;
    std::cout << "💡 ¡Mueve la tetera fuera del piso para ver qué pasa!" << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o modo de exibição
    glutInitWindowSize(800, 600);                              // Define o tamanho da janela
    glutCreateWindow("Tetera 3D con Fisica de Caida Lenta");  // Cria a janela com um título

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








/*
#include <GL/glut.h>

// Variables para controlar la rotación y posición de la tetera
float rotacionX = 0.0f;
float rotacionY = 0.0f;
float posicionX = 0.0f;
float posicionY = 0.0f;

float esc = 1.5f; // Variable para controlar la escala

void Inicializador()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // Cor de fundo preta
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
    // Piso principal (plano sólido) - más pequeño y más cerca
    glColor3f(0.2, 0.8, 0.2); // Color verde claro para mejor visibilidad
    glBegin(GL_QUADS);
    glVertex3f(-15.0, -5.0, -15.0);
    glVertex3f(15.0, -5.0, -15.0);
    glVertex3f(15.0, -5.0, 15.0);
    glVertex3f(-15.0, -5.0, 15.0);
    glEnd();

    // Líneas de la cuadrícula para el piso
    glColor3f(0.0, 0.4, 0.0); // Color verde más oscuro para las líneas
    glBegin(GL_LINES);

    // Líneas paralelas al eje X
    for (int i = -15; i <= 15; i += 3) {
        glVertex3f(-15.0, -4.9, i);
        glVertex3f(15.0, -4.9, i);
    }

    // Líneas paralelas al eje Z
    for (int i = -15; i <= 15; i += 3) {
        glVertex3f(i, -4.9, -15.0);
        glVertex3f(i, -4.9, 15.0);
    }

    glEnd();
}


// Función para manejar las teclas del teclado
void teclado(unsigned char key, int x, int y)
{
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
        case '+': case '=':
            if (esc < 3.0f) esc += 0.4f; // Aumentar escala (máximo 3.0)
            break;
        case '-': case '_':
            if (esc > 0.2f) esc -= 0.4f; // Disminuir escala (mínimo 0.2)
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
        case 27: // Tecla ESC
            exit(0);
            break;
    }
    glutPostRedisplay(); // Forzar redibujado
}


void graficar ()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa a tela e o buffer de profundidade
    glMatrixMode(GL_MODELVIEW);                     // Seleciona a matriz de modelagem
    glLoadIdentity();     // Reseta a matriz de modelagem
    gluLookAt(45.0, 45.0, 45.0,  // Posição da câmera
              0.0, 0.0, 0.0,  // Ponto para onde a câmera está olhando
              0.0, 1.0, 0.0); // Vetor "up" da câmera
    
    // Dibujar primero el piso (objetos sólidos primero)
    //dibujarPiso();
    
    // Luego los ejes (líneas)
    graficarejes();
    dibujarPiso();
    // Aplicar transformaciones a la tetera
    glPushMatrix(); // Guardar la matriz actual
    
    // Aplicar traslación
    glTranslatef(posicionX, posicionY, 0.0f);
    
    // Aplicar rotaciones
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f); // Rotación en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f); // Rotación en Y
    
    // Aplicar escalamiento
    glScalef(esc, esc, esc); // Escalar uniformemente en todos los ejes
    // Dibujar la tetera
    glColor3f(1.0, 0.5, 0.0); // Color naranja
    glutWireTeapot(3.0); // Tetera de tamaño 3
    
    glPopMatrix(); // Restaurar la matriz

    glFlush();        // Ejecuta los comandos OpenGL
    glutSwapBuffers();        // Troca os buffers (duplo buffer)
}

void redimensionar (int w, int h)
{
    glViewport(0, 0, w, h); // Define a viewport para cobrir toda a janela
    glMatrixMode(GL_PROJECTION); // Seleciona a matriz de projeção
    glLoadIdentity();            // Reseta a matriz de projeção
    gluPerspective(60.0, (float)w / (float)h, 0.1, 200.0); // Campo de visión más amplio y plano lejano extendido
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o modo de exibição
    glutInitWindowSize(800, 600);                              // Define o tamanho da janela
    glutCreateWindow("Tetera 3D Interactiva");                    // Cria a janela com um título

    Inicializador();                                           // Chama a função de inicialização

    glMatrixMode(GL_MODELVIEW);                                // Seleciona a matriz de modelagem
    glLoadIdentity();                                          // Reseta a matriz de modelagem

    glutDisplayFunc(graficar);                                 // Registra a função de exibição
    glutReshapeFunc(redimensionar);                            // Registra a função de redimensionamento
    glutKeyboardFunc(teclado);                                 // Registra la función de teclado

    glutMainLoop();                                            // Entra no loop principal do GLUT
    return 0;
}
*/