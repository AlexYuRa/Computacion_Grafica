#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// Constantes
const float PI = 3.14159265359f;
const float DEG2RAD = PI / 180.0f;
const float RAD2DEG = 180.0f / PI;

// Variables globales
int windowWidth = 1600;
int windowHeight = 900;
float animationTime = 0.0f;
bool animating = true;
bool showGimbalLock = false;

// Ángulos de Euler (en grados)
float eulerX = 0.0f, eulerY = 0.0f, eulerZ = 0.0f;

// Cuaternion (w, x, y, z)
struct Quaternion {
    float w, x, y, z;
    
    Quaternion(float w = 1, float x = 0, float y = 0, float z = 0) 
        : w(w), x(x), y(y), z(z) {}
    
    void normalize() {
        float mag = sqrt(w*w + x*x + y*y + z*z);
        if (mag > 0.0001f) {
            w /= mag; x /= mag; y /= mag; z /= mag;
        }
    }
    
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        );
    }
    
    static Quaternion fromAxisAngle(float angle, float ax, float ay, float az) {
        float halfAngle = angle * 0.5f;
        float s = sin(halfAngle);
        return Quaternion(cos(halfAngle), ax*s, ay*s, az*s);
    }
    
    void toMatrix(float* matrix) const {
        float xx = x*x, yy = y*y, zz = z*z;
        float xy = x*y, xz = x*z, yz = y*z;
        float wx = w*x, wy = w*y, wz = w*z;
        
        matrix[0] = 1 - 2*(yy + zz); matrix[4] = 2*(xy - wz);     matrix[8]  = 2*(xz + wy);     matrix[12] = 0;
        matrix[1] = 2*(xy + wz);     matrix[5] = 1 - 2*(xx + zz); matrix[9]  = 2*(yz - wx);     matrix[13] = 0;
        matrix[2] = 2*(xz - wy);     matrix[6] = 2*(yz + wx);     matrix[10] = 1 - 2*(xx + yy); matrix[14] = 0;
        matrix[3] = 0;               matrix[7] = 0;               matrix[11] = 0;               matrix[15] = 1;
    }
    
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        Quaternion result;
        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
        
        Quaternion q2_corrected = q2;
        if (dot < 0.0f) {
            q2_corrected.w = -q2.w;
            q2_corrected.x = -q2.x;
            q2_corrected.y = -q2.y;
            q2_corrected.z = -q2.z;
            dot = -dot;
        }
        
        if (dot > 0.9995f) {
            result.w = q1.w + t * (q2_corrected.w - q1.w);
            result.x = q1.x + t * (q2_corrected.x - q1.x);
            result.y = q1.y + t * (q2_corrected.y - q1.y);
            result.z = q1.z + t * (q2_corrected.z - q1.z);
            result.normalize();
            return result;
        }
        
        float theta = acos(dot);
        float sinTheta = sin(theta);
        float w1 = sin((1.0f - t) * theta) / sinTheta;
        float w2 = sin(t * theta) / sinTheta;
        
        result.w = w1 * q1.w + w2 * q2_corrected.w;
        result.x = w1 * q1.x + w2 * q2_corrected.x;
        result.y = w1 * q1.y + w2 * q2_corrected.y;
        result.z = w1 * q1.z + w2 * q2_corrected.z;
        result.normalize();
        
        return result;
    }
};

Quaternion quat(1, 0, 0, 0);

// Matriz de rotación 3x3 (almacenada como 4x4 para OpenGL)
float rotationMatrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

// Funciones de utilidad
void createRotationMatrixX(float angle, float* m) {
    float c = cos(angle), s = sin(angle);
    m[0]=1; m[4]=0;  m[8]=0;   m[12]=0;
    m[1]=0; m[5]=c;  m[9]=-s;  m[13]=0;
    m[2]=0; m[6]=s;  m[10]=c;  m[14]=0;
    m[3]=0; m[7]=0;  m[11]=0;  m[15]=1;
}

