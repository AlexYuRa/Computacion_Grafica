#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

// Constantes
#define PI 3.14159265359
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)

// Estructura para Cuaternión
struct Quaternion {
    float w, x, y, z;
    
    Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) 
        : w(w), x(x), y(y), z(z) {}
    
    // Normalizar cuaternión
    void normalize() {
        float magnitude = sqrt(w*w + x*x + y*y + z*z);
        if (magnitude > 0.0001f) {
            w /= magnitude; x /= magnitude; y /= magnitude; z /= magnitude;
        }
    }
    
    // Conversión de ángulos de Euler a cuaternión
    static Quaternion fromEuler(float pitch, float yaw, float roll) {
        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);
        float cy = cos(yaw * 0.5f);
        float sy = sin(yaw * 0.5f);
        float cr = cos(roll * 0.5f);
        float sr = sin(roll * 0.5f);
        
        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        
        return q;
    }
    
    // Conversión a matriz de rotación
    void toMatrix(float matrix[16]) {
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;
        
        matrix[0]  = 1 - 2 * (yy + zz);
        matrix[1]  = 2 * (xy - zw);
        matrix[2]  = 2 * (xz + yw);
        matrix[3]  = 0;
        
        matrix[4]  = 2 * (xy + zw);
        matrix[5]  = 1 - 2 * (xx + zz);
        matrix[6]  = 2 * (yz - xw);
        matrix[7]  = 0;
        
        matrix[8]  = 2 * (xz - yw);
        matrix[9]  = 2 * (yz + xw);
        matrix[10] = 1 - 2 * (xx + yy);
        matrix[11] = 0;
        
        matrix[12] = 0;
        matrix[13] = 0;
        matrix[14] = 0;
        matrix[15] = 1;
    }
    
    // SLERP (interpolación esférica)
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
        float dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
        
        // Si el dot product es negativo, usar -q2 para tomar el camino más corto
        Quaternion q2_copy = q2;
        if (dot < 0.0f) {
            q2_copy.w = -q2.w;
            q2_copy.x = -q2.x;
            q2_copy.y = -q2.y;
            q2_copy.z = -q2.z;
            dot = -dot;
        }
        
        const float DOT_THRESHOLD = 0.9995f;
        if (dot > DOT_THRESHOLD) {
            // Interpolación linear para ángulos muy pequeños
            Quaternion result;
            result.w = q1.w + t * (q2_copy.w - q1.w);
            result.x = q1.x + t * (q2_copy.x - q1.x);
            result.y = q1.y + t * (q2_copy.y - q1.y);
            result.z = q1.z + t * (q2_copy.z - q1.z);
            result.normalize();
            return result;
        }
        
        float theta_0 = acos(dot);
        float theta = theta_0 * t;
        float sin_theta = sin(theta);
        float sin_theta_0 = sin(theta_0);
        
        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;
        
        return Quaternion(
            s0 * q1.w + s1 * q2_copy.w,
            s0 * q1.x + s1 * q2_copy.x,
            s0 * q1.y + s1 * q2_copy.y,
            s0 * q1.z + s1 * q2_copy.z
        );
    }
};

// Variables globales
enum RotationMode { EULER_MODE, QUATERNION_MODE, MATRIX_MODE };
RotationMode currentMode = EULER_MODE;

// Ángulos de Euler (en grados)
float eulerX = 0.0f, eulerY = 0.0f, eulerZ = 0.0f;

// Cuaternión
Quaternion currentQuaternion(1, 0, 0, 0);

// Matriz de rotación
float rotationMatrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

// Estados de demostración
bool isAnimating = false;
bool gimbalLockDemo = false;
bool showComparison = false;
int animationStep = 0;
float animationTime = 0.0f;

// Control de cámara
float cameraDistance = 8.0f;
float cameraAngleX = 20.0f;
float cameraAngleY = 45.0f;

