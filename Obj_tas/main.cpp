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
        "freddy_cadera", "freddy_pierna_derecha", "freddy_rodilla_derecha", "freddy_pantorrilla_derecha", "freddy_pie_derecho",
        "freddy_pierna_izquierda", "freddy_rodilla_izquierda", "freddy_pantorrilla_izquierda", "freddy_pie_izquierdo",
        "freddy_hombro_derecho", "freddy_brazo_derecho", "freddy_codo_derecho", "freddy_antebrazo_derecho", "freddy_mano_derecha",
        "freddy_palma_derecha",
        "freddy_hombro_izquierdo", "freddy_brazo_izquierdo", "freddy_codo_izquierdo", "freddy_antebrazo_izquierdo", "freddy_mano_izquierda",
        "freddy_palma_izquierda",
        "freddy_cabeza", "freddy_orejas", "freddy_ojos", "freddy_boca_inferior", "freddy_dientes", "freddy_sombrero", "freddy_ocico"
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
        // torzo (papi)
        glPushMatrix();
            // Saltito del torso durante la carrera
            glTranslatef(0.0f, interpolar(2.0f, -2.0f, factorInterpolacion), 0.0f);
            glRotatef(11, 1, 0, 0);
            // Compuesto de torso
            glColor3f(0.4f, 0.25f, 0.1f);  // Marron oscuro
            dibujarModelo("freddy_torzo");
            glColor3f(0.0f, 0.0f, 0.0f);    // Negro
            dibujarModelo("freddy_corbata");
            glColor3f(0.9f, 0.75f, 0.4f);   // Marron claro
            dibujarModelo("freddy_pancita");
            // cadera
            glPushMatrix();
                // Blender: (-0.0000, 0.0771, 57.0200) → OpenGL: (0.0000, 57.0200, -0.0771)
                glTranslatef(0.0000f, 57.0200f, -0.0771f);
                glRotatef(-8, 1, 0, 0);
                glTranslatef(0.0000f, -57.0200f, 0.0771f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_cadera");

                // pierna buena
                glPushMatrix();
                    // Blender: (-7.2192, -0.4607, 49.6020) → OpenGL: (-7.2192, 49.6020, 0.4607f)
                    glTranslatef(-7.2192f, 49.6020f, 0.4607f);
                    glRotatef(interpolar(-50, 70, factorInterpolacion), 1, 0, 0);
                    glTranslatef(7.2192f, -49.6020f, -0.4607f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_pierna_derecha");

                    glPushMatrix();
                        // Blender: (-8.2748, -7.4936, 28.8432) → OpenGL: (-8.2748, 28.8432, 7.4936)
                        glTranslatef(-8.2748f, 28.8432f, 7.4936f);
                        glRotatef(interpolar(40, -40, factorInterpolacion), 1, 0, 0);
                        glTranslatef(8.2748f, -28.8432f, -7.4936f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_rodilla_derecha");

                        glPushMatrix();
                            // Blender: (-8.3349, -4.0948, 26.6729) → OpenGL: (-8.3349, 26.6729, 4.0948)
                            glTranslatef(-8.3349f, 26.6729f, 4.0948f);
                            glRotatef(30, 1, 0, 0);
                            glTranslatef(8.3349f, -26.6729f, -4.0948f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_pantorrilla_derecha");

                            glPushMatrix();
                                // Blender: (-8.2733, -3.7685, 8.2345) → OpenGL: (-8.2733, 8.2345, 3.7685)
                                glTranslatef(-8.2733f, 8.2345f, 3.7685f);
                                glRotatef(interpolar(20, -20, factorInterpolacion), 1, 0, 0);
                                glTranslatef(8.2733f, -8.2345f, -3.7685f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_pie_derecho");
                            glPopMatrix();

                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();

                // pierna mala
                glPushMatrix();
                    // Blender: (7.2256, -0.5372, 49.5044) → OpenGL: (7.2256, 49.5044, 0.5372)
                    glTranslatef(7.2256f, 49.5044f, 0.5372f);
                    glRotatef(interpolar(70, -50, factorInterpolacion), 1, 0, 0);
                    glTranslatef(-7.2256f, -49.5044f, -0.5372f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_pierna_izquierda");

                    glPushMatrix();
                        // Blender: (8.2748, -7.4936, 28.8432) → OpenGL: (8.2748, 28.8432, 7.4936)
                        glTranslatef(8.2748f, 28.8432f, 7.4936f);
                        glRotatef(interpolar(-40, 40, factorInterpolacion), 1, 0, 0);
                        glTranslatef(-8.2748f, -28.8432f, -7.4936f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_rodilla_izquierda");

                        glPushMatrix();
                            // Blender: (8.4381, -3.4338, 26.6654) → OpenGL: (8.4381, 26.6654, 3.4338)
                            glTranslatef(8.4381f, 26.6654f, 3.4338f);
                            glRotatef(30, 1, 0, 0);
                            glTranslatef(-8.4381f, -26.6654f, -3.4338f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_pantorrilla_izquierda");

                            glPushMatrix();
                                // Blender: (8.2439, -3.7138, 8.1688) → OpenGL: (8.2439, 8.1688, 3.7138)
                                glTranslatef(8.2439f, 8.1688f, 3.7138f);
                                glRotatef(interpolar(20, -20, factorInterpolacion), 1, 0, 0);
                                glTranslatef(-8.2439f, -8.168, -3.7138f);
                                glColor3f(0.4f, 0.25f, 0.1f);
                                dibujarModelo("freddy_pie_izquierdo");
                            glPopMatrix();

                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();

            glPopMatrix(); // Fin cadera

            // Hombro derecho, el fuerte
            glPushMatrix();
                // Blender: (-13.3616, 0.7444, 89.1309) → OpenGL: (-13.3616, 89.1309, -0.7444)
                glTranslatef(-13.3616f, 89.1309f, -0.7444f);
                glRotatef(interpolar(0, 15, factorInterpolacion), 0, 1, 0);
                glTranslatef(13.3616f, -89.1309f, 0.7444f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_hombro_derecho");

                // Brazo Derecho
                glPushMatrix();
                    // Blender: (-16.5272, 0.0000, 85.7020) → OpenGL: (-16.5272, 85.7020, 0.0000)
                    glTranslatef(-16.5272f, 85.7020f, 0.0000f);
                    glRotatef(interpolar(0, 70, factorInterpolacion), 0, 0, 1);
                    glTranslatef(16.5272f, -85.7020f, 0.0000f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_brazo_derecho");

                    // Codo Derecho
                    glPushMatrix();
                        // Blender: (-36.0000, -0.3000, 85.6211) → OpenGL: (-36.0000, 85.6211, 0.3000)
                        glTranslatef(-36.0000f, 85.6211f, 0.3000f);
                        glRotatef(interpolar(0, 60, factorInterpolacion), 0, 1, 0);
                        glTranslatef(36.0000f, -85.6211f, -0.3000f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_codo_derecho");

                        // Antebrazo Derecho
                        glPushMatrix();
                            // Blender: (-36.9013, -0.0000, 85.7020) → OpenGL: (-36.9013, 85.7020, 0.0000)
                            glTranslatef(-36.9013f, 85.7020f, 0.0000f);
                            glRotatef(interpolar(0, 35, factorInterpolacion), 0, 1, 0);
                            glTranslatef(36.9013f, -85.7020f, 0.0000f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_antebrazo_derecho");

                            // Mano Derecha
                            glPushMatrix();
                                // Blender: (-51.5157, 0.0000, 85.7020) → OpenGL: (-51.5157, 85.7020, 0.0000)
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

            // hombro izquierdo gaa
            glPushMatrix();
                // Blender: (11.9376, 0.7444, 89.3683) → OpenGL: (11.9376, 89.3683, -0.7444)
                glTranslatef(11.9376f, 89.3683f, -0.7444f);
                glRotatef(interpolar(-15, 0, factorInterpolacion), 0, 1, 0);  //
                glTranslatef(-11.9376f, -89.3683f, 0.7444f);
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_hombro_izquierdo");

                // Brazo Izquierdo
                glPushMatrix();
                    // Blender: (16.8830, 0.0000, 85.8832) → OpenGL: (16.8830, 85.8832, 0.0000)
                    glTranslatef(16.8830f, 85.8832f, 0.0000f);
                    glRotatef(interpolar(-70, 0, factorInterpolacion), 0, 0, 1);
                    glTranslatef(-16.8830f, -85.8832f, 0.0000f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_brazo_izquierdo");
                    // Codo Izquierdo
                    glPushMatrix();
                        // Blender: (36.0000, -0.3000, 85.6211) → OpenGL: (36.0000, 85.6211, 0.3000)
                        glTranslatef(36.0000f, 85.6211f, 0.3000f);
                        glRotatef(interpolar(-60, 0, factorInterpolacion), 0, 1, 0);
                        glTranslatef(-36.0000f, -85.6211f, -0.3000f);
                        glColor3f(0.4f, 0.25f, 0.1f);
                        dibujarModelo("freddy_codo_izquierdo");

                        // Antebrazo Izquierdo
                        glPushMatrix();
                            // Blender: (36.9013, -0.0000, 85.7020) → OpenGL: (36.9013, 85.7020, 0.0000)
                            glTranslatef(36.9013f, 85.7020f, 0.0000f);
                            glRotatef(interpolar(-35, 0, factorInterpolacion), 0, 1, 0);
                            glTranslatef(-36.9013f, -85.7020f, 0.0000f);
                            glColor3f(0.4f, 0.25f, 0.1f);
                            dibujarModelo("freddy_antebrazo_izquierdo");

                            // Mano Izquierda
                            glPushMatrix();
                                // Blender: (51.5157, 0.0000, 85.7020) → OpenGL: (51.5157, 85.7020, 0.0000)
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

            // cabeza unu
            glPushMatrix();
                glColor3f(0.4f, 0.25f, 0.1f);
                dibujarModelo("freddy_cabeza");

                // Orejas
                glPushMatrix();
                    // Movimiento vertical de las orejas (rebote)
                    glTranslatef(0.0f, interpolar(2.0f, -2.0f, factorInterpolacion), 0.0f);
                    glColor3f(0.4f, 0.25f, 0.1f);
                    dibujarModelo("freddy_orejas");
                glPopMatrix();

                // Ocico
                glPushMatrix();
                    glColor3f(0.9f, 0.75f, 0.4f);
                    dibujarModelo("freddy_ocico");
                glPopMatrix();

                // Ojos
                glPushMatrix();
                    glColor3f(0.2f, 0.2f, 0.2f);
                    dibujarModelo("freddy_ojos");
                glPopMatrix();

                // Boca inferior
                glPushMatrix();
                    // Movimiento de la boca (4 unidades hacia abajo y arriba)
                    glTranslatef(0.0f, interpolar(0.0f, -2.0f, factorInterpolacion), 0.0f);
                    glColor3f(0.9f, 0.75f, 0.4f);
                    dibujarModelo("freddy_boca_inferior");

                    // Dientes
                    glPushMatrix();
                        glColor3f(1.0f, 1.0f, 1.0f);
                        dibujarModelo("freddy_dientes");
                    glPopMatrix();

                glPopMatrix();

                // Sombrero
                glPushMatrix();
                    glColor3f(0.0f, 0.0f, 0.0f);
                    dibujarModelo("freddy_sombrero");
                glPopMatrix();

            glPopMatrix(); // Fin cabeza
        glPopMatrix(); // Fin torso
    glPopMatrix(); // Fin
}

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

        // Cambio de cámara activa
        case '1':
            camaraActiva = 0;
            break;
        case '2':
            camaraActiva = 1;
            break;
        case '3':
            camaraActiva = 2;
            break;

        // Selección de cámara a editar
        case 'z':
        case 'Z':
            camaraSeleccionada = 0;
            break;
        case 'x':
        case 'X':
            camaraSeleccionada = 1;
            break;
        case 'c':
        case 'C':
            camaraSeleccionada = 2;
            break;

        // Rotación horizontal (izquierda/derecha)
        case 'a':
        case 'A':
            camaras[camaraSeleccionada].anguloHorizontal -= 5.0f;
            break;
        case 'd':
        case 'D':
            camaras[camaraSeleccionada].anguloHorizontal += 5.0f;
            break;

        // Rotación vertical (arriba/abajo)
        case 'q':
        case 'Q':
            camaras[camaraSeleccionada].anguloVertical -= 5.0f;
            // Limitar para no dar vuelta completa
            if (camaras[camaraSeleccionada].anguloVertical < -89.0f) {
                camaras[camaraSeleccionada].anguloVertical = -89.0f;
            }
            break;
        case 'e':
        case 'E':
            camaras[camaraSeleccionada].anguloVertical += 5.0f;
            // Limitar para no dar vuelta completa
            if (camaras[camaraSeleccionada].anguloVertical > 89.0f) {
                camaras[camaraSeleccionada].anguloVertical = 89.0f;
            }
            break;

        // Zoom
        case 'w':
        case 'W':
            camaras[camaraSeleccionada].radio -= 0.5f;
            if (camaras[camaraSeleccionada].radio < 1.0f) {
                camaras[camaraSeleccionada].radio = 1.0f;
            }
            break;
        case 's':
        case 'S':
            camaras[camaraSeleccionada].radio += 0.5f;
            break;

        // Salir
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
        break;
    case GLUT_KEY_RIGHT:
        camaras[camaraSeleccionada].anguloHorizontal += 5.0f;
        break;

    case GLUT_KEY_UP:
        camaras[camaraSeleccionada].anguloVertical += 5.0f;
        if (camaras[camaraSeleccionada].anguloVertical > 89.0f) {
            camaras[camaraSeleccionada].anguloVertical = 89.0f;
        }
        break;
    case GLUT_KEY_DOWN:
        camaras[camaraSeleccionada].anguloVertical -= 5.0f;
        if (camaras[camaraSeleccionada].anguloVertical < -89.0f) {
            camaras[camaraSeleccionada].anguloVertical = -89.0f;
        }
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
    }
    else if (boton == 4 && estado == GLUT_UP) {
        camaras[camaraSeleccionada].radio += 0.5f;
    }
}

/*
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float radH = camaras[camaraActiva].anguloHorizontal * 3.14159f / 180.0f;
    float radV = camaras[camaraActiva].anguloVertical * 3.14159f / 180.0f;
    float camX = camaras[camaraActiva].radio * cos(radV) * cos(radH);
    float camY = camaras[camaraActiva].radio * sin(radV);
    float camZ = camaras[camaraActiva].radio * cos(radV) * sin(radH);

    gluLookAt(camX, camY, camZ,
              0, 0, 0,
              0, 1, 0);

    glPushMatrix();
    dibujarEjes();
    glPopMatrix();

    glPushMatrix();
    dibujarPiso();
    glPopMatrix();

    glPushMatrix();
    dibujarFreddy();
    glPopMatrix();

    // Dibujar cámaras no activas
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
            dibujarModeloCamara();
            glPopMatrix();
        }
    }

    glutSwapBuffers();
}
*/

void display() {
    // ========== VISTA PRINCIPAL (Cámara Activa) ==========
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
    
    // Posicionar cámara activa
    float radH = camaras[camaraActiva].anguloHorizontal * 3.14159f / 180.0f;
    float radV = camaras[camaraActiva].anguloVertical * 3.14159f / 180.0f;
    float camX = camaras[camaraActiva].radio * cos(radV) * cos(radH);
    float camY = camaras[camaraActiva].radio * sin(radV);
    float camZ = camaras[camaraActiva].radio * cos(radV) * sin(radH);
    
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
    
    // Dibujar escena
    dibujarEjes();
    dibujarPiso();
    dibujarFreddy();
    
    // Dibujar representación de cámaras no activas
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
            
            // Color diferente para cámara seleccionada
            if (i == camaraSeleccionada) {
                glColor3f(1.0f, 1.0f, 0.0f); // Amarillo = seleccionada
            } else {
                glColor3f(0.4f, 0.4f, 0.4f); // Gris = no seleccionada
            }
            dibujarModeloCamara();
            glPopMatrix();
        }
    }
    
    // ========== VISTAS PEQUEÑAS (3 cámaras) ==========
    int anchoVistaPequeña = anchoPantalla / 3;
    int altoVistaPequeña = altoPantalla / 3;
    float aspectPequeño = (float)anchoVistaPequeña / (float)altoVistaPequeña;
    
    for (int i = 0; i < 3; i++) {
        int x = anchoPrincipal;
        int y = altoPantalla - (i + 1) * altoVistaPequeña;
        
        glViewport(x, y, anchoVistaPequeña, altoVistaPequeña);
        glScissor(x, y, anchoVistaPequeña, altoVistaPequeña);
        
        // Limpiar solo esta área
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, aspectPequeño, 0.1f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Posicionar cámara i
        float rH = camaras[i].anguloHorizontal * 3.14159f / 180.0f;
        float rV = camaras[i].anguloVertical * 3.14159f / 180.0f;
        float cX = camaras[i].radio * cos(rV) * cos(rH);
        float cY = camaras[i].radio * sin(rV);
        float cZ = camaras[i].radio * cos(rV) * sin(rH);
        
        gluLookAt(cX, cY, cZ, 0, 0, 0, 0, 1, 0);
        
        // Dibujar escena
        dibujarEjes();
        dibujarPiso();
        dibujarFreddy();
        
        // Borde de color para indicar estado
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Color del borde según estado
        if (i == camaraActiva && i == camaraSeleccionada) {
            glColor3f(0.0f, 1.0f, 0.0f); // Verde = activa Y seleccionada
        } else if (i == camaraActiva) {
            glColor3f(0.0f, 0.5f, 1.0f); // Azul = solo activa
        } else if (i == camaraSeleccionada) {
            glColor3f(1.0f, 1.0f, 0.0f); // Amarillo = solo seleccionada
        } else {
            glColor3f(0.3f, 0.3f, 0.3f); // Gris = ninguna
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
    glutCreateWindow("Freddy");

    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    quadricCamara = gluNewQuadric();

    // Cámara 1 - Vista frontal
    camaras[0].radio = 15.0f;
    camaras[0].anguloHorizontal = 0.0f;
    camaras[0].anguloVertical = 20.0f;

    // Cámara 2 - Vista lateral
    camaras[1].radio = 15.0f;
    camaras[1].anguloHorizontal = 90.0f;
    camaras[1].anguloVertical = 0.0f;

    // Cámara 3 - Vista diagonal alta
    camaras[2].radio = 15.8f;
    camaras[2].anguloHorizontal = 45.0f;
    camaras[2].anguloVertical = 45.0f;

    cout << "Cargando modelos..." << endl;
    cargarTodosLosModelos();
    cout << "Modelos cargados!" << endl;

    glutDisplayFunc(display);
    glutReshapeFunc(redimensionar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciales);
    glutMouseFunc(mouse);
    glutTimerFunc(16, temporizador, 0);

    glutMainLoop();

    return 0;
}