void createRotationMatrixY(float angle, float* m) {
    float c = cos(angle), s = sin(angle);
    m[0]=c;  m[4]=0; m[8]=s;   m[12]=0;
    m[1]=0;  m[5]=1; m[9]=0;   m[13]=0;
    m[2]=-s; m[6]=0; m[10]=c;  m[14]=0;
    m[3]=0;  m[7]=0; m[11]=0;  m[15]=1;
}

void createRotationMatrixZ(float angle, float* m) {
    float c = cos(angle), s = sin(angle);
    m[0]=c;  m[4]=-s; m[8]=0;  m[12]=0;
    m[1]=s;  m[5]=c;  m[9]=0;  m[13]=0;
    m[2]=0;  m[6]=0;  m[10]=1; m[14]=0;
    m[3]=0;  m[7]=0;  m[11]=0; m[15]=1;
}

void multiplyMatrices(const float* a, const float* b, float* result) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i*4 + j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i*4 + j] += a[i*4 + k] * b[k*4 + j];
            }
        }
    }
}

void eulerToMatrix(float x, float y, float z, float* result) {
    float mx[16], my[16], mz[16], temp[16];
    createRotationMatrixX(x * DEG2RAD, mx);
    createRotationMatrixY(y * DEG2RAD, my);
    createRotationMatrixZ(z * DEG2RAD, mz);
    
    multiplyMatrices(mz, my, temp);
    multiplyMatrices(temp, mx, result);
}

// Dibujar ejes de coordenadas
void drawAxes(float size = 1.0f) {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Eje X (rojo)
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    // Eje Y (verde)
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    // Eje Z (azul)
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1.0f);
}

// Dibujar un cubo con colores diferentes en cada cara
void drawColoredCube(float size = 0.8f) {
    glBegin(GL_QUADS);
    
    // Cara frontal (roja)
    glColor3f(1, 0, 0);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    
    // Cara trasera (verde oscuro)
    glColor3f(0, 0.7f, 0);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, -size, -size);
    
    // Cara superior (azul)
    glColor3f(0, 0, 1);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);
    glVertex3f(size, size, -size);
    
    // Cara inferior (amarilla)
    glColor3f(1, 1, 0);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    
    // Cara derecha (magenta)
    glColor3f(1, 0, 1);
    glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glVertex3f(size, -size, size);
    
    // Cara izquierda (cian)
    glColor3f(0, 1, 1);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);
    
    glEnd();
    
    // Bordes del cubo en negro
    glColor3f(0, 0, 0);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size, size); glVertex3f(size, -size, size);
    glVertex3f(size, size, size); glVertex3f(-size, size, size);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size, -size); glVertex3f(size, -size, -size);
    glVertex3f(size, size, -size); glVertex3f(-size, size, -size);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(-size, -size, size); glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, size); glVertex3f(size, -size, -size);
    glVertex3f(size, size, size); glVertex3f(size, size, -size);
    glVertex3f(-size, size, size); glVertex3f(-size, size, -size);
    glEnd();
    glLineWidth(1.0f);
}

// Dibujar texto en pantalla
void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_9_BY_15) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Dibujar una escena con el método específico
void drawScene(float* matrix, int method, float vpX, float vpY, float vpWidth, float vpHeight) {
    glViewport(vpX, vpY, vpWidth, vpHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, vpWidth / vpHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 4, 6, 0, 0, 0, 0, 1, 0);
    
    // Dibujar ejes de referencia
    drawAxes(1.5f);
    
    // Aplicar rotación y dibujar cubo
    glPushMatrix();
    glMultMatrixf(matrix);
    drawColoredCube(0.8f);
    glPopMatrix();
}

