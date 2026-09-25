#include <GL/freeglut.h>
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
int windowWidth = 1400;
int windowHeight = 800;
int currentDemo = 0; // 0=Comparación normal, 1=Gimbal Lock, 2=Interpolación suave
float animationTime = 0.0f;
bool animating = true;
bool showInfo = true;
bool manualControl = false; // Modo control manual

// Ángulos objetivo para todas las representaciones
float targetX = 30.0f, targetY = 45.0f, targetZ = 60.0f;

// Ángulos de Euler para control manual
float manualEulerX = 0.0f, manualEulerY = 0.0f, manualEulerZ = 0.0f;

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
        
        if (dot < 0.0f) {
            dot = -dot;
        }
        
        if (dot > 0.9995f) {
            result.w = q1.w + t * (q2.w - q1.w);
            result.x = q1.x + t * (q2.x - q1.x);
            result.y = q1.y + t * (q2.y - q1.y);
            result.z = q1.z + t * (q2.z - q1.z);
            result.normalize();
            return result;
        }
        
        float theta = acos(fabs(dot));
        float sinTheta = sin(theta);
        float w1 = sin((1.0f - t) * theta) / sinTheta;
        float w2 = sin(t * theta) / sinTheta;
        
        result.w = w1 * q1.w + w2 * q2.w;
        result.x = w1 * q1.x + w2 * q2.x;
        result.y = w1 * q1.y + w2 * q2.y;
        result.z = w1 * q1.z + w2 * q2.z;
        result.normalize();
        
        return result;
    }
};

Quaternion quat(1, 0, 0, 0);

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

// Función para actualizar todas las representaciones desde Euler
void updateFromEuler(float eulerX, float eulerY, float eulerZ, Quaternion& outQuat, float* outMatrix) {
    // Crear cuaternión desde ángulos de Euler
    outQuat = Quaternion::fromAxisAngle(eulerX * DEG2RAD, 1, 0, 0) *
              Quaternion::fromAxisAngle(eulerY * DEG2RAD, 0, 1, 0) *
              Quaternion::fromAxisAngle(eulerZ * DEG2RAD, 0, 0, 1);
    outQuat.normalize();
    
    // Crear matriz desde ángulos de Euler
    eulerToMatrix(eulerX, eulerY, eulerZ, outMatrix);
}

