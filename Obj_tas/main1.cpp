#include <GL/glut.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>

using namespace std;

struct Vertice {
    float x, y, z;
};

struct Cara {
    vector<int> vertices;
};

struct ModeloOBJ {
    vector<Vertice> vertices;
    vector<Cara> caras;

    void dibujar() const {
        glBegin(GL_TRIANGLES);
        for (const auto& cara : caras) {
            for (int idx : cara.vertices) {
                if (idx > 0 && idx <= (int)vertices.size()) {
                    const Vertice& v = vertices[idx - 1];
                    glVertex3f(v.x, v.y, v.z);
                }
            }
        }
        glEnd();
    }
};

// Estructura para almacenar coordenadas en todos los espacios del pipeline
struct CoordenadasPipeline {
    float objeto[3];      // Coordenadas de objeto (local)
    float mundo[3];       // Coordenadas de mundo (world)
    float vista[3];       // Coordenadas de vista (eye)
    float recorte[4];     // Coordenadas de recorte (clip) - homogéneas
    float ndc[3];         // Coordenadas NDC (normalized device)
    float ventana[3];     // Coordenadas de ventana (window/screen)
};

// Estructura para cada escalón
struct Escalon {
    float posicionX;      // Posición X en el mundo
    float posicionY;      // Posición Y en el mundo (altura)
    float posicionZ;      // Posición Z en el mundo
    float ancho;          // Ancho del escalón
    float alto;           // Alto del escalón
    float profundidad;    // Profundidad del escalón
    bool seleccionado;    // ¿Está seleccionado?
    int id;               // ID único para picking
    CoordenadasPipeline coordenadas;  // Coordenadas en todos los espacios
};

// ============= VARIABLES GLOBALES =============
map<string, ModeloOBJ> modelos;
int anchoPantalla = 1200;
int altoPantalla = 800;
bool pausado = false;

struct Camara {
    float radio;
    float anguloHorizontal;
    float anguloVertical;
};

Camara camaras[3];
int camaraActiva = 0;
int camaraSeleccionada = 0;

GLUquadric* quadricCamara = nullptr;

float tiempoAnimacion = 0.0f;
float velocidadAnimacion = 0.04f;
float factorInterpolacion = 0.0f;

// ============= VARIABLES PARA LA ESCALERA =============
vector<Escalon> escalones;  // Vector con los 6 escalones
int escalonSeleccionado = -1;  // Índice del escalón seleccionado (-1 = ninguno)
bool arrastrando = false;      // ¿Estamos arrastrando un escalón?
int mouseX = 0, mouseY = 0;    // Posición del mouse

// ============= FUNCIONES AUXILIARES PARA MATRICES =============

// Función para multiplicar matriz 4x4 por vector 4D
void multiplicarMatrizVector(const GLfloat* matriz, const float* vectorIn, float* vectorOut) {
    for (int i = 0; i < 4; i++) {
        vectorOut[i] = 0.0f;
        for (int j = 0; j < 4; j++) {
            vectorOut[i] += matriz[i + j * 4] * vectorIn[j];
        }
    }
}

// ============= FUNCIONES DE TRANSFORMACIÓN DE COORDENADAS =============