// Función de renderizado
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Título principal
    drawText(20, windowHeight - 20, "COMPARACION DE REPRESENTACIONES DE ROTACION 3D\n", GLUT_BITMAP_HELVETICA_18);
    drawText(20, windowHeight - 50, "\nPresiona ESPACIO=Pausar, G=Gimbal Lock, R=Reset, +/- =Velocidad", GLUT_BITMAP_9_BY_15);
    
    // Dimensiones de cada viewport
    float vpWidth = windowWidth / 3.0f;
    float vpHeight = (windowHeight - 300) / 2.0f;
    
    // Preparar matrices para cada método
    float matrixMatrix[16], eulerMatrix[16], quatMatrix[16];
    
    // Copiar matriz de rotación
    for (int i = 0; i < 16; i++) matrixMatrix[i] = rotationMatrix[i];
    
    // Calcular matriz desde Euler
    eulerToMatrix(eulerX, eulerY, eulerZ, eulerMatrix);
    
    // Calcular matriz desde cuaternión
    quat.toMatrix(quatMatrix);
    
    // Dibujar las 6 vistas (3 métodos x 2 ángulos de cámara)
    
    // FILA SUPERIOR - Vista principal
    drawScene(matrixMatrix, 0, 0, vpHeight + 300, vpWidth, vpHeight);
    drawScene(eulerMatrix, 1, vpWidth, vpHeight + 300, vpWidth, vpHeight);
    drawScene(quatMatrix, 2, vpWidth * 2, vpHeight + 300, vpWidth, vpHeight);
    
    // FILA INFERIOR - Vista lateral (para ver mejor el gimbal lock)
    glViewport(0, 300, vpWidth, vpHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, vpWidth / vpHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 6, 0, 0, 0, 0, 0, 0, -1);
    drawAxes(1.5f);
    glPushMatrix();
    glMultMatrixf(matrixMatrix);
    drawColoredCube(0.8f);
    glPopMatrix();
    
    glViewport(vpWidth, 300, vpWidth, vpHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, vpWidth / vpHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 6, 0, 0, 0, 0, 0, 0, -1);
    drawAxes(1.5f);
    glPushMatrix();
    glMultMatrixf(eulerMatrix);
    drawColoredCube(0.8f);
    glPopMatrix();
    
    glViewport(vpWidth * 2, 300, vpWidth, vpHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, vpWidth / vpHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 6, 0, 0, 0, 0, 0, 0, -1);
    drawAxes(1.5f);
    glPushMatrix();
    glMultMatrixf(quatMatrix);
    drawColoredCube(0.8f);
    glPopMatrix();
    
    // Información detallada de cada método
    std::stringstream ss;
    float infoY = 250;
    
    // MATRIZ
    glColor3f(0.2f, 0.2f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(10, 10);
    glVertex2f(vpWidth - 10, 10);
    glVertex2f(vpWidth - 10, 280);
    glVertex2f(10, 280);
    glEnd();
    
    glColor3f(1, 1, 0);
    drawText(20, infoY + 20, "MATRIZ DE ROTACION", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1, 1, 1);
    ss.str(""); ss << std::fixed << std::setprecision(2);
    ss << "[" << rotationMatrix[0] << " " << rotationMatrix[4] << " " << rotationMatrix[8] << "]";
    drawText(20, infoY - 10, ss.str());
    ss.str("");
    ss << "[" << rotationMatrix[1] << " " << rotationMatrix[5] << " " << rotationMatrix[9] << "]";
    drawText(20, infoY - 35, ss.str());
    ss.str("");
    ss << "[" << rotationMatrix[2] << " " << rotationMatrix[6] << " " << rotationMatrix[10] << "]";
    drawText(20, infoY - 60, ss.str());
    
    glColor3f(0, 1, 0);
    drawText(20, infoY - 90, "Ventajas:");
    glColor3f(1, 1, 1);
    drawText(20, infoY - 110, "- Sin gimbal lock");
    drawText(20, infoY - 130, "- Directa en GPU");
    glColor3f(1, 0.5f, 0);
    drawText(20, infoY - 155, "Desventajas:");
    glColor3f(1, 1, 1);
    drawText(20, infoY - 175, "- 9 valores");
    drawText(20, infoY - 195, "- Pierde ortogonalidad");
    
    // EULER
    glColor3f(0.2f, 0.2f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(vpWidth + 10, 10);
    glVertex2f(vpWidth * 2 - 10, 10);
    glVertex2f(vpWidth * 2 - 10, 280);
    glVertex2f(vpWidth + 10, 280);
    glEnd();
    
    if (showGimbalLock) {
        glColor3f(1, 0, 0);
    } else {
        glColor3f(1, 1, 0);
    }
    drawText(vpWidth + 20, infoY + 20, "ANGULOS DE EULER", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1, 1, 1);
    ss.str(""); ss << std::fixed << std::setprecision(1);
    ss << "Roll  (X): " << eulerX << " grados";
    drawText(vpWidth + 20, infoY - 10, ss.str());
    ss.str("");
    ss << "Pitch (Y): " << eulerY << " grados";
    drawText(vpWidth + 20, infoY - 35, ss.str());
    ss.str("");
    ss << "Yaw   (Z): " << eulerZ << " grados";
    drawText(vpWidth + 20, infoY - 60, ss.str());
    
    if (showGimbalLock) {
        glColor3f(1, 0, 0);
        drawText(vpWidth + 20, infoY - 90, "*** GIMBAL LOCK ***");
        drawText(vpWidth + 20, infoY - 110, "Y = 90 grados");
        drawText(vpWidth + 20, infoY - 130, "X y Z alineados!");
        glColor3f(1, 1, 1);
        drawText(vpWidth + 20, infoY - 155, "Rotacion erratica");
        drawText(vpWidth + 20, infoY - 175, "Perdida de DOF");
    } else {
        glColor3f(0, 1, 0);
        drawText(vpWidth + 20, infoY - 90, "Ventajas:");
        glColor3f(1, 1, 1);
        drawText(vpWidth + 20, infoY - 110, "- Intuitivo (3 vals)");
        drawText(vpWidth + 20, infoY - 130, "- Facil entender");
        glColor3f(1, 0.5f, 0);
        drawText(vpWidth + 20, infoY - 155, "Desventajas:");
        glColor3f(1, 1, 1);
        drawText(vpWidth + 20, infoY - 175, "- GIMBAL LOCK!");
        drawText(vpWidth + 20, infoY - 195, "- Mala interpolacion");
    }
    
    // CUATERNION
    glColor3f(0.2f, 0.2f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(vpWidth * 2 + 10, 10);
    glVertex2f(windowWidth - 10, 10);
    glVertex2f(windowWidth - 10, 280);
    glVertex2f(vpWidth * 2 + 10, 280);
    glEnd();
    
    glColor3f(1, 1, 0);
    drawText(vpWidth * 2 + 20, infoY + 20, "CUATERNION", GLUT_BITMAP_HELVETICA_18);
    glColor3f(1, 1, 1);
    ss.str(""); ss << std::fixed << std::setprecision(3);
    ss << "w = " << quat.w << " (escalar)";
    drawText(vpWidth * 2 + 20, infoY - 10, ss.str());
    ss.str("");
    ss << "x = " << quat.x;
    drawText(vpWidth * 2 + 20, infoY - 35, ss.str());
    ss.str("");
    ss << "y = " << quat.y;
    drawText(vpWidth * 2 + 20, infoY - 60, ss.str());
    ss.str("");
    ss << "z = " << quat.z;
    drawText(vpWidth * 2 + 20, infoY - 85, ss.str());
    
    glColor3f(0, 1, 0);
    drawText(vpWidth * 2 + 20, infoY - 115, "Ventajas:");
    glColor3f(1, 1, 1);
    drawText(vpWidth * 2 + 20, infoY - 135, "- Sin gimbal lock");
    drawText(vpWidth * 2 + 20, infoY - 155, "- SLERP suave");
    drawText(vpWidth * 2 + 20, infoY - 175, "- 4 valores");
    glColor3f(1, 0.5f, 0);
    drawText(vpWidth * 2 + 20, infoY - 200, "Desventajas:");
    glColor3f(1, 1, 1);
    drawText(vpWidth * 2 + 20, infoY - 220, "- Menos intuitivo");
    
    glutSwapBuffers();
}

// Actualización de animación
void update(int value) {
    if (animating) {
        animationTime += 0.02f;
        
        // MATRICES: rotación combinada suave
        float rx[16], ry[16], rz[16], temp[16];
        createRotationMatrixX(animationTime * 0.8f, rx);
        createRotationMatrixY(animationTime * 1.0f, ry);
        createRotationMatrixZ(animationTime * 0.6f, rz);
        multiplyMatrices(rz, ry, temp);
        multiplyMatrices(temp, rx, rotationMatrix);
        
        // EULER: animación que demuestra gimbal lock
        if (showGimbalLock) {
            eulerY = 90.0f; // Bloqueo de gimbal
            eulerX = sin(animationTime * 3.0f) * 180.0f;
            eulerZ = cos(animationTime * 3.0f) * 180.0f;
        } else {
            eulerX = sin(animationTime * 0.8f) * 60.0f;
            eulerY = sin(animationTime * 1.0f) * 60.0f;
            eulerZ = sin(animationTime * 0.6f) * 60.0f;
        }
        
        // CUATERNIONES: rotación continua usando eje variable
        float axisX = sin(animationTime * 0.3f);
        float axisY = cos(animationTime * 0.5f);
        float axisZ = sin(animationTime * 0.7f);
        float axisMag = sqrt(axisX*axisX + axisY*axisY + axisZ*axisZ);
        if (axisMag > 0.001f) {
            axisX /= axisMag;
            axisY /= axisMag;
            axisZ /= axisMag;
        }
        quat = Quaternion::fromAxisAngle(animationTime * 1.5f, axisX, axisY, axisZ);
        quat.normalize();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Manejo de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ' ':
            animating = !animating;
            break;
        case 'g':
        case 'G':
            showGimbalLock = !showGimbalLock;
            animationTime = 0;
            break;
        case 'r':
        case 'R':
            animationTime = 0;
            eulerX = eulerY = eulerZ = 0;
            quat = Quaternion(1, 0, 0, 0);
            showGimbalLock = false;
            for (int i = 0; i < 16; i++) rotationMatrix[i] = (i%5 == 0) ? 1 : 0;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// Redimensionamiento de ventana
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
}

// Inicialización
void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Comparacion de Rotaciones 3D - Matriz vs Euler vs Cuaternion");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    
    std::cout << "\n=== COMPARACION DE ROTACIONES 3D ===" << std::endl;
    std::cout << "\nLos 3 metodos se muestran simultaneamente:" << std::endl;
    std::cout << "  IZQUIERDA  - Matrices de Rotacion" << std::endl;
    std::cout << "  CENTRO     - Angulos de Euler" << std::endl;
    std::cout << "  DERECHA    - Cuaterniones" << std::endl;
    std::cout << "\nFilas:" << std::endl;
    std::cout << "  SUPERIOR   - Vista en perspectiva" << std::endl;
    std::cout << "  INFERIOR   - Vista lateral (detectar gimbal lock)" << std::endl;
    std::cout << "\nControles:" << std::endl;
    std::cout << "  ESPACIO - Pausar/Reanudar animacion" << std::endl;
    std::cout << "  G - Activar/Desactivar GIMBAL LOCK (modo Euler)" << std::endl;
    std::cout << "  R - Reiniciar animacion" << std::endl;
    std::cout << "  ESC - Salir" << std::endl;
    std::cout << "\nOBSERVA:" << std::endl;
    std::cout << "  - Los 3 cubos rotan con el MISMO movimiento" << std::endl;
    std::cout << "  - Presiona 'G' para ver como Euler falla (gimbal lock)" << std::endl;
    std::cout << "  - Matrices y Cuaterniones NUNCA tienen gimbal lock\n" << std::endl;
    
    glutMainLoop();
    return 0;
}