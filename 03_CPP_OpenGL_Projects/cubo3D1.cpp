#include <GL/freeglut.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

const float PI = 3.14159265358979323846f;
const float DEG_TO_RAD = PI / 180.0f;
const float ROTATION_STEP = 5.0f * DEG_TO_RAD;
float blinkTimer = 0.0f;
float cameraRotY = 30.0f;
float cameraRotX = 20.0f;


// ============================================================================
// ESTRUCTURAS MATEMÁTICAS BÁSICAS
// ============================================================================

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    float length() const { return sqrtf(x * x + y * y + z * z); }
    Vec3 normalized() const { float len = length(); return len > 0 ? (*this) * (1.0f / len) : *this; }
};

struct Mat4 {
    float m[16];
    
    Mat4() { identity(); }
    
    void identity() {
        memset(m, 0, sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    
    static Mat4 fromEulerAngles(float pitch, float yaw, float roll) {
        float cp = cosf(pitch), sp = sinf(pitch);
        float cy = cosf(yaw), sy = sinf(yaw);
        float cr = cosf(roll), sr = sinf(roll);
        
        Mat4 result;
        result.m[0] = cy * cr + sy * sp * sr;
        result.m[1] = cy * sr - sy * sp * cr;
        result.m[2] = sy * cp;
        result.m[3] = 0;
        
        result.m[4] = -cp * sr;
        result.m[5] = cp * cr;
        result.m[6] = sp;
        result.m[7] = 0;
        
        result.m[8] = sy * cr - cy * sp * sr;
        result.m[9] = sy * sr + cy * sp * cr;
        result.m[10] = cy * cp;
        result.m[11] = 0;
        
        result.m[12] = result.m[13] = result.m[14] = 0;
        result.m[15] = 1;
        
        return result;
    }
    
    static Mat4 fromAxisAngle(const Vec3& axis, float angle) {
        Vec3 a = axis.normalized();
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - c;
        
        Mat4 result;
        result.m[0] = t * a.x * a.x + c;
        result.m[1] = t * a.x * a.y + s * a.z;
        result.m[2] = t * a.x * a.z - s * a.y;
        result.m[3] = 0;
        
        result.m[4] = t * a.x * a.y - s * a.z;
        result.m[5] = t * a.y * a.y + c;
        result.m[6] = t * a.y * a.z + s * a.x;
        result.m[7] = 0;
        
        result.m[8] = t * a.x * a.z + s * a.y;
        result.m[9] = t * a.y * a.z - s * a.x;
        result.m[10] = t * a.z * a.z + c;
        result.m[11] = 0;
        
        result.m[12] = result.m[13] = result.m[14] = 0;
        result.m[15] = 1;
        
        return result;
    }
    
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i * 4 + j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i * 4 + j] += m[k * 4 + j] * other.m[i * 4 + k];
                }
            }
        }
        return result;
    }
    
    Vec3 transformVector(const Vec3& v) const {
        return Vec3(
            m[0] * v.x + m[4] * v.y + m[8] * v.z,
            m[1] * v.x + m[5] * v.y + m[9] * v.z,
            m[2] * v.x + m[6] * v.y + m[10] * v.z
        );
    }
    
    // Interpolación lineal de matrices
    static Mat4 lerp(const Mat4& a, const Mat4& b, float t) {
        Mat4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = a.m[i] * (1.0f - t) + b.m[i] * t;
        }
        return result;
    }
    
    // Calcular "error de ortogonalidad"
    float orthogonalityError() const {
        Vec3 x(m[0], m[1], m[2]);
        Vec3 y(m[4], m[5], m[6]);
        Vec3 z(m[8], m[9], m[10]);
        
        float error = 0;
        error += fabsf(x.dot(y));
        error += fabsf(x.dot(z));
        error += fabsf(y.dot(z));
        error += fabsf(x.length() - 1.0f);
        error += fabsf(y.length() - 1.0f);
        error += fabsf(z.length() - 1.0f);
        
        return error;
    }
};

struct Quat {
    float x, y, z, w;
    
    Quat(float x = 0, float y = 0, float z = 0, float w = 1) : x(x), y(y), z(z), w(w) {}
    
    static Quat fromEulerAngles(float pitch, float yaw, float roll) {
        float cp = cosf(pitch * 0.5f), sp = sinf(pitch * 0.5f);
        float cy = cosf(yaw * 0.5f), sy = sinf(yaw * 0.5f);
        float cr = cosf(roll * 0.5f), sr = sinf(roll * 0.5f);
        
        Quat q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }
    
    static Quat fromAxisAngle(const Vec3& axis, float angle) {
        Vec3 a = axis.normalized();
        float halfAngle = angle * 0.5f;
        float s = sinf(halfAngle);
        return Quat(a.x * s, a.y * s, a.z * s, cosf(halfAngle));
    }
    
    Quat operator*(const Quat& q) const {
        return Quat(
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w,
            w * q.w - x * q.x - y * q.y - z * q.z
        );
    }
    