/*
*
 * Esta función calcula las coordenadas de un escalón en todos los espacios
 * del pipeline gráfico de OpenGL:
 * 
 * OBJETO → MUNDO → VISTA → RECORTE → NDC → VENTANA
 * 
 * Pipeline de transformación:
 * 1. Coordenadas de OBJETO: Sistema local del escalón (antes de transformaciones)
 * 2. Coordenadas de MUNDO: Después de aplicar la matriz MODEL (glTranslate, glScale)
 * 3. Coordenadas de VISTA: Después de aplicar la matriz VIEW (gluLookAt)
 * 4. Coordenadas de RECORTE: Después de aplicar la matriz PROJECTION (perspectiva)
 * 5. Coordenadas NDC: Después de división por w (perspectiva)
 * 6. Coordenadas de VENTANA: Después de transformación de viewport
 */

 void calcularCoordenadasPipeline(Escalon& escalon) {
    // 1. COORDENADAS DE OBJETO (espacio local del escalón)
    escalon.coordenadas.objeto[0] = 0.0f;
    escalon.coordenadas.objeto[1] = 0.0f;
    escalon.coordenadas.objeto[2] = 0.0f;
    
    // 2. COORDENADAS DE MUNDO
    escalon.coordenadas.mundo[0] = escalon.posicionX;
    escalon.coordenadas.mundo[1] = escalon.posicionY;
    escalon.coordenadas.mundo[2] = escalon.posicionZ;
    
    // Obtenemos las matrices actuales de OpenGL
    GLfloat matrizModelView[16];
    GLfloat matrizProjection[16];
    GLint viewport[4];
    
    glGetFloatv(GL_MODELVIEW_MATRIX, matrizModelView);
    glGetFloatv(GL_PROJECTION_MATRIX, matrizProjection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Preparamos el vector en coordenadas de mundo (homogéneas)
    float puntoMundo[4] = {
        escalon.posicionX,
        escalon.posicionY,
        escalon.posicionZ,
        1.0f  // w = 1 para un punto
    };
    
    // 3. COORDENADAS DE VISTA (Eye Coordinates)
    float puntoVista[4];
    multiplicarMatrizVector(matrizModelView, puntoMundo, puntoVista);
    
    escalon.coordenadas.vista[0] = puntoVista[0];
    escalon.coordenadas.vista[1] = puntoVista[1];
    escalon.coordenadas.vista[2] = puntoVista[2];
    
    // 4. COORDENADAS DE RECORTE (Clip Coordinates)
    float puntoRecorte[4];
    multiplicarMatrizVector(matrizProjection, puntoVista, puntoRecorte);
    
    escalon.coordenadas.recorte[0] = puntoRecorte[0];
    escalon.coordenadas.recorte[1] = puntoRecorte[1];
    escalon.coordenadas.recorte[2] = puntoRecorte[2];
    escalon.coordenadas.recorte[3] = puntoRecorte[3];
    
    // 5. COORDENADAS NDC (Normalized Device Coordinates)
    if (fabs(puntoRecorte[3]) > 0.0001f) {
        escalon.coordenadas.ndc[0] = puntoRecorte[0] / puntoRecorte[3];
        escalon.coordenadas.ndc[1] = puntoRecorte[1] / puntoRecorte[3];
        escalon.coordenadas.ndc[2] = puntoRecorte[2] / puntoRecorte[3];
    } else {
        escalon.coordenadas.ndc[0] = 0.0f;
        escalon.coordenadas.ndc[1] = 0.0f;
        escalon.coordenadas.ndc[2] = 0.0f;
    }
    
    // 6. COORDENADAS DE VENTANA (Window/Screen Coordinates)
    escalon.coordenadas.ventana[0] = 
        (escalon.coordenadas.ndc[0] + 1.0f) * viewport[2] / 2.0f + viewport[0];
    escalon.coordenadas.ventana[1] = 
        (escalon.coordenadas.ndc[1] + 1.0f) * viewport[3] / 2.0f + viewport[1];
    escalon.coordenadas.ventana[2] = 
        (escalon.coordenadas.ndc[2] + 1.0f) * 0.5f;
}

// ============= FUNCIONES DE INICIALIZACIÓN =============

/**
 * Inicializa los 6 escalones de la escalera
 */
void inicializarEscalera() {
    escalones.clear();
    
    float anchoBase = 2.0f;
    float altoEscalon = 0.5f;
    float profundidadBase = 2.0f;
    float posicionZInicial = 5.0f;
    
    for (int i = 0; i < 6; i++) {
        Escalon e;
        e.id = i;
        e.ancho = anchoBase;
        e.alto = altoEscalon;
        e.profundidad = profundidadBase;
        
        e.posicionX = 0.0f;
        e.posicionY = -0.2f + (i * altoEscalon) + (altoEscalon / 2.0f);
        e.posicionZ = posicionZInicial - (i * 1.0f);
        
        e.seleccionado = false;
        
        escalones.push_back(e);
    }
    
    cout << "Escalera inicializada con " << escalones.size() << " escalones." << endl;
}

// ============= FUNCIONES DE DIBUJO =============

void dibujarEscalon(const Escalon& e) {
    glPushMatrix();
    
    glTranslatef(e.posicionX, e.posicionY, e.posicionZ);
    glScalef(e.ancho, e.alto, e.profundidad);
    
    if (e.seleccionado) {
        glColor3f(1.0f, 1.0f, 0.0f);  // Amarillo
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);  // Gris
    }
    
    glutSolidCube(1.0f);
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glutWireCube(1.01f);
    
    glPopMatrix();
}

void dibujarEscalera() {
    for (const auto& escalon : escalones) {
        dibujarEscalon(escalon);
    }
}

void dibujarEscalonParaPicking(const Escalon& e) {
    glPushMatrix();
    
    glTranslatef(e.posicionX, e.posicionY, e.posicionZ);
    glScalef(e.ancho, e.alto, e.profundidad);
    
    // Color único: Canal azul = ID + 1
    float r = 0.0f;
    float g = 0.0f;
    float b = (e.id + 1) / 255.0f;
    
    glColor3f(r, g, b);
    glutSolidCube(1.0f);
    
    glPopMatrix();
}

// ============= FUNCIÓN AUXILIAR PARA CONFIGURAR VIEWPORT PRINCIPAL =============