// Para texto en pantalla
void renderBitmapString(float x, float y, void *font, const std::string& str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

// Dibujar cubo con caras de colores diferentes
void drawColoredCube() {
    glBegin(GL_QUADS);
    
    // Cara frontal (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Cara trasera (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Cara superior (Azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Cara inferior (Amarillo)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Cara derecha (Rojo)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Cara izquierda (Verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
    
    // Dibujar wireframe para mejor visibilidad
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(2.0f);
    glutWireCube(2.0f);
    glLineWidth(1.0f);
}

// Dibujar ejes coordenados
void drawAxes() {
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    
    // Eje X (Rojo)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    
    // Eje Y (Verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    
    // Eje Z (Azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    
    glEnd();
    glLineWidth(1.0f);
}

// Actualizar rotación basada en el modo actual
void updateRotation() {
    if (currentMode == EULER_MODE) {
        // Actualizar cuaternión y matriz desde Euler
        currentQuaternion = Quaternion::fromEuler(
            DEG_TO_RAD(eulerX), 
            DEG_TO_RAD(eulerY), 
            DEG_TO_RAD(eulerZ)
        );
        currentQuaternion.toMatrix(rotationMatrix);
    }
    else if (currentMode == QUATERNION_MODE) {
        // Actualizar matriz desde cuaternión
        currentQuaternion.normalize();
        currentQuaternion.toMatrix(rotationMatrix);
        
        // Actualizar Euler desde cuaternión (aproximado)
        float test = currentQuaternion.x*currentQuaternion.y + currentQuaternion.z*currentQuaternion.w;
        if (test > 0.499) { // singularidad en el polo norte
            eulerY = 2 * atan2(currentQuaternion.x, currentQuaternion.w) * 180/PI;
            eulerZ = 90;
            eulerX = 0;
        }
        else if (test < -0.499) { // singularidad en el polo sur
            eulerY = -2 * atan2(currentQuaternion.x, currentQuaternion.w) * 180/PI;
            eulerZ = -90;
            eulerX = 0;
        }
        else {
            eulerY = atan2(2*currentQuaternion.y*currentQuaternion.w-2*currentQuaternion.x*currentQuaternion.z, 1-2*currentQuaternion.y*currentQuaternion.y-2*currentQuaternion.z*currentQuaternion.z) * 180/PI;
            eulerZ = asin(2*test) * 180/PI;
            eulerX = atan2(2*currentQuaternion.x*currentQuaternion.w-2*currentQuaternion.y*currentQuaternion.z, 1-2*currentQuaternion.x*currentQuaternion.x-2*currentQuaternion.z*currentQuaternion.z) * 180/PI;
        }
    }
}

// Función de renderizado principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Configurar vista 3D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0/600.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Posicionar cámara
    gluLookAt(
        cameraDistance * cos(DEG_TO_RAD(cameraAngleX)) * cos(DEG_TO_RAD(cameraAngleY)),
        cameraDistance * sin(DEG_TO_RAD(cameraAngleX)),
        cameraDistance * cos(DEG_TO_RAD(cameraAngleX)) * sin(DEG_TO_RAD(cameraAngleY)),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );
    
    // Dibujar ejes coordenados
    drawAxes();
    
    if (showComparison) {
        // Mostrar tres cubos para comparación
        
        // Cubo 1: Euler (izquierda)
        glPushMatrix();
        glTranslatef(-4.0f, 0.0f, 0.0f);
        glRotatef(eulerZ, 0, 0, 1);
        glRotatef(eulerY, 0, 1, 0);
        glRotatef(eulerX, 1, 0, 0);
        drawColoredCube();
        glPopMatrix();
        
        // Cubo 2: Actual (centro)
        glPushMatrix();
        glMultMatrixf(rotationMatrix);
        drawColoredCube();
        glPopMatrix();
        
        // Cubo 3: Quaternion directo (derecha)
        glPushMatrix();
        glTranslatef(4.0f, 0.0f, 0.0f);
        float qMatrix[16];
        currentQuaternion.toMatrix(qMatrix);
        glMultMatrixf(qMatrix);
        drawColoredCube();
        glPopMatrix();
    }
    else {
        // Mostrar un solo cubo
        glPushMatrix();
        if (currentMode == MATRIX_MODE) {
            glMultMatrixf(rotationMatrix);
        } else {
            glRotatef(eulerZ, 0, 0, 1);
            glRotatef(eulerY, 0, 1, 0);
            glRotatef(eulerX, 1, 0, 0);
        }
        drawColoredCube();
        glPopMatrix();
    }
    
    // Cambiar a vista 2D para texto
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Mostrar información del modo actual
    std::string modeStr;
    switch(currentMode) {
        case EULER_MODE: modeStr = "MODO: Angulos de Euler"; break;
        case QUATERNION_MODE: modeStr = "MODO: Cuaterniones"; break;
        case MATRIX_MODE: modeStr = "MODO: Matrices"; break;
    }
    renderBitmapString(10, 30, GLUT_BITMAP_HELVETICA_18, modeStr);
    
    // Mostrar valores actuales
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    // Ángulos de Euler
    ss.str("");
    ss << "Euler X: " << eulerX << "°  Y: " << eulerY << "°  Z: " << eulerZ << "°";
    renderBitmapString(10, 60, GLUT_BITMAP_HELVETICA_12, ss.str());
    
    // Cuaternión
    ss.str("");
    ss << "Quaternion W: " << currentQuaternion.w << " X: " << currentQuaternion.x 
       << " Y: " << currentQuaternion.y << " Z: " << currentQuaternion.z;
    renderBitmapString(10, 80, GLUT_BITMAP_HELVETICA_12, ss.str());
    
    // Advertencia de Gimbal Lock
    if (abs(eulerY) > 85) {
        glColor3f(1.0f, 0.0f, 0.0f);
        renderBitmapString(10, 100, GLUT_BITMAP_HELVETICA_12, "ADVERTENCIA: Cerca de Gimbal Lock!");
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    // Controles
    renderBitmapString(10, 520, GLUT_BITMAP_HELVETICA_12, "Controles:");
    renderBitmapString(10, 540, GLUT_BITMAP_HELVETICA_10, "1,2,3: Cambiar modo  |  Q,A: Pitch  |  W,S: Yaw  |  E,D: Roll");
    renderBitmapString(10, 555, GLUT_BITMAP_HELVETICA_10, "G: Demo Gimbal Lock  |  I: Demo Interpolacion  |  C: Comparacion  |  R: Reset");
    renderBitmapString(10, 570, GLUT_BITMAP_HELVETICA_10, "Mouse: Rotar camara  |  Scroll: Zoom");
    
    if (gimbalLockDemo) {
        glColor3f(1.0f, 0.0f, 0.0f);
        renderBitmapString(300, 100, GLUT_BITMAP_HELVETICA_12, "DEMOSTRANDO GIMBAL LOCK...");
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    glEnable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

// Manejo del teclado
void keyboard(unsigned char key, int x, int y) {
    const float increment = 5.0f;
    
    switch(key) {
        case '1': currentMode = EULER_MODE; break;
        case '2': currentMode = QUATERNION_MODE; break;
        case '3': currentMode = MATRIX_MODE; break;
        
        case 'q': case 'Q': eulerX += increment; break;
        case 'a': case 'A': eulerX -= increment; break;
        case 'w': case 'W': eulerY += increment; break;
        case 's': case 'S': eulerY -= increment; break;
        case 'e': case 'E': eulerZ += increment; break;
        case 'd': case 'D': eulerZ -= increment; break;
        
        case 'r': case 'R':
            eulerX = eulerY = eulerZ = 0.0f;
            currentQuaternion = Quaternion(1, 0, 0, 0);
            break;
            
        case 'g': case 'G':
            gimbalLockDemo = !gimbalLockDemo;
            if (gimbalLockDemo) {
                currentMode = EULER_MODE;
                eulerX = 0; eulerY = 90; eulerZ = 0;
            }
            break;
            
        case 'i': case 'I':
            isAnimating = !isAnimating;
            animationStep = 0;
            animationTime = 0;
            break;
            
        case 'c': case 'C':
            showComparison = !showComparison;
            break;
            
        case 27: // ESC
            exit(0);
            break;
    }
    
    updateRotation();
    glutPostRedisplay();
}

// Control del mouse para la cámara
int lastMouseX, lastMouseY;
bool mousePressed = false;

void mouseMotion(int x, int y) {
    if (mousePressed) {
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;
        
        if (cameraAngleX > 89) cameraAngleX = 89;
        if (cameraAngleX < -89) cameraAngleX = -89;
        
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mousePressed = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
    else if (button == 3) { // scroll up
        cameraDistance -= 0.5f;
        if (cameraDistance < 2.0f) cameraDistance = 2.0f;
        glutPostRedisplay();
    }
    else if (button == 4) { // scroll down
        cameraDistance += 0.5f;
        if (cameraDistance > 20.0f) cameraDistance = 20.0f;
        glutPostRedisplay();
    }
}

// Animación para demos
void timer(int value) {
    if (isAnimating) {
        animationTime += 0.05f;
        
        // Demo de interpolación SLERP
        Quaternion start(1, 0, 0, 0);
        Quaternion end = Quaternion::fromEuler(PI/4, PI/3, PI/6);
        
        float t = (sin(animationTime) + 1.0f) * 0.5f; // oscila entre 0 y 1
        currentQuaternion = Quaternion::slerp(start, end, t);
        
        currentMode = QUATERNION_MODE;
        updateRotation();
        glutPostRedisplay();
    }
    
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Redimensionar ventana
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Demo de Rotaciones 3D - OpenGL C++");
    
    // Configuración inicial de OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    
    // Configurar iluminación
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float lightPos[] = {3.0f, 3.0f, 3.0f, 1.0f};
    float lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    
    float ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    // Registrar callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    std::cout << "\n=== DEMO DE ROTACIONES 3D ===\n";
    std::cout << "Controles:\n";
    std::cout << "1, 2, 3: Cambiar entre Euler, Quaternion, Matrix\n";
    std::cout << "Q/A: Pitch (X)  W/S: Yaw (Y)  E/D: Roll (Z)\n";
    std::cout << "G: Demostrar Gimbal Lock\n";
    std::cout << "I: Demo de interpolación\n";
    std::cout << "C: Mostrar comparación lado a lado\n";
    std::cout << "R: Reset rotación\n";
    std::cout << "Mouse: Rotar cámara, Scroll: Zoom\n";
    std::cout << "ESC: Salir\n\n";
    
    updateRotation();
    glutMainLoop();
    
    return 0;
}