    float length() const {
        return sqrtf(x * x + y * y + z * z + w * w);
    }
    
    Quat normalized() const {
        float len = length();
        return len > 0 ? Quat(x / len, y / len, z / len, w / len) : *this;
    }
    
    // SLERP - Interpolación esférica
    static Quat slerp(const Quat& a, const Quat& b, float t) {
        Quat qa = a.normalized();
        Quat qb = b.normalized();
        
        float dot = qa.x * qb.x + qa.y * qb.y + qa.z * qb.z + qa.w * qb.w;
        
        // Si están muy cerca, usar interpolación lineal
        if (fabsf(dot) > 0.9995f) {
            Quat result(
                qa.x * (1.0f - t) + qb.x * t,
                qa.y * (1.0f - t) + qb.y * t,
                qa.z * (1.0f - t) + qb.z * t,
                qa.w * (1.0f - t) + qb.w * t
            );
            return result.normalized();
        }
        
        // Asegurar que tomamos el camino más corto
        if (dot < 0.0f) {
            qb.x = -qb.x;
            qb.y = -qb.y;
            qb.z = -qb.z;
            qb.w = -qb.w;
            dot = -dot;
        }
        
        dot = fminf(fmaxf(dot, -1.0f), 1.0f);
        float theta = acosf(dot);
        float sinTheta = sinf(theta);
        
        float wa = sinf((1.0f - t) * theta) / sinTheta;
        float wb = sinf(t * theta) / sinTheta;
        
        return Quat(
            qa.x * wa + qb.x * wb,
            qa.y * wa + qb.y * wb,
            qa.z * wa + qb.z * wb,
            qa.w * wa + qb.w * wb
        );
    }
    
    Mat4 toMatrix() const {
        Mat4 result;
        float xx = x * x, yy = y * y, zz = z * z;
        float xy = x * y, xz = x * z, yz = y * z;
        float wx = w * x, wy = w * y, wz = w * z;
        
        result.m[0] = 1.0f - 2.0f * (yy + zz);
        result.m[1] = 2.0f * (xy + wz);
        result.m[2] = 2.0f * (xz - wy);
        result.m[3] = 0;
        
        result.m[4] = 2.0f * (xy - wz);
        result.m[5] = 1.0f - 2.0f * (xx + zz);
        result.m[6] = 2.0f * (yz + wx);
        result.m[7] = 0;
        
        result.m[8] = 2.0f * (xz + wy);
        result.m[9] = 2.0f * (yz - wx);
        result.m[10] = 1.0f - 2.0f * (xx + yy);
        result.m[11] = 0;
        
        result.m[12] = result.m[13] = result.m[14] = 0;
        result.m[15] = 1;
        
        return result;
    }
    
    float normalizationError() const {
        return fabsf(length() - 1.0f);
    }
};

enum DemoMode {
    MODE_GIMBAL_LOCK,
    MODE_INTERPOLATION,
    MODE_ERROR_ACCUMULATION,
    MODE_ROTATION_ORDER
};

DemoMode currentMode = MODE_GIMBAL_LOCK;

// Estados para cada demostración
struct GimbalLockState {
    // Estados actuales
    float eulerPitch, eulerYaw, eulerRoll;
    Mat4 matrixRot;
    Quat quatRot;
    
    // Estados objetivo
    float eulerPitchTarget, eulerYawTarget, eulerRollTarget;
    Mat4 matrixRotTarget;
    Quat quatRotTarget;
    
    // Interpolación
    float interpT;
    bool interpolating;
    
    bool gimbalLockActive;
} gimbalState;

struct InterpolationState {
    Quat quatStart, quatEnd, quatCurrent;
    Mat4 matStart, matEnd, matCurrent;
    float eulerStart[3], eulerEnd[3], eulerCurrent[3];
    float t; // 0.0 a 1.0
    bool animating;
    
    // Guardar trayectorias para visualización
    vector<Vec3> eulerPath;
    vector<Vec3> matrixPath;
    vector<Vec3> quatPath;
} interpState;

struct ErrorState {
    Mat4 matrixRot;
    Quat quatRot;
    int rotationCount;
    float matrixError;
    float quatError;
} errorState;

struct RotationOrderState {
    float angles[3]; // pitch, yaw, roll
    Mat4 orderXYZ, orderZYX, orderYXZ;
} orderState;

// FUNCIONES AUXILIARES

float normalizeAngle(float angle) {
    const float TWO_PI = 2.0f * PI;
    angle = fmodf(angle, TWO_PI);
    if (angle > PI) angle -= TWO_PI;
    else if (angle < -PI) angle += TWO_PI;
    return angle;
}