/**
 * Configura el viewport y las matrices para la vista principal
 * CRÍTICO: Esto asegura que los cálculos de picking y movimiento usen las matrices correctas
 */
void configurarVistaPrincipal() {
    int anchoPrincipal = (anchoPantalla * 2) / 3;
    
    glViewport(0, 0, anchoPrincipal, altoPantalla);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)anchoPrincipal / (float)altoPantalla, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Configurar la cámara activa
    float radH = camaras[camaraActiva].anguloHorizontal * 3.14159f / 180.0f;
    float radV = camaras[camaraActiva].anguloVertical * 3.14159f / 180.0f;
    float camX = camaras[camaraActiva].radio * cos(radV) * cos(radH);
    float camY = camaras[camaraActiva].radio * sin(radV);
    float camZ = camaras[camaraActiva].radio * cos(radV) * sin(radH);
    
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
}

// ============= PICKING CON COLORES (CORREGIDO) =============

/**
 * Realiza el picking usando el método de color único
 * IMPORTANTE: Usa el mismo viewport y matrices que el renderizado principal
 */
int realizarPicking(int mouseX, int mouseY) {
    int anchoPrincipal = (anchoPantalla * 2) / 3;
    
    // Verificar si el clic está dentro de la vista principal
    if (mouseX >= anchoPrincipal) {
        cout << "Clic fuera del area de picking" << endl;
        return -1;
    }
    
    // Configurar el viewport y matrices correctas
    configurarVistaPrincipal();
    
    // Limpiar solo el buffer, NO cambiar glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Desactivar efectos para colores puros
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_DITHER);
    
    // Dibujar SOLO los escalones con colores únicos
    for (const auto& escalon : escalones) {
        dibujarEscalonParaPicking(escalon);
    }
    
    glFlush();
    glFinish();
    
    // Leer el pixel (invertir Y porque OpenGL usa origen abajo-izquierda)
    unsigned char pixel[3];
    glReadPixels(mouseX, altoPantalla - mouseY - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    
    // Convertir color a ID
    int idSeleccionado = pixel[2] - 1;
    
    cout << "Picking en (" << mouseX << ", " << mouseY << "): " 
         << "RGB(" << (int)pixel[0] << ", " << (int)pixel[1] << ", " << (int)pixel[2] << ") "
         << "-> ID: " << idSeleccionado << endl;
    
    if (idSeleccionado >= 0 && idSeleccionado < (int)escalones.size()) {
        return idSeleccionado;
    }
    
    return -1;
}

// ============= FUNCIONES AUXILIARES PARA MOVIMIENTO =============

/**
 * Calcula la nueva posición del escalón al arrastrar con el mouse
 * IMPORTANTE: Configura las matrices correctas antes del cálculo
 */
void moverEscalonConMouse(int mouseX, int mouseY) {
    if (escalonSeleccionado < 0 || escalonSeleccionado >= (int)escalones.size()) {
        return;
    }
    
    Escalon& e = escalones[escalonSeleccionado];
    
    configurarVistaPrincipal();
    
    GLint viewport[4];
    GLdouble matrizModelView[16];
    GLdouble matrizProjection[16];
    
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrizModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, matrizProjection);
    
    // Proyectar la posición actual del escalón a coordenadas de ventana
    GLdouble winX, winY, winZ;
    gluProject(e.posicionX, e.posicionY, e.posicionZ,
               matrizModelView, matrizProjection, viewport,
               &winX, &winY, &winZ);
    
    // Desproyectar la nueva posición del mouse manteniendo la profundidad Z
    GLdouble objX, objY, objZ;
    gluUnProject((GLdouble)mouseX, (GLdouble)(altoPantalla - mouseY), winZ,
                 matrizModelView, matrizProjection, viewport,
                 &objX, &objY, &objZ);
    
    // Actualizar posición (solo X y Z, mantenemos Y para altura)
    e.posicionX = (float)objX;
    e.posicionZ = (float)objZ;
}

// ============= FUNCIONES EXISTENTES =============

