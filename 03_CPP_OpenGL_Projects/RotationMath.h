#ifndef ROTATION_MATH_H
#define ROTATION_MATH_H

#include <cmath>
#include <iostream>
#include <string>

// Constantes matemáticas
const float PI = 3.14159265359f;
const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;

// Estructura para vectores 3D
struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
    
    float magnitude() const { return sqrt(x*x + y*y + z*z); }
    Vector3 normalize() const { 
        float mag = magnitude(); 
        return mag > 0 ? Vector3(x/mag, y/mag, z/mag) : Vector3(0, 0, 0); 
    }
    
    Vector3 cross(const Vector3& v) const {
        return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    
    float dot(const Vector3& v) const {
        return x*v.x + y*v.y + z*v.z;
    }
};

// Clase para ángulos de Euler
class EulerRotation {
private:
    float pitch, yaw, roll;  // En radianes
    
public:
    EulerRotation() : pitch(0), yaw(0), roll(0) {}
    EulerRotation(float p, float y, float r) : pitch(p), yaw(y), roll(r) {}
    
    // Setters
    void setPitch(float p) { pitch = p; }
    void setYaw(float y) { yaw = y; }
    void setRoll(float r) { roll = r; }
    void setAngles(float p, float y, float r) { pitch = p; yaw = y; roll = r; }
    
    // Getters
    float getPitch() const { return pitch; }
    float getYaw() const { return yaw; }
    float getRoll() const { return roll; }
    
    // Getters en grados
    float getPitchDeg() const { return pitch * RAD_TO_DEG; }
    float getYawDeg() const { return yaw * RAD_TO_DEG; }
    float getRollDeg() const { return roll * RAD_TO_DEG; }
    
    // Convertir a matriz de rotación
    void toMatrix(float matrix[16]) const;
    
    // Aplicar rotación a OpenGL
    void applyToOpenGL() const;
    
    // Verificar si está cerca del gimbal lock
    bool isNearGimbalLock() const { return fabs(pitch) > PI/2 - 0.1f; }
    
    std::string toString() const;
};

// Clase para cuaterniones
class Quaternion {
private:
    float w, x, y, z;
    
public:
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    
    // Crear cuaternión desde ángulos de Euler
    static Quaternion fromEuler(float pitch, float yaw, float roll);
    
    // Crear cuaternión desde eje y ángulo
    static Quaternion fromAxisAngle(const Vector3& axis, float angle);
    
    // Setters
    void set(float w, float x, float y, float z) { this->w = w; this->x = x; this->y = y; this->z = z; }
    
    // Getters
    float getW() const { return w; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }
    
    // Operaciones
    Quaternion operator*(const Quaternion& q) const;
    Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }
    float magnitude() const { return sqrt(w*w + x*x + y*y + z*z); }
    Quaternion normalize() const;
    
    // Interpolación esférica lineal (SLERP)
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
    
    // Convertir a matriz de rotación
    void toMatrix(float matrix[16]) const;
    
    // Aplicar rotación a OpenGL
    void applyToOpenGL() const;
    
    // Convertir a ángulos de Euler
    EulerRotation toEuler() const;
    
    std::string toString() const;
};

// Clase para matrices de rotación
class RotationMatrix {
private:
    float matrix[16];  // Matriz 4x4 en formato OpenGL (columna mayor)
    
public:
    RotationMatrix();
    RotationMatrix(const float m[16]);
    
    // Crear matriz identidad
    void identity();
    
    // Crear matrices de rotación básicas
    static RotationMatrix rotateX(float angle);
    static RotationMatrix rotateY(float angle);
    static RotationMatrix rotateZ(float angle);
    
    // Crear desde ángulos de Euler
    static RotationMatrix fromEuler(float pitch, float yaw, float roll);
    
    // Crear desde cuaternión
    static RotationMatrix fromQuaternion(const Quaternion& q);
    
    // Operaciones
    RotationMatrix operator*(const RotationMatrix& m) const;
    Vector3 transform(const Vector3& v) const;
    
    // Getters
    const float* getMatrix() const { return matrix; }
    void getMatrix(float m[16]) const;
    
    // Aplicar rotación a OpenGL
    void applyToOpenGL() const;
    
    // Convertir a cuaternión
    Quaternion toQuaternion() const;
    
    // Convertir a ángulos de Euler
    EulerRotation toEuler() const;
    
    std::string toString() const;
};

#endif // ROTATION_MATH_H