// Dibujar ejes de coordenadas
void drawAxes(float size = 1.5f) {
    glLineWidth(3.0f);
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
void drawColoredCube(float size = 1.0f) {
    glBegin(GL_QUADS);
    
    // Cara frontal (roja)
    glColor3f(1, 0, 0);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    
    // Cara trasera (verde)
    glColor3f(0, 1, 0);
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
void drawText(float x, float y, const std::string& text) {
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
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// Función de renderizado
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Título principal
    std::stringstream ss;
    ss << "COMPARACION DE REPRESENTACIONES DE ROTACION 3D";
    if (currentDemo == 1) ss << " - DEMOSTRACION GIMBAL LOCK";
    else if (currentDemo == 2) ss << " - INTERPOLACION SUAVE";
    else if (manualControl) ss << " - CONTROL MANUAL";
    drawText(20, windowHeight - 30, ss.str());
    
    drawText(20, windowHeight - 60, "1=Normal, 2=Gimbal Lock, 3=Interpolacion, M=Control Manual");
    if (manualControl) {
        drawText(20, windowHeight - 90, "CONTROL MANUAL - X/x=Roll, Y/y=Pitch, Z/z=Yaw (+/- 10 grados)");
    }
    
    // Calcular rotaciones actuales para todas las representaciones
    float currentEulerX, currentEulerY, currentEulerZ;
    Quaternion currentQuat;
    float currentMatrix[16];
    
    if (manualControl) {
        // Modo control manual - usar ángulos manuales
        currentEulerX = manualEulerX;
        currentEulerY = manualEulerY;
        currentEulerZ = manualEulerZ;
        updateFromEuler(currentEulerX, currentEulerY, currentEulerZ, currentQuat, currentMatrix);
        
    } else if (currentDemo == 0) {
        // Rotación normal - mismo objetivo para todos
        currentEulerX = targetX + sin(animationTime * 0.5f) * 30.0f;
        currentEulerY = targetY + sin(animationTime * 0.7f) * 45.0f;
        currentEulerZ = targetZ + sin(animationTime * 0.3f) * 30.0f;
        
        // Crear cuaternión desde Euler
        currentQuat = Quaternion::fromAxisAngle(currentEulerX * DEG2RAD, 1, 0, 0) *
                     Quaternion::fromAxisAngle(currentEulerY * DEG2RAD, 0, 1, 0) *
                     Quaternion::fromAxisAngle(currentEulerZ * DEG2RAD, 0, 0, 1);
        
        // Crear matriz desde Euler
        eulerToMatrix(currentEulerX, currentEulerY, currentEulerZ, currentMatrix);
        
    } else if (currentDemo == 1) {
        // GIMBAL LOCK DEMOSTRACIÓN MEJORADA
        // Forzamos Y a 90° (gimbal lock) y variamos X y Z
        currentEulerY = 90.0f; // ¡GIMBAL LOCK!
        
        // X y Z ahora se comportan de manera idéntica - esto es el problema
        float angle = sin(animationTime * 1.5f) * 45.0f;
        currentEulerX = angle;
        currentEulerZ = angle; // Mismo valor - esto demuestra la pérdida de grado de libertad
        
        // Para el cuaternión, hacemos una rotación normal sin gimbal lock
        // Esto demostrará que el cuaternión NO sufre este problema
        Quaternion qx = Quaternion::fromAxisAngle(sin(animationTime * 0.8f) * 60.0f * DEG2RAD, 1, 0, 0);
        Quaternion qy = Quaternion::fromAxisAngle(90.0f * DEG2RAD, 0, 1, 0); // Misma Y
        Quaternion qz = Quaternion::fromAxisAngle(cos(animationTime * 1.2f) * 45.0f * DEG2RAD, 0, 0, 1);
        currentQuat = qz * qy * qx; // Combinación suave sin gimbal lock
        currentQuat.normalize();
        
        // La matriz hereda el problema de Euler
        eulerToMatrix(currentEulerX, currentEulerY, currentEulerZ, currentMatrix);
        
    } else {
        // Interpolación suave - SLERP vs interpolación lineal
        float t = (sin(animationTime * 0.8f) + 1.0f) * 0.5f;
        
        currentEulerX = 30.0f + t * 60.0f; // Interpolación lineal
        currentEulerY = 45.0f + t * 90.0f;
        currentEulerZ = 60.0f + t * 120.0f;
        
        // SLERP para cuaterniones
        Quaternion q1 = Quaternion::fromAxisAngle(30.0f * DEG2RAD, 1, 0, 0);
        Quaternion q2 = Quaternion::fromAxisAngle(150.0f * DEG2RAD, 0, 1, 1);
        q2.normalize();
        currentQuat = Quaternion::slerp(q1, q2, t);
        
        eulerToMatrix(currentEulerX, currentEulerY, currentEulerZ, currentMatrix);
    }
    
    // Información técnica en panel lateral
    if (showInfo) {
        glColor3f(0.15f, 0.15f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(1000, 100);
        glVertex2f(1380, 100);
        glVertex2f(1380, 600);
        glVertex2f(1000, 600);
        glEnd();
        
        float infoY = 580;
        glColor3f(1, 1, 1);
        if (manualControl) {
            drawText(1010, infoY, "CONTROL MANUAL ACTIVO");
            infoY -= 25;
            glColor3f(0, 1, 0);
            drawText(1010, infoY, "Use X/x Y/y Z/z para rotar");
        } else {
            drawText(1010, infoY, "INFORMACION TECNICA:");
        }
        
        infoY -= 30;
        drawText(1010, infoY, "ANGULOS DE EULER:");
        infoY -= 20;
        ss.str(""); ss << std::fixed << std::setprecision(1);
        ss << "X: " << currentEulerX << "°";
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str(""); ss << "Y: " << currentEulerY << "°";
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str(""); ss << "Z: " << currentEulerZ << "°";
        drawText(1015, infoY, ss.str());
        
        infoY -= 25;
        drawText(1010, infoY, "CUATERNION:");
        infoY -= 20;
        ss.str(""); ss << std::fixed << std::setprecision(3);
        ss << "w: " << currentQuat.w;
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str(""); ss << "x: " << currentQuat.x;
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str(""); ss << "y: " << currentQuat.y;
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str(""); ss << "z: " << currentQuat.z;
        drawText(1015, infoY, ss.str());
        
        infoY -= 25;
        drawText(1010, infoY, "MATRIZ (primeras 2 filas):");
        infoY -= 20;
        ss.str(""); ss << std::fixed << std::setprecision(2);
        ss << "[" << currentMatrix[0] << " " << currentMatrix[4] << " " << currentMatrix[8] << "]";
        drawText(1015, infoY, ss.str());
        infoY -= 15;
        ss.str("");
        ss << "[" << currentMatrix[1] << " " << currentMatrix[5] << " " << currentMatrix[9] << "]";
        drawText(1015, infoY, ss.str());
        
        // Advertencias específicas
        infoY -= 35;
        glColor3f(1, 1, 0);
        if (currentDemo == 1) {
            drawText(1010, infoY, "GIMBAL LOCK ACTIVO:");
            infoY -= 20;
            glColor3f(1, 0, 0);
            drawText(1015, infoY, "- Matriz/Euler: X y Z identicos!");
            infoY -= 15;
            drawText(1015, infoY, "- Perdida 1 grado libertad");
            infoY -= 15;
            glColor3f(0, 1, 0);
            drawText(1015, infoY, "- Cuaternion: Rotacion normal");
            infoY -= 15;
            glColor3f(1, 1, 1);
            drawText(1015, infoY, "Y=90° causa el problema");
        } else if (currentDemo == 2) {
            drawText(1010, infoY, "INTERPOLACION:");
            infoY -= 20;
            drawText(1015, infoY, "- Euler: Lineal (problemas)");
            infoY -= 15;
            drawText(1015, infoY, "- Cuaternion: SLERP (suave)");
            infoY -= 15;
            drawText(1015, infoY, "- Matriz: Costosa");
        } else if (manualControl) {
            drawText(1010, infoY, "CONTROL MANUAL:");
            infoY -= 20;
            glColor3f(0, 1, 1);
            drawText(1015, infoY, "- Pruebe Y=90° para gimbal lock");
            infoY -= 15;
            drawText(1015, infoY, "- Observe como se sincronizan");
            infoY -= 15;
            drawText(1015, infoY, "- Cuaternion siempre suave");
        } else {
            drawText(1010, infoY, "DIFERENCIAS SUTILES:");
            infoY -= 20;
            drawText(1015, infoY, "- Matriz: Aplicacion directa");
            infoY -= 15;
            drawText(1015, infoY, "- Euler: Intuitivo pero limitado");
            infoY -= 15;
            drawText(1015, infoY, "- Cuaternion: Matematica robusta");
        }
    }
    
    // VISTA PRINCIPAL: Tres cubos lado a lado
    glViewport(0, 0, 1000, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1000.0f / windowHeight, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3, 12, 0, 0, 0, 0, 1, 0);
    
    // Ejes de referencia globales
    drawAxes(3.0f);
    
    // CUBO 1: MATRIZ DE ROTACION (izquierda)
    glPushMatrix();
    glTranslatef(-4.0f, 0, 0);
    
    // Aplicar rotación por matriz
    glMultMatrixf(currentMatrix);
    
    drawColoredCube(1.2f);
    drawAxes(1.8f);
    glPopMatrix();
    
    // Etiqueta
    glColor3f(1, 0, 1);
    drawText(-5.0f, -3.5f, "MATRIZ ROTACION");
    
    // CUBO 2: ANGULOS DE EULER (centro)
    glPushMatrix();
    glTranslatef(0, 0, 0);
    
    // Aplicar rotación Euler (orden Z-Y-X)
    glRotatef(currentEulerZ, 0, 0, 1);
    glRotatef(currentEulerY, 0, 1, 0);
    glRotatef(currentEulerX, 1, 0, 0);
    
    drawColoredCube(1.2f);
    drawAxes(1.8f);
    glPopMatrix();
    
    // Etiqueta
    glColor3f(1, 1, 0);
    drawText(-1.5f, -3.5f, "ANGULOS DE EULER");
    if (currentDemo == 1 || (manualControl && fabs(manualEulerY - 90.0f) < 5.0f)) {
        glColor3f(1, 0, 0);
        drawText(-1.2f, -4.0f, "(GIMBAL LOCK!)");
    }
    
    // CUBO 3: CUATERNION (derecha)
    glPushMatrix();
    glTranslatef(4.0f, 0, 0);
    
    // Aplicar rotación por cuaternión
    float quatMatrix[16];
    currentQuat.toMatrix(quatMatrix);
    glMultMatrixf(quatMatrix);
    
    drawColoredCube(1.2f);
    drawAxes(1.8f);
    glPopMatrix();
    
    // Etiqueta
    glColor3f(0, 1, 1);
    drawText(3.0f, -3.5f, "CUATERNION");
    if (currentDemo == 1) {
        glColor3f(0, 1, 0);
        drawText(3.2f, -4.0f, "(Sin problemas)");
    }
    
    glutSwapBuffers();
}

// Actualización de animación
void update(int value) {
    if (animating && !manualControl) {
        animationTime += 0.025f; // Velocidad de animación
    }
    
    glutPostRedisplay();
    glutTimerFunc(20, update, 0); // ~50 FPS
}

// Manejo de teclado
void keyboard(unsigned char key, int x, int y) {
    (void)x; // Evitar warning de parámetro no usado
    (void)y; // Evitar warning de parámetro no usado
    
    switch (key) {
        case '1':
            currentDemo = 0; // Comparación normal
            manualControl = false;
            animationTime = 0;
            break;
        case '2':
            currentDemo = 1; // Demostración Gimbal Lock
            manualControl = false;
            animationTime = 0;
            break;
        case '3':
            currentDemo = 2; // Demostración interpolación
            manualControl = false;
            animationTime = 0;
            break;
        case 'm':
        case 'M':
            manualControl = !manualControl;
            if (manualControl) {
                animating = false; // Pausar animación en modo manual
                // Resetear ángulos manuales
                manualEulerX = 0.0f;
                manualEulerY = 0.0f;
                manualEulerZ = 0.0f;
            }
            break;
        case ' ':
            if (!manualControl) {
                animating = !animating;
            }
            break;
        case 'x': // Roll negativo
            if (manualControl) {
                manualEulerX -= 10.0f;
                if (manualEulerX < -180.0f) manualEulerX += 360.0f;
            }
            break;
        case 'X': // Roll positivo
            if (manualControl) {
                manualEulerX += 10.0f;
                if (manualEulerX > 180.0f) manualEulerX -= 360.0f;
            }
            break;
        case 'y': // Pitch negativo
            if (manualControl) {
                manualEulerY -= 10.0f;
                if (manualEulerY < -180.0f) manualEulerY += 360.0f;
            }
            break;
        case 'Y': // Pitch positivo
            if (manualControl) {
                manualEulerY += 10.0f;
                if (manualEulerY > 180.0f) manualEulerY -= 360.0f;
            }
            break;
        case 'z': // Yaw negativo
            if (manualControl) {
                manualEulerZ -= 10.0f;
                if (manualEulerZ < -180.0f) manualEulerZ += 360.0f;
            }
            break;
        case 'Z': // Yaw positivo
            if (manualControl) {
                manualEulerZ += 10.0f;
                if (manualEulerZ > 180.0f) manualEulerZ -= 360.0f;
            }
            break;
        case 'i':
        case 'I':
            showInfo = !showInfo;
            break;
        case 'r':
        case 'R':
            animationTime = 0;
            targetX = 30.0f;
            targetY = 45.0f;
            targetZ = 60.0f;
            manualEulerX = 0.0f;
            manualEulerY = 0.0f;
            manualEulerZ = 0.0f;
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
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Comparacion de Representaciones de Rotacion 3D");
    
    init();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    
    std::cout << "=== COMPARACION DE ROTACIONES 3D ===" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  1 - Comparacion Normal (diferencias sutiles)" << std::endl;
    std::cout << "  2 - Demostracion Gimbal Lock" << std::endl;
    std::cout << "  3 - Demostracion Interpolacion Suave" << std::endl;
    std::cout << "  M - Activar/Desactivar Control Manual" << std::endl;
    std::cout << "  ESPACIO - Pausar/Reanudar animacion (solo en modo automatico)" << std::endl;
    std::cout << "  I - Mostrar/Ocultar informacion tecnica" << std::endl;
    std::cout << "  R - Reset" << std::endl;
    std::cout << "  ESC - Salir" << std::endl;
    std::cout << std::endl;
    std::cout << "CONTROL MANUAL DE EULER:" << std::endl;
    std::cout << "  X/x - Roll (rotacion en eje X) +/-10 grados" << std::endl;
    std::cout << "  Y/y - Pitch (rotacion en eje Y) +/-10 grados" << std::endl;
    std::cout << "  Z/z - Yaw (rotacion en eje Z) +/-10 grados" << std::endl;
    std::cout << std::endl;
    std::cout << "OBSERVE LAS DIFERENCIAS:" << std::endl;
    std::cout << "- Los 3 cubos muestran la misma rotacion aplicada de diferentes formas" << std::endl;
    std::cout << "- En modo 2: Vea como Euler sufre Gimbal Lock pero Cuaternion no" << std::endl;
    std::cout << "- En modo 3: Compare interpolacion lineal vs SLERP" << std::endl;
    std::cout << "- En modo Manual: Controle Euler y vea como se sincronizan los otros" << std::endl;
    
    glutMainLoop();
    return 0;
}