ModeloOBJ cargarOBJ(const string& archivo) {
    ModeloOBJ modelo;
    ifstream archivoEntrada(archivo);

    if (!archivoEntrada.is_open()) {
        cerr << "No se puede cargar: " << archivo << endl;
        return modelo;
    }

    string linea;
    while (getline(archivoEntrada, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        istringstream iss(linea);
        string tipo;
        iss >> tipo;

        if (tipo == "v") {
            Vertice v;
            iss >> v.x >> v.y >> v.z;
            modelo.vertices.push_back(v);
        }
        else if (tipo == "f") {
            Cara cara;
            string cadenaVertice;
            while (iss >> cadenaVertice) {
                int indiceVertice = stoi(cadenaVertice.substr(0, cadenaVertice.find('/')));
                cara.vertices.push_back(indiceVertice);
            }
            modelo.caras.push_back(cara);
        }
    }

    archivoEntrada.close();
    return modelo;
}

void cargarTodosLosModelos() {
    vector<string> archivos = {
        "freddy_torzo", "freddy_corbata", "freddy_pancita",
        "freddy_cadera", "freddy_pierna_derecha", "freddy_rodilla_derecha", 
        "freddy_pantorrilla_derecha", "freddy_pie_derecho",
        "freddy_pierna_izquierda", "freddy_rodilla_izquierda", 
        "freddy_pantorrilla_izquierda", "freddy_pie_izquierdo",
        "freddy_hombro_derecho", "freddy_brazo_derecho", "freddy_codo_derecho", 
        "freddy_antebrazo_derecho", "freddy_mano_derecha", "freddy_palma_derecha",
        "freddy_hombro_izquierdo", "freddy_brazo_izquierdo", "freddy_codo_izquierdo", 
        "freddy_antebrazo_izquierdo", "freddy_mano_izquierda", "freddy_palma_izquierda",
        "freddy_cabeza", "freddy_orejas", "freddy_ojos", "freddy_boca_inferior", 
        "freddy_dientes", "freddy_sombrero", "freddy_ocico"
    };

    for (const auto& archivo : archivos) {
        modelos[archivo] = cargarOBJ(archivo + ".obj");
        cout << "Cargado: " << archivo << endl;
    }
}

void dibujarModelo(const string& nombre) {
    auto it = modelos.find(nombre);
    if (it != modelos.end()) {
        it->second.dibujar();
    }
}

float interpolar(float a, float b, float t) {
    return a + (b - a) * t;
}

void dibujarEjes() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
    glLineWidth(1.0f);
}

void dibujarPiso() {
    glColor3f(0.2f, 0.6f, 0.2f);
    glPushMatrix();
        glTranslatef(0, -0.2, 0);
        glScalef(2, 0.01f, 2);
        glutSolidCube(10);
    glPopMatrix();
}

void dibujarModeloCamara() {
    glPushMatrix();
        glColor3f(0.4f, 0.4f, 0.4f);
        glutSolidCube(0.3);
        glPushMatrix();
            glColor3f(0.1f, 0.1f, 0.1f);
            glTranslatef(-0.25, 0, 0);
            glRotatef(90, 0, 1, 0);
            gluCylinder(quadricCamara, 0.08, 0.08, 0.2, 16, 16);
        glPopMatrix();
    glPopMatrix();
}

void dibujarFreddy() {
    glPushMatrix();
        glScalef(0.05f, 0.05f, 0.05f);
        glPushMatrix();
            glTranslatef(0.0f, interpolar(2.0f, -2.0f, factorInterpolacion), 0.0f);
            glRotatef(11, 1, 0, 0);
            glColor3f(0.4f, 0.25f, 0.1f);
            dibujarModelo("freddy_torzo");
            glColor3f(0.0f, 0.0f, 0.0f);
            dibujarModelo("freddy_corbata");
            glColor3f(0.9f, 0.75f, 0.4f);
            dibujarModelo("freddy_pancita");
            
            glPushMatrix();
                glTranslatef(0.0000f, 57.0200f, -0.0771f);
                glRotatef(-8, 1, 0, 0);
                glTranslatef(0.0000f, -57.0200f, 0.0771f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_cadera");

                glPushMatrix();
                    glTranslatef(-7.2192f, 49.6020f, 0.4607f);
                    glRotatef(interpolar(-50, 70, factorInterpolacion), 1, 0, 0);
                    glTranslatef(7.2192f, -49.6020f, -0.4607f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_pierna_derecha");

                    glPushMatrix();
                        glTranslatef(-8.2748f, 28.8432f, 7.4936f);
                        glRotatef(interpolar(40, -40, factorInterpolacion), 1, 0, 0);
                        glTranslatef(8.2748f, -28.8432f, -7.4936f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_rodilla_derecha");

                        glPushMatrix();
                            glTranslatef(-8.3349f, 26.6729f, 4.0948f);
                            glRotatef(30, 1, 0, 0);
                            glTranslatef(8.3349f, -26.6729f, -4.0948f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_pantorrilla_derecha");

                            glPushMatrix();
                                glTranslatef(-8.2733f, 8.2345f, 3.7685f);
                                glRotatef(interpolar(20, -20, factorInterpolacion), 1, 0, 0);
                                glTranslatef(8.2733f, -8.2345f, -3.7685f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_pie_derecho");
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(7.2256f, 49.5044f, 0.5372f);
                    glRotatef(interpolar(70, -50, factorInterpolacion), 1, 0, 0);
                    glTranslatef(-7.2256f, -49.5044f, -0.5372f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_pierna_izquierda");

                    glPushMatrix();
                        glTranslatef(8.2748f, 28.8432f, 7.4936f);
                        glRotatef(interpolar(-40, 40, factorInterpolacion), 1, 0, 0);
                        glTranslatef(-8.2748f, -28.8432f, -7.4936f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_rodilla_izquierda");

                        glPushMatrix();
                            glTranslatef(8.4381f, 26.6654f, 3.4338f);
                            glRotatef(30, 1, 0, 0);
                            glTranslatef(-8.4381f, -26.6654f, -3.4338f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_pantorrilla_izquierda");

                            glPushMatrix();
                                glTranslatef(8.2439f, 8.1688f, 3.7138f);
                                glRotatef(interpolar(20, -20, factorInterpolacion), 1, 0, 0);
                                glTranslatef(-8.2439f, -8.1688f, -3.7138f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_pie_izquierdo");
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(-13.3616f, 89.1309f, -0.7444f);
                glRotatef(interpolar(0, 15, factorInterpolacion), 0, 1, 0);
                glTranslatef(13.3616f, -89.1309f, 0.7444f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_hombro_derecho");

                glPushMatrix();
                    glTranslatef(-16.5272f, 85.7020f, 0.0000f);
                    glRotatef(interpolar(0, 70, factorInterpolacion), 0, 0, 1);
                    glTranslatef(16.5272f, -85.7020f, 0.0000f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_brazo_derecho");

                    glPushMatrix();
                        glTranslatef(-36.0000f, 85.6211f, 0.3000f);
                        glRotatef(interpolar(0, 60, factorInterpolacion), 0, 1, 0);
                        glTranslatef(36.0000f, -85.6211f, -0.3000f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_codo_derecho");

                        glPushMatrix();
                            glTranslatef(-36.9013f, 85.7020f, 0.0000f);
                            glRotatef(interpolar(0, 35, factorInterpolacion), 0, 1, 0);
                            glTranslatef(36.9013f, -85.7020f, 0.0000f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_antebrazo_derecho");

                            glPushMatrix();
                                glTranslatef(-51.5157f, 85.7020f, 0.0000f);
                                glRotatef(interpolar(0, 25, factorInterpolacion), 0, 1, 0);
                                glTranslatef(51.5157f, -85.7020f, 0.0000f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_mano_derecha");
                                glColor3f(0.9f, 0.75f, 0.4f);
                                dibujarModelo("freddy_palma_derecha");
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glTranslatef(11.9376f, 89.3683f, -0.7444f);
                glRotatef(interpolar(-15, 0, factorInterpolacion), 0, 1, 0);
                glTranslatef(-11.9376f, -89.3683f, 0.7444f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_hombro_izquierdo");

                glPushMatrix();
                    glTranslatef(16.8830f, 85.8832f, 0.0000f);
                    glRotatef(interpolar(-70, 0, factorInterpolacion), 0, 0, 1);
                    glTranslatef(-16.8830f, -85.8832f, 0.0000f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_brazo_izquierdo");

                    glPushMatrix();
                        glTranslatef(36.0000f, 85.6211f, 0.3000f);
                        glRotatef(interpolar(-60, 0, factorInterpolacion), 0, 1, 0);
                        glTranslatef(-36.0000f, -85.6211f, -0.3000f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_codo_izquierdo");

                        glPushMatrix();
                            glTranslatef(36.9013f, 85.7020f, 0.0000f);
                            glRotatef(interpolar(-35, 0, factorInterpolacion), 0, 1, 0);
                            glTranslatef(-36.9013f, -85.7020f, 0.0000f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_antebrazo_izquierdo");

                            glPushMatrix();
                                glTranslatef(51.5157f, 85.7020f, 0.0000f);
                                glRotatef(interpolar(-25, 0, factorInterpolacion), 0, 1, 0);
                                glTranslatef(-51.5157f, -85.7020f, 0.0000f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_mano_izquierda");
                                glColor3f(0.9f, 0.75f, 0.4f);
                                dibujarModelo("freddy_palma_izquierda");
                            glPopMatrix();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();

            glPushMatrix();
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_cabeza");

                glPushMatrix();
                    glTranslatef(0.0f, interpolar(2.0f, -2.0f, factorInterpolacion), 0.0f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_orejas");
                glPopMatrix();

                glPushMatrix();
                    glColor3f(0.9f, 0.75f, 0.4f);
                    dibujarModelo("freddy_ocico");
                glPopMatrix();

                glPushMatrix();
                    glColor3f(0.2f, 0.2f, 0.2f);
                    dibujarModelo("freddy_ojos");
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0.0f, interpolar(0.0f, -2.0f, factorInterpolacion), 0.0f);
                    glColor3f(0.9f, 0.75f, 0.4f);
                    dibujarModelo("freddy_boca_inferior");

                    glPushMatrix();
                        glColor3f(1.0f, 1.0f, 1.0f);
                        dibujarModelo("freddy_dientes");
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    glColor3f(0.0f, 0.0f, 0.0f);
                    dibujarModelo("freddy_sombrero");
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

// ============= RENDERIZADO DE TEXTO =============

void renderizarTexto(float x, float y, const string& texto, void* fuente = GLUT_BITMAP_HELVETICA_12) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, anchoPantalla, 0, altoPantalla);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glRasterPos2f(x, y);
    
    for (char c : texto) {
        glutBitmapCharacter(fuente, c);
    }
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void mostrarCoordenadasEnPantalla() {
    if (escalonSeleccionado < 0 || escalonSeleccionado >= (int)escalones.size()) {
        return;
    }
    
    const Escalon& e = escalones[escalonSeleccionado];
    const CoordenadasPipeline& coord = e.coordenadas;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    float x = 10.0f;
    float y = altoPantalla - 30.0f;
    float espaciado = 20.0f;
    
    renderizarTexto(x, y, "=== ESCALON " + to_string(e.id) + " SELECCIONADO ===", 
                    GLUT_BITMAP_HELVETICA_18);
    y -= espaciado * 1.5f;
    
    char buffer[256];
    
    renderizarTexto(x, y, "Coordenadas de OBJETO (local):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.2f, %.2f, %.2f)", 
             coord.objeto[0], coord.objeto[1], coord.objeto[2]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    renderizarTexto(x, y, "Coordenadas de MUNDO (world):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.2f, %.2f, %.2f)", 
             coord.mundo[0], coord.mundo[1], coord.mundo[2]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    renderizarTexto(x, y, "Coordenadas de VISTA (eye):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.2f, %.2f, %.2f)", 
             coord.vista[0], coord.vista[1], coord.vista[2]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    renderizarTexto(x, y, "Coordenadas de RECORTE (clip):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.2f, %.2f, %.2f, %.2f)", 
             coord.recorte[0], coord.recorte[1], coord.recorte[2], coord.recorte[3]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    renderizarTexto(x, y, "Coordenadas NDC (normalized):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.2f, %.2f, %.2f)", 
             coord.ndc[0], coord.ndc[1], coord.ndc[2]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    renderizarTexto(x, y, "Coordenadas de VENTANA (screen):");
    y -= espaciado;
    snprintf(buffer, sizeof(buffer), "  (%.0f, %.0f, %.2f)", 
             coord.ventana[0], coord.ventana[1], coord.ventana[2]);
    renderizarTexto(x, y, string(buffer));
    y -= espaciado * 1.5f;
    
    glColor3f(1.0f, 1.0f, 0.0f);
    renderizarTexto(x, y, "Arrastra con el mouse para mover");
}

// ============= CALLBACKS =============

void redimensionar(int w, int h) {
    anchoPantalla = w;
    altoPantalla = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case ' ':
            pausado = !pausado;
            break;
        case '1':
            camaraActiva = 0;
            glutPostRedisplay();
            break;
        case '2':
            camaraActiva = 1;
            glutPostRedisplay();
            break;
        case '3':
            camaraActiva = 2;
            glutPostRedisplay();
            break;
        case 'z':
        case 'Z':
            camaraSeleccionada = 0;
            glutPostRedisplay();
            break;
        case 'x':
        case 'X':
            camaraSeleccionada = 1;
            glutPostRedisplay();
            break;
        case 'c':
        case 'C':
            camaraSeleccionada = 2;
            glutPostRedisplay();
            break;
        case 'a':
        case 'A':
            camaras[camaraSeleccionada].anguloHorizontal -= 5.0f;
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            camaras[camaraSeleccionada].anguloHorizontal += 5.0f;
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            camaras[camaraSeleccionada].anguloVertical -= 5.0f;
            if (camaras[camaraSeleccionada].anguloVertical < -89.0f) {
                camaras[camaraSeleccionada].anguloVertical = -89.0f;
            }
            glutPostRedisplay();
            break;
        case 'e':
        case 'E':
            camaras[camaraSeleccionada].anguloVertical += 5.0f;
            if (camaras[camaraSeleccionada].anguloVertical > 89.0f) {
                camaras[camaraSeleccionada].anguloVertical = 89.0f;
            }
            glutPostRedisplay();
            break;
        case 'w':
        case 'W':
            camaras[camaraSeleccionada].radio -= 0.5f;
            if (camaras[camaraSeleccionada].radio < 1.0f) {
                camaras[camaraSeleccionada].radio = 1.0f;
            }
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            camaras[camaraSeleccionada].radio += 0.5f;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void teclasEspeciales(int tecla, int x, int y) {
    switch (tecla) {
    case GLUT_KEY_LEFT:
        camaras[camaraSeleccionada].anguloHorizontal -= 5.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        camaras[camaraSeleccionada].anguloHorizontal += 5.0f;
        glutPostRedisplay();
        break;
    case GLUT_KEY_UP:
        camaras[camaraSeleccionada].anguloVertical += 5.0f;
        if (camaras[camaraSeleccionada].anguloVertical > 89.0f) {
            camaras[camaraSeleccionada].anguloVertical = 89.0f;
        }
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        camaras[camaraSeleccionada].anguloVertical -= 5.0f;
        if (camaras[camaraSeleccionada].anguloVertical < -89.0f) {
            camaras[camaraSeleccionada].anguloVertical = -89.0f;
        }
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void mouse(int boton, int estado, int x, int y) {
    if (boton == 3 && estado == GLUT_UP) {
        camaras[camaraSeleccionada].radio -= 0.5f;
        if (camaras[camaraSeleccionada].radio < 1.0f) {
            camaras[camaraSeleccionada].radio = 1.0f;
        }
        glutPostRedisplay();
        return;
    }
    else if (boton == 4 && estado == GLUT_UP) {
        camaras[camaraSeleccionada].radio += 0.5f;
        glutPostRedisplay();
        return;
    }
    
    if (boton == GLUT_LEFT_BUTTON) {
        if (estado == GLUT_DOWN) {
            mouseX = x;
            mouseY = y;
            
            // Deseleccionar escalón anterior
            int idAnterior = escalonSeleccionado;
            if (idAnterior >= 0 && idAnterior < (int)escalones.size()) {
                escalones[idAnterior].seleccionado = false;
            }
            
            // Realizar picking
            escalonSeleccionado = realizarPicking(x, y);
            
            // Si se seleccionó un escalón
            if (escalonSeleccionado >= 0) {
                escalones[escalonSeleccionado].seleccionado = true;
                arrastrando = true;
                
                // Configurar matrices antes de calcular coordenadas
                configurarVistaPrincipal();
                calcularCoordenadasPipeline(escalones[escalonSeleccionado]);
                
                cout << "Escalon " << escalonSeleccionado << " seleccionado en posicion: ("
                     << escalones[escalonSeleccionado].posicionX << ", "
                     << escalones[escalonSeleccionado].posicionY << ", "
                     << escalones[escalonSeleccionado].posicionZ << ")" << endl;
            } else {
                cout << "No se selecciono ningun escalon." << endl;
            }
            
            // Forzar redibujado completo
            glutPostRedisplay();
        }
        else if (estado == GLUT_UP) {
            arrastrando = false;
        }
    }
}

void movimientoMouse(int x, int y) {
    if (arrastrando && escalonSeleccionado >= 0) {
        // Mover el escalón (ya configura las matrices internamente)
        moverEscalonConMouse(x, y);
        
        // Actualizar coordenadas del pipeline
        configurarVistaPrincipal();
        calcularCoordenadasPipeline(escalones[escalonSeleccionado]);
        
        // Forzar redibujado
        glutPostRedisplay();
    }
    
    mouseX = x;
    mouseY = y;
}

void display() {
    // ========== VISTA PRINCIPAL ==========
    int anchoPrincipal = (anchoPantalla * 2) / 3;
    
    glViewport(0, 0, anchoPrincipal, altoPantalla);
    glScissor(0, 0, anchoPrincipal, altoPantalla);
    glEnable(GL_SCISSOR_TEST);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)anchoPrincipal / (float)altoPantalla, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float radH = camaras[camaraActiva].anguloHorizontal * 3.14159f / 180.0f;
    float radV = camaras[camaraActiva].anguloVertical * 3.14159f / 180.0f;
    float camX = camaras[camaraActiva].radio * cos(radV) * cos(radH);
    float camY = camaras[camaraActiva].radio * sin(radV);
    float camZ = camaras[camaraActiva].radio * cos(radV) * sin(radH);
    
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
    
    dibujarEjes();
    dibujarPiso();
    dibujarFreddy();
    dibujarEscalera();
    
    if (escalonSeleccionado >= 0 && escalonSeleccionado < (int)escalones.size()) {
        calcularCoordenadasPipeline(escalones[escalonSeleccionado]);
    }
    
    for (int i = 0; i < 3; i++) {
        if (i != camaraActiva) {
            glPushMatrix();
            float rH = camaras[i].anguloHorizontal * 3.14159f / 180.0f;
            float rV = camaras[i].anguloVertical * 3.14159f / 180.0f;
            float posX = camaras[i].radio * cos(rV) * cos(rH);
            float posY = camaras[i].radio * sin(rV);
            float posZ = camaras[i].radio * cos(rV) * sin(rH);
            
            glTranslatef(posX, posY, posZ);
            glRotatef(-camaras[i].anguloHorizontal, 0, 1, 0);
            
            if (i == camaraSeleccionada) {
                glColor3f(1.0f, 1.0f, 0.0f);
            } else {
                glColor3f(0.4f, 0.4f, 0.4f);
            }
            dibujarModeloCamara();
            glPopMatrix();
        }
    }
    
    mostrarCoordenadasEnPantalla();
    
    // ========== VISTAS PEQUEÑAS ==========
    int anchoVistaPequeña = anchoPantalla / 3;
    int altoVistaPequeña = altoPantalla / 3;
    float aspectPequeño = (float)anchoVistaPequeña / (float)altoVistaPequeña;
    
    for (int i = 0; i < 3; i++) {
        int x = anchoPrincipal;
        int y = altoPantalla - (i + 1) * altoVistaPequeña;
        
        glViewport(x, y, anchoVistaPequeña, altoVistaPequeña);
        glScissor(x, y, anchoVistaPequeña, altoVistaPequeña);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, aspectPequeño, 0.1f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        float rH = camaras[i].anguloHorizontal * 3.14159f / 180.0f;
        float rV = camaras[i].anguloVertical * 3.14159f / 180.0f;
        float cX = camaras[i].radio * cos(rV) * cos(rH);
        float cY = camaras[i].radio * sin(rV);
        float cZ = camaras[i].radio * cos(rV) * sin(rH);
        
        gluLookAt(cX, cY, cZ, 0, 0, 0, 0, 1, 0);
        
        dibujarEjes();
        dibujarPiso();
        dibujarFreddy();
        dibujarEscalera();
        
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        if (i == camaraActiva && i == camaraSeleccionada) {
            glColor3f(0.0f, 1.0f, 0.0f);
        } else if (i == camaraActiva) {
            glColor3f(0.0f, 0.5f, 1.0f);
        } else if (i == camaraSeleccionada) {
            glColor3f(1.0f, 1.0f, 0.0f);
        } else {
            glColor3f(0.3f, 0.3f, 0.3f);
        }
        
        glLineWidth(3.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(0.01f, 0.01f);
            glVertex2f(0.99f, 0.01f);
            glVertex2f(0.99f, 0.99f);
            glVertex2f(0.01f, 0.99f);
        glEnd();
        glLineWidth(1.0f);
        
        glEnable(GL_DEPTH_TEST);
    }
    
    glDisable(GL_SCISSOR_TEST);
    glutSwapBuffers();
}

void temporizador(int valor) {
    if (!pausado) {
        tiempoAnimacion += velocidadAnimacion;
        if (tiempoAnimacion >= 2.0f) {
            tiempoAnimacion = 0.0f;
        }
        if (tiempoAnimacion < 1.0f) {
            factorInterpolacion = tiempoAnimacion;
        } else {
            factorInterpolacion = 2.0f - tiempoAnimacion;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, temporizador, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(anchoPantalla, altoPantalla);
    glutCreateWindow("Freddy con Escalera Interactiva");

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    quadricCamara = gluNewQuadric();

    camaras[0].radio = 15.0f;
    camaras[0].anguloHorizontal = 0.0f;
    camaras[0].anguloVertical = 20.0f;

    camaras[1].radio = 15.0f;
    camaras[1].anguloHorizontal = 90.0f;
    camaras[1].anguloVertical = 0.0f;

    camaras[2].radio = 15.8f;
    camaras[2].anguloHorizontal = 45.0f;
    camaras[2].anguloVertical = 45.0f;

    cout << "Cargando modelos de Freddy..." << endl;
    cargarTodosLosModelos();
    cout << "Modelos cargados!" << endl;
    
    cout << "Inicializando escalera..." << endl;
    inicializarEscalera();
    cout << "Escalera inicializada!" << endl;

    glutDisplayFunc(display);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciales);
    glutMouseFunc(mouse);
    glutMotionFunc(movimientoMouse);
    glutTimerFunc(16, temporizador, 0);

    cout << "\n=== CONTROLES ===" << endl;
    cout << "ESCALERA:" << endl;
    cout << "  - Clic izquierdo: Seleccionar escalon" << endl;
    cout << "  - Arrastrar: Mover escalon seleccionado" << endl;
    cout << "\nCAMARAS:" << endl;
    cout << "  - 1/2/3: Cambiar camara activa" << endl;
    cout << "  - Z/X/C: Seleccionar camara a editar" << endl;
    cout << "  - W/S: Zoom in/out" << endl;
    cout << "  - A/D: Rotar horizontalmente" << endl;
    cout << "  - Q/E: Rotar verticalmente" << endl;
    cout << "  - Flechas: Rotar camara" << endl;
    cout << "  - Espacio: Pausar animacion" << endl;
    cout << "  - ESC: Salir" << endl;

    glutMainLoop();

    return 0;
}