void drawCube(float size) {
    float s = size / 2.0f;
    glBegin(GL_QUADS);
    
    glNormal3f(0, 0, 1);
    glVertex3f(-s, -s, s); glVertex3f(s, -s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);
    
    glNormal3f(0, 0, -1);
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, -s, -s);
    
    glNormal3f(0, 1, 0);
    glVertex3f(-s, s, -s); glVertex3f(-s, s, s);
    glVertex3f(s, s, s); glVertex3f(s, s, -s);
    
    glNormal3f(0, -1, 0);
    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    
    glNormal3f(1, 0, 0);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, s); glVertex3f(s, -s, s);
    
    glNormal3f(-1, 0, 0);
    glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);
    
    glEnd();
}

void drawAxes(float length) {
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    
    glColor3f(1, 0, 0); // X - Rojo
    glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
    
    glColor3f(0, 1, 0); // Y - Verde
    glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
    
    glColor3f(0, 0, 1); // Z - Azul
    glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
    
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// Función auxiliar para configuración de texto 2D
struct TextMode {
    GLint originalMode;
    
    TextMode() {
        glGetIntegerv(GL_MATRIX_MODE, &originalMode);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1000, 0, 700);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
    }
    
    ~TextMode() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(originalMode);
    }
};

void drawText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_12) {
    TextMode textSetup;
    
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 

// MODO 1: GIMBAL LOCK

void updateGimbalLock() {
    gimbalState.gimbalLockActive = (fabsf(gimbalState.eulerPitch) > 89.5f * DEG_TO_RAD);
    
    // Interpolación suave cuando hay cambios
    if (gimbalState.interpolating) {
        gimbalState.interpT += 0.08f; // Velocidad de interpolación
        
        if (gimbalState.interpT >= 1.0f) {
            gimbalState.interpT = 1.0f;
            gimbalState.interpolating = false;
            
            // Actualizar valores actuales
            gimbalState.eulerPitch = gimbalState.eulerPitchTarget;
            gimbalState.eulerYaw = gimbalState.eulerYawTarget;
            gimbalState.eulerRoll = gimbalState.eulerRollTarget;
            gimbalState.matrixRot = gimbalState.matrixRotTarget;
            gimbalState.quatRot = gimbalState.quatRotTarget;
        } else {
            // Interpolar Euler (lineal)
            gimbalState.eulerPitch = gimbalState.eulerPitch * (1.0f - gimbalState.interpT) + 
                                     gimbalState.eulerPitchTarget * gimbalState.interpT;
            gimbalState.eulerYaw = gimbalState.eulerYaw * (1.0f - gimbalState.interpT) + 
                                   gimbalState.eulerYawTarget * gimbalState.interpT;
            gimbalState.eulerRoll = gimbalState.eulerRoll * (1.0f - gimbalState.interpT) + 
                                    gimbalState.eulerRollTarget * gimbalState.interpT;
            
            // Interpolar Matriz (LERP - para demostración)
            gimbalState.matrixRot = Mat4::lerp(gimbalState.matrixRot, gimbalState.matrixRotTarget, gimbalState.interpT);
            
            // Interpolar Quaternion (SLERP - correcto)
            gimbalState.quatRot = Quat::slerp(gimbalState.quatRot, gimbalState.quatRotTarget, gimbalState.interpT);
        }
    }
}

void renderGimbalLock() {
    char buf[256];
    
    // Título
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    drawText(300, 670, "MODO 1: GIMBAL LOCK", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(180, 640, "Observa como EULER pierde libertad en gimbal lock, pero Matriz/Quat NO", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
    
    // CUBO EULER
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    glRotatef(gimbalState.eulerRoll * 180.0f / PI, 0, 0, 1);
    glRotatef(gimbalState.eulerYaw * 180.0f / PI, 0, 1, 0);
    glRotatef(gimbalState.eulerPitch * 180.0f / PI, 1, 0, 0);
    drawAxes(1.2f);
    if (gimbalState.gimbalLockActive) {
        glDisable(GL_LIGHTING);
        glLineWidth(4.0f);
        glColor3f(1, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawCube(0.6f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.0f);
        glEnable(GL_LIGHTING);
    }
    glColor3f(0.7f, 0.3f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO MATRIZ
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glMultMatrixf(gimbalState.matrixRot.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.7f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO QUATERNION
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    Mat4 qmat = gimbalState.quatRot.toMatrix();
    glMultMatrixf(qmat.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.3f, 0.7f);
    drawCube(0.5f);
    glPopMatrix();
    
    // Labels
    glDisable(GL_LIGHTING);
    glColor3f(gimbalState.gimbalLockActive ? 1.0f : 1.0f, 
              gimbalState.gimbalLockActive ? 0.0f : 1.0f, 
              gimbalState.gimbalLockActive ? 0.0f : 1.0f);
    drawText(80, 600, "EULER (LERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(50, 575, "Interpolacion irregular", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 1.0f, 0.5f);
    drawText(370, 600, "MATRIZ (LERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(350, 575, "Pierde ortogonalidad", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 0.5f, 1.0f);
    drawText(650, 600, "QUATERNION (SLERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(650, 575, "Interpolacion suave!", GLUT_BITMAP_HELVETICA_12);
    
    // Info
    sprintf(buf, "Pitch: %.1f | Yaw: %.1f | Roll: %.1f", 
            gimbalState.eulerPitchTarget * 180.0f / PI,
            gimbalState.eulerYawTarget * 180.0f / PI,
            gimbalState.eulerRollTarget * 180.0f / PI);
    drawText(10, 550, buf);
    
    if (gimbalState.gimbalLockActive) {
        glColor3f(1, 0, 0);
        drawText(200, 520, "!! GIMBAL LOCK ACTIVO EN EULER !!", GLUT_BITMAP_HELVETICA_18);
        glColor3f(1, 1, 0);
        drawText(150, 495, "Euler pierde un grado de libertad. Matriz/Quat funcionan correctamente", GLUT_BITMAP_HELVETICA_12);
    }
    
    if (gimbalState.interpolating) {
        glColor3f(0, 1, 1);
        sprintf(buf, "Interpolando... %.0f%%", gimbalState.interpT * 100);
        drawText(400, 470, buf);
    }
    
    glColor3f(1, 1, 0);
    drawText(200, 445, "NOTA: Observa como cada metodo interpola diferente al rotar!", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 80, "Q/A: Pitch  W/S: Yaw  E/D: Roll  |  G: Forzar Gimbal  R: Reset", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
}

// MODO 2: INTERPOLACIÓN

void updateInterpolation() {
    if (interpState.animating) {
        interpState.t += 0.005f; // Más lento para ver mejor
        if (interpState.t >= 1.0f) {
            interpState.t = 1.0f;
            interpState.animating = false;
        }
        
        // Interpolación Euler
        for (int i = 0; i < 3; i++) {
            interpState.eulerCurrent[i] = interpState.eulerStart[i] * (1.0f - interpState.t) + 
                                          interpState.eulerEnd[i] * interpState.t;
        }
        
        // Interpolación Matriz
        interpState.matCurrent = Mat4::lerp(interpState.matStart, interpState.matEnd, interpState.t);
        
        // Interpolación Quaternion
        interpState.quatCurrent = Quat::slerp(interpState.quatStart, interpState.quatEnd, interpState.t);
        
        // Guardar posiciones para trazar trayectoria
        if ((int)(interpState.t * 200) % 2 == 0) { // Guardar cada ciertos frames
            // Euler
            Mat4 eulerMat = Mat4::fromEulerAngles(
                interpState.eulerCurrent[0],
                interpState.eulerCurrent[1],
                interpState.eulerCurrent[2]
            );
            Vec3 eulerPoint = eulerMat.transformVector(Vec3(0, 0, 0.7f));
            eulerPoint.x -= 2.5f; // Offset del cubo
            interpState.eulerPath.push_back(eulerPoint);
            
            // Matriz
            Vec3 matPoint = interpState.matCurrent.transformVector(Vec3(0, 0, 0.7f));
            interpState.matrixPath.push_back(matPoint);
            
            // Quaternion
            Mat4 quatMat = interpState.quatCurrent.toMatrix();
            Vec3 quatPoint = quatMat.transformVector(Vec3(0, 0, 0.7f));
            quatPoint.x += 2.5f; // Offset del cubo
            interpState.quatPath.push_back(quatPoint);
        }
    }
}

void renderInterpolation() {
    char buf[256];
    
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    drawText(250, 670, "MODO 2: INTERPOLACION DE ROTACIONES", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(200, 640, "De 0deg a (45,60,30)deg - Observa las diferentes trayectorias", GLUT_BITMAP_HELVETICA_12);
    
    // Dibujar trayectorias
    glLineWidth(2.0f);
    
    // Trayectoria Euler - Rojo
    if (interpState.eulerPath.size() > 1) {
        glColor3f(1.0f, 0.3f, 0.3f);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : interpState.eulerPath) {
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
    
    // Trayectoria Matriz - Verde
    if (interpState.matrixPath.size() > 1) {
        glColor3f(0.3f, 1.0f, 0.3f);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : interpState.matrixPath) {
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
    
    // Trayectoria Quaternion - Azul
    if (interpState.quatPath.size() > 1) {
        glColor3f(0.3f, 0.3f, 1.0f);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : interpState.quatPath) {
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
    }
    
    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
    
    // CUBO EULER
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    glRotatef(interpState.eulerCurrent[2] * 180.0f / PI, 0, 0, 1);
    glRotatef(interpState.eulerCurrent[1] * 180.0f / PI, 0, 1, 0);
    glRotatef(interpState.eulerCurrent[0] * 180.0f / PI, 1, 0, 0);
    drawAxes(1.2f);
    glColor3f(0.7f, 0.3f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO MATRIZ
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glMultMatrixf(interpState.matCurrent.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.7f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO QUATERNION
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    Mat4 qmat = interpState.quatCurrent.toMatrix();
    glMultMatrixf(qmat.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.3f, 0.7f);
    drawCube(0.5f);
    glPopMatrix();
    
    // Labels
    glDisable(GL_LIGHTING);
    glColor3f(1, 0.5f, 0.5f);
    drawText(100, 600, "EULER (LERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(70, 575, "Interpolacion lineal", GLUT_BITMAP_HELVETICA_12);
    drawText(80, 560, "Trayectoria irregular", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 1, 0.5f);
    drawText(380, 600, "MATRIZ (LERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(350, 575, "Interpolacion lineal", GLUT_BITMAP_HELVETICA_12);
    drawText(340, 560, "Pierde ortogonalidad", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 0.5f, 1);
    drawText(650, 600, "QUATERNION (SLERP)", GLUT_BITMAP_HELVETICA_18);
    drawText(630, 575, "Interpolacion esferica", GLUT_BITMAP_HELVETICA_12);
    drawText(640, 560, "Camino mas corto!", GLUT_BITMAP_HELVETICA_12);
    
    // Progress bar
    glColor3f(0.5f, 0.5f, 0.5f);
    sprintf(buf, "Progreso: %.0f%%", interpState.t * 100);
    drawText(400, 520, buf);
    
    // Barra de progreso
    glBegin(GL_LINE_LOOP);
    glVertex2f(300, 500);
    glVertex2f(700, 500);
    glVertex2f(700, 490);
    glVertex2f(300, 490);
    glEnd();
    
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex2f(300, 500);
    glVertex2f(300 + 400 * interpState.t, 500);
    glVertex2f(300 + 400 * interpState.t, 490);
    glVertex2f(300, 490);
    glEnd();
    
    glColor3f(1, 1, 0);
    drawText(250, 465, "Las lineas muestran el recorrido de un punto del objeto", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 80, "ESPACIO: Iniciar/Reiniciar animacion  |  R: Reset posiciones", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
}

// MODO 3: ACUMULACIÓN DE ERRORES

void updateErrorAccumulation() {
    errorState.matrixError = errorState.matrixRot.orthogonalityError();
    errorState.quatError = errorState.quatRot.normalizationError();
}

void renderErrorAccumulation() {
    char buf[256];
    
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    drawText(230, 670, "MODO 3: ACUMULACION DE ERROR NUMERICO", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(180, 640, "Presiona ESPACIO repetidamente - Las matrices pierden ortogonalidad", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
    
    // CUBO MATRIZ (sin normalización)
    glPushMatrix();
    glTranslatef(-1.5f, 0, 0);
    glMultMatrixf(errorState.matrixRot.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.7f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO QUATERNION (con normalización)
    glPushMatrix();
    glTranslatef(1.5f, 0, 0);
    Mat4 qmat = errorState.quatRot.toMatrix();
    glMultMatrixf(qmat.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.3f, 0.7f);
    drawCube(0.5f);
    glPopMatrix();
    
    // Labels y errores
    glDisable(GL_LIGHTING);
    glColor3f(0.5f, 1, 0.5f);
    drawText(250, 600, "MATRIZ (sin normalizar)", GLUT_BITMAP_HELVETICA_18);
    sprintf(buf, "Error ortogonalidad: %.6f", errorState.matrixError);
    glColor3f(errorState.matrixError > 0.01f ? 1.0f : 0.5f, 
              errorState.matrixError > 0.01f ? 0.0f : 1.0f, 0.5f);
    drawText(230, 575, buf, GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 0.5f, 1);
    drawText(520, 600, "QUATERNION (normalizado)", GLUT_BITMAP_HELVETICA_18);
    sprintf(buf, "Error normalizacion: %.6f", errorState.quatError);
    glColor3f(0.5f, 1, 0.5f);
    drawText(530, 575, buf, GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(1, 1, 1);
    sprintf(buf, "Rotaciones aplicadas: %d", errorState.rotationCount);
    drawText(400, 520, buf);
    
    if (errorState.matrixError > 0.01f) {
        glColor3f(1, 0, 0);
        drawText(200, 490, "ADVERTENCIA: La matriz esta perdiendo ortogonalidad!", GLUT_BITMAP_HELVETICA_12);
        glColor3f(1, 1, 0);
        drawText(250, 465, "En aplicaciones reales se debe re-ortogonalizar", GLUT_BITMAP_HELVETICA_12);
    }
    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 80, "ESPACIO: Aplicar rotacion aleatoria  |  R: Reset", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
}

// MODO 4: ORDEN DE ROTACIÓN

void updateRotationOrder() {
    float p = orderState.angles[0];
    float y = orderState.angles[1];
    float r = orderState.angles[2];
    
    float cp = cosf(p), sp = sinf(p);
    float cy = cosf(y), sy = sinf(y);
    float cr = cosf(r), sr = sinf(r);
    
    // Orden XYZ (Pitch -> Yaw -> Roll)
    Mat4 rx, ry, rz;
    rx.m[5] = cp; rx.m[6] = sp;
    rx.m[9] = -sp; rx.m[10] = cp;
    
    ry.m[0] = cy; ry.m[2] = -sy;
    ry.m[8] = sy; ry.m[10] = cy;
    
    rz.m[0] = cr; rz.m[1] = sr;
    rz.m[4] = -sr; rz.m[5] = cr;
    
    orderState.orderXYZ = rz * (ry * rx);
    
    // Orden ZYX (Roll -> Yaw -> Pitch)
    orderState.orderZYX = rx * (ry * rz);
    
    // Orden YXZ (Yaw -> Pitch -> Roll)
    orderState.orderYXZ = rz * (rx * ry);
}

void renderRotationOrder() {
    char buf[256];
    
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 0);
    drawText(250, 670, "MODO 4: ORDEN DE ROTACION (EULER)", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.8f, 0.8f, 0.8f);
    drawText(150, 640, "Mismos angulos, diferente orden = DIFERENTES orientaciones finales", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
    
    // CUBO XYZ
    glPushMatrix();
    glTranslatef(-2.5f, 0, 0);
    glMultMatrixf(orderState.orderXYZ.m);
    drawAxes(1.2f);
    glColor3f(0.7f, 0.3f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO ZYX
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glMultMatrixf(orderState.orderZYX.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.7f, 0.3f);
    drawCube(0.5f);
    glPopMatrix();
    
    // CUBO YXZ
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    glMultMatrixf(orderState.orderYXZ.m);
    drawAxes(1.2f);
    glColor3f(0.3f, 0.3f, 0.7f);
    drawCube(0.5f);
    glPopMatrix();
    
    // Labels
    glDisable(GL_LIGHTING);
    glColor3f(1, 0.5f, 0.5f);
    drawText(100, 600, "ORDEN XYZ", GLUT_BITMAP_HELVETICA_18);
    drawText(70, 575, "(Pitch->Yaw->Roll)", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 1, 0.5f);
    drawText(400, 600, "ORDEN ZYX", GLUT_BITMAP_HELVETICA_18);
    drawText(370, 575, "(Roll->Yaw->Pitch)", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.5f, 0.5f, 1);
    drawText(700, 600, "ORDEN YXZ", GLUT_BITMAP_HELVETICA_18);
    drawText(670, 575, "(Yaw->Pitch->Roll)", GLUT_BITMAP_HELVETICA_12);
    
    // Ángulos actuales
    glColor3f(1, 1, 1);
    sprintf(buf, "Angulos: Pitch=%.1f  Yaw=%.1f  Roll=%.1f", 
            orderState.angles[0] * 180.0f / PI,
            orderState.angles[1] * 180.0f / PI,
            orderState.angles[2] * 180.0f / PI);
    drawText(250, 520, buf);
    
    glColor3f(1, 1, 0);
    drawText(180, 490, "Nota: Matrices y Quaternions NO dependen del orden!", GLUT_BITMAP_HELVETICA_12);
    drawText(200, 465, "Solo Euler angles tiene este problema de ambiguedad", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 80, "Q/A: Pitch  W/S: Yaw  E/D: Roll  |  R: Reset", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 2, 8, 0, 0, 0, 0, 1, 0);
    
    glRotatef(cameraRotX, 1, 0, 0);
    glRotatef(cameraRotY, 0, 1, 0);
    
    // Renderizar según el modo activo
    switch (currentMode) {
        case MODE_GIMBAL_LOCK:
            renderGimbalLock();
            break;
        case MODE_INTERPOLATION:
            renderInterpolation();
            break;
        case MODE_ERROR_ACCUMULATION:
            renderErrorAccumulation();
            break;
        case MODE_ROTATION_ORDER:
            renderRotationOrder();
            break;
    }
    
    // Selector de modo
    glDisable(GL_LIGHTING);
    glColor3f(0.5f, 0.5f, 0.5f);
    drawText(10, 50, "Cambiar modo: [1]Gimbal Lock  [2]Interpolacion  [3]Error  [4]Orden", GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0, 1, 1);
    char modeName[256];
    sprintf(modeName, "MODO ACTUAL: %d/%d", (int)currentMode + 1, 4);
    drawText(10, 30, modeName, GLUT_BITMAP_HELVETICA_12);
    
    glColor3f(0.7f, 0.7f, 0.7f);
    drawText(10, 10, "Camara: Flechas  |  ESC: Salir", GLUT_BITMAP_HELVETICA_12);
    glEnable(GL_LIGHTING);
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    // Cambiar modo
    if (key >= '1' && key <= '4') {
        currentMode = (DemoMode)(key - '1');
        glutPostRedisplay();
        return;
    }
    
    switch (currentMode) {
        case MODE_GIMBAL_LOCK:
            switch (key) {
                case 'q': case 'Q':
                    gimbalState.eulerPitchTarget += ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(1,0,0), ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(1,0,0), ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'a': case 'A':
                    gimbalState.eulerPitchTarget -= ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(1,0,0), -ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(1,0,0), -ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'w': case 'W':
                    gimbalState.eulerYawTarget += ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(0,1,0), ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(0,1,0), ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 's': case 'S':
                    gimbalState.eulerYawTarget -= ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(0,1,0), -ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(0,1,0), -ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'e': case 'E':
                    gimbalState.eulerRollTarget += ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(0,0,1), ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(0,0,1), ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'd': case 'D':
                    gimbalState.eulerRollTarget -= ROTATION_STEP;
                    gimbalState.matrixRotTarget = Mat4::fromAxisAngle(Vec3(0,0,1), -ROTATION_STEP) * gimbalState.matrixRotTarget;
                    gimbalState.quatRotTarget = (Quat::fromAxisAngle(Vec3(0,0,1), -ROTATION_STEP) * gimbalState.quatRotTarget).normalized();
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'g': case 'G':
                    gimbalState.eulerPitchTarget = 90.0f * DEG_TO_RAD;
                    gimbalState.interpT = 0.0f;
                    gimbalState.interpolating = true;
                    break;
                case 'r': case 'R':
                    gimbalState.eulerPitch = gimbalState.eulerYaw = gimbalState.eulerRoll = 0;
                    gimbalState.eulerPitchTarget = gimbalState.eulerYawTarget = gimbalState.eulerRollTarget = 0;
                    gimbalState.matrixRot.identity();
                    gimbalState.matrixRotTarget.identity();
                    gimbalState.quatRot = Quat(0,0,0,1);
                    gimbalState.quatRotTarget = Quat(0,0,0,1);
                    gimbalState.interpolating = false;
                    break;
            }
            gimbalState.eulerPitchTarget = normalizeAngle(gimbalState.eulerPitchTarget);
            gimbalState.eulerYawTarget = normalizeAngle(gimbalState.eulerYawTarget);
            gimbalState.eulerRollTarget = normalizeAngle(gimbalState.eulerRollTarget);
            updateGimbalLock();
            break;
            
        case MODE_INTERPOLATION:
            switch (key) {
                case ' ':
                    interpState.t = 0.0f;
                    interpState.animating = true;
                    // Limpiar trayectorias
                    interpState.eulerPath.clear();
                    interpState.matrixPath.clear();
                    interpState.quatPath.clear();
                    break;
                case 'r': case 'R':
                    interpState.t = 0.0f;
                    interpState.animating = false;
                    interpState.eulerPath.clear();
                    interpState.matrixPath.clear();
                    interpState.quatPath.clear();
                    for (int i = 0; i < 3; i++) {
                        interpState.eulerStart[i] = 0;
                        interpState.eulerEnd[i] = 0;
                        interpState.eulerCurrent[i] = 0;
                    }
                    interpState.eulerEnd[0] = 45.0f * DEG_TO_RAD;
                    interpState.eulerEnd[1] = 60.0f * DEG_TO_RAD;
                    interpState.eulerEnd[2] = 30.0f * DEG_TO_RAD;
                    
                    interpState.matStart.identity();
                    interpState.matEnd = Mat4::fromEulerAngles(
                        interpState.eulerEnd[0],
                        interpState.eulerEnd[1],
                        interpState.eulerEnd[2]
                    );
                    interpState.matCurrent = interpState.matStart;
                    
                    interpState.quatStart = Quat(0,0,0,1);
                    interpState.quatEnd = Quat::fromEulerAngles(
                        interpState.eulerEnd[0],
                        interpState.eulerEnd[1],
                        interpState.eulerEnd[2]
                    );
                    interpState.quatCurrent = interpState.quatStart;
                    break;
            }
            break;
            
        case MODE_ERROR_ACCUMULATION:
            switch (key) {
                case ' ':
                    // Aplicar rotación aleatoria pequeña
                    {
                        Vec3 randomAxis(
                            (rand() % 200 - 100) / 100.0f,
                            (rand() % 200 - 100) / 100.0f,
                            (rand() % 200 - 100) / 100.0f
                        );
                        randomAxis = randomAxis.normalized();
                        float angle = 10.0f * DEG_TO_RAD;
                        
                        errorState.matrixRot = Mat4::fromAxisAngle(randomAxis, angle) * errorState.matrixRot;
                        Quat deltaQuat = Quat::fromAxisAngle(randomAxis, angle);
                        errorState.quatRot = (deltaQuat * errorState.quatRot).normalized();
                        errorState.rotationCount++;
                    }
                    break;
                case 'r': case 'R':
                    errorState.matrixRot.identity();
                    errorState.quatRot = Quat(0,0,0,1);
                    errorState.rotationCount = 0;
                    break;
            }
            updateErrorAccumulation();
            break;
            
        case MODE_ROTATION_ORDER:
            switch (key) {
                case 'q': case 'Q': orderState.angles[0] += ROTATION_STEP; break;
                case 'a': case 'A': orderState.angles[0] -= ROTATION_STEP; break;
                case 'w': case 'W': orderState.angles[1] += ROTATION_STEP; break;
                case 's': case 'S': orderState.angles[1] -= ROTATION_STEP; break;
                case 'e': case 'E': orderState.angles[2] += ROTATION_STEP; break;
                case 'd': case 'D': orderState.angles[2] -= ROTATION_STEP; break;
                case 'r': case 'R':
                    orderState.angles[0] = orderState.angles[1] = orderState.angles[2] = 0;
                    break;
            }
            for (int i = 0; i < 3; i++) {
                orderState.angles[i] = normalizeAngle(orderState.angles[i]);
            }
            updateRotationOrder();
            break;
    }
    
    if (key == 27) exit(0); // ESC
    
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP: cameraRotX += 5; break;
        case GLUT_KEY_DOWN: cameraRotX -= 5; break;
        case GLUT_KEY_LEFT: cameraRotY -= 5; break;
        case GLUT_KEY_RIGHT: cameraRotY += 5; break;
    }
    glutPostRedisplay();
}

void update(int value) {
    blinkTimer += 0.016f;
    
    if (currentMode == MODE_INTERPOLATION) {
        updateInterpolation();
    } else if (currentMode == MODE_GIMBAL_LOCK) {
        updateGimbalLock();
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void init() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    float lightPos[] = {5.0f, 5.0f, 10.0f, 1.0f};
    float lightAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float lightDif[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    
    // Inicializar estados
    gimbalState.eulerPitch = gimbalState.eulerYaw = gimbalState.eulerRoll = 0;
    gimbalState.eulerPitchTarget = gimbalState.eulerYawTarget = gimbalState.eulerRollTarget = 0;
    gimbalState.matrixRot.identity();
    gimbalState.matrixRotTarget.identity();
    gimbalState.quatRot = Quat(0,0,0,1);
    gimbalState.quatRotTarget = Quat(0,0,0,1);
    gimbalState.gimbalLockActive = false;
    gimbalState.interpT = 0.0f;
    gimbalState.interpolating = false;
    
    interpState.t = 0;
    interpState.animating = false;
    for (int i = 0; i < 3; i++) {
        interpState.eulerStart[i] = 0;
        interpState.eulerCurrent[i] = 0;
    }
    interpState.eulerEnd[0] = 45.0f * DEG_TO_RAD;
    interpState.eulerEnd[1] = 60.0f * DEG_TO_RAD;
    interpState.eulerEnd[2] = 30.0f * DEG_TO_RAD;
    interpState.matStart.identity();
    interpState.matEnd = Mat4::fromEulerAngles(
        interpState.eulerEnd[0],
        interpState.eulerEnd[1],
        interpState.eulerEnd[2]
    );
    interpState.matCurrent = interpState.matStart;
    interpState.quatStart = Quat(0,0,0,1);
    interpState.quatEnd = Quat::fromEulerAngles(
        interpState.eulerEnd[0],
        interpState.eulerEnd[1],
        interpState.eulerEnd[2]
    );
    interpState.quatCurrent = interpState.quatStart;
    
    errorState.matrixRot.identity();
    errorState.quatRot = Quat(0,0,0,1);
    errorState.rotationCount = 0;
    errorState.matrixError = 0;
    errorState.quatError = 0;
    
    for (int i = 0; i < 3; i++) {
        orderState.angles[i] = 0;
    }
    updateRotationOrder();
}

int main(int argc, char** argv) {
    cout << "=== DEMOSTRACION COMPLETA: EULER vs MATRIZ vs QUATERNION ===" << endl;
    cout << "\n4 MODOS DE DEMOSTRACION:" << endl;
    cout << "\n[1] GIMBAL LOCK" << endl;
    cout << "    - Demuestra como Euler pierde un grado de libertad" << endl;
    cout << "    - Matriz y Quaternion NO sufren este problema" << endl;
    cout << "\n[2] INTERPOLACION" << endl;
    cout << "    - Euler: Interpolacion lineal (trayectoria irregular)" << endl;
    cout << "    - Matriz: LERP (pierde ortogonalidad)" << endl;
    cout << "    - Quaternion: SLERP (trayectoria optima y suave)" << endl;
    cout << "\n[3] ACUMULACION DE ERROR" << endl;
    cout << "    - Matrices acumulan error y pierden ortogonalidad" << endl;
    cout << "    - Quaternions se pueden normalizar facilmente" << endl;
    cout << "\n[4] ORDEN DE ROTACION" << endl;
    cout << "    - Euler depende del ORDEN (XYZ vs ZYX vs YXZ)" << endl;
    cout << "    - Mismos angulos = diferentes orientaciones finales" << endl;
    cout << "    - Matriz/Quaternion NO tienen este problema" << endl;
    cout << "\nCONTROLES:" << endl;
    cout << "  1/2/3/4 - Cambiar modo de demostracion" << endl;
    cout << "  Flechas - Rotar camara" << endl;
    cout << "  ESC     - Salir" << endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Demo Completa: Euler vs Matriz vs Quaternion");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, update, 0);
    
    glutMainLoop();
    return 0;
}