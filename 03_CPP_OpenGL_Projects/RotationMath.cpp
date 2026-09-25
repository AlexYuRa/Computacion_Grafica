#include "RotationMath.h"
#include <GL/gl.h>
#include <sstream>
#include <iomanip>

// ===== IMPLEMENTACIÓN EULER ROTATION =====

void EulerRotation::toMatrix(float matrix[16]) const {
    // Limpiar matriz
    for(int i = 0; i < 16; i++) matrix[i] = 0.0f;
    
    float cp = cos(pitch), sp = sin(pitch);
    float cy = cos(yaw), sy = sin(yaw);
    float cr = cos(roll), sr = sin(roll);
    
    // Matriz de rotación ZYX (Yaw-Pitch-Roll)
    matrix[0] = cy * cr;
    matrix[1] = cy * sr;
    matrix[2] = -sy;
    matrix[3] = 0;
    
    matrix[4] = sp * sy * cr - cp * sr;
    matrix[5] = sp * sy * sr + cp * cr;
    matrix[6] = sp * cy;
    matrix[7] = 0;
    
    matrix[8] = cp * sy * cr + sp * sr;
    matrix[9] = cp * sy * sr - sp * cr;
    matrix[10] = cp * cy;
    matrix[11] = 0;
    
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void EulerRotation::applyToOpenGL() const {
    glRotatef(roll * RAD_TO_DEG, 0, 0, 1);    // Z
    glRotatef(pitch * RAD_TO_DEG, 1, 0, 0);   // X
    glRotatef(yaw * RAD_TO_DEG, 0, 1, 0);     // Y
}

std::string EulerRotation::toString() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);
    ss << "Euler: P=" << getPitchDeg() << "° Y=" << getYawDeg() << "° R=" << getRollDeg() << "°";
    return ss.str();
}

// ===== IMPLEMENTACIÓN QUATERNION =====

Quaternion Quaternion::fromEuler(float pitch, float yaw, float roll) {
    float cp = cos(pitch * 0.5f), sp = sin(pitch * 0.5f);
    float cy = cos(yaw * 0.5f), sy = sin(yaw * 0.5f);
    float cr = cos(roll * 0.5f), sr = sin(roll * 0.5f);
    
    return Quaternion(
        cr * cp * cy + sr * sp * sy,  // w
        sr * cp * cy - cr * sp * sy,  // x
        cr * sp * cy + sr * cp * sy,  // y
        cr * cp * sy - sr * sp * cy   // z
    );
}

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, float angle) {
    Vector3 normalizedAxis = axis.normalize();
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = sin(halfAngle);
    
    return Quaternion(
        cos(halfAngle),
        normalizedAxis.x * sinHalfAngle,
        normalizedAxis.y * sinHalfAngle,
        normalizedAxis.z * sinHalfAngle
    );
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::normalize() const {
    float mag = magnitude();
    if (mag > 0) {
        return Quaternion(w/mag, x/mag, y/mag, z/mag);
    }
    return Quaternion(1, 0, 0, 0);
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) {
    float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
    
    // Si el producto punto es negativo, slerp tomará el camino más largo
    Quaternion q2Copy = q2;
    if (dot < 0.0f) {
        q2Copy.w = -q2Copy.w;
        q2Copy.x = -q2Copy.x;
        q2Copy.y = -q2Copy.y;
        q2Copy.z = -q2Copy.z;
        dot = -dot;
    }
    
    // Si los cuaterniones son muy similares, usar interpolación lineal
    if (dot > 0.9995f) {
        Quaternion result(
            q1.w + t * (q2Copy.w - q1.w),
            q1.x + t * (q2Copy.x - q1.x),
            q1.y + t * (q2Copy.y - q1.y),
            q1.z + t * (q2Copy.z - q1.z)
        );
        return result.normalize();
    }
    
    // Calcular ángulo entre cuaterniones
    float theta_0 = acos(dot);
    float sin_theta_0 = sin(theta_0);
    float theta = theta_0 * t;
    float sin_theta = sin(theta);
    
    float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;
    
    return Quaternion(
        s0 * q1.w + s1 * q2Copy.w,
        s0 * q1.x + s1 * q2Copy.x,
        s0 * q1.y + s1 * q2Copy.y,
        s0 * q1.z + s1 * q2Copy.z
    );
}

void Quaternion::toMatrix(float matrix[16]) const {
    // Limpiar matriz
    for(int i = 0; i < 16; i++) matrix[i] = 0.0f;
    
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;
    
    matrix[0] = 1 - 2 * (yy + zz);
    matrix[1] = 2 * (xy + wz);
    matrix[2] = 2 * (xz - wy);
    matrix[3] = 0;
    
    matrix[4] = 2 * (xy - wz);
    matrix[5] = 1 - 2 * (xx + zz);
    matrix[6] = 2 * (yz + wx);
    matrix[7] = 0;
    
    matrix[8] = 2 * (xz + wy);
    matrix[9] = 2 * (yz - wx);
    matrix[10] = 1 - 2 * (xx + yy);
    matrix[11] = 0;
    
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void Quaternion::applyToOpenGL() const {
    float matrix[16];
    toMatrix(matrix);
    glMultMatrixf(matrix);
}

EulerRotation Quaternion::toEuler() const {
    // Convertir cuaternión a ángulos de Euler
    float test = x*y + z*w;
    
    if (test > 0.499f) { // singularidad en el polo norte
        float yaw = 2 * atan2(x, w);
        float pitch = PI / 2;
        float roll = 0;
        return EulerRotation(pitch, yaw, roll);
    }
    
    if (test < -0.499f) { // singularidad en el polo sur
        float yaw = -2 * atan2(x, w);
        float pitch = -PI / 2;
        float roll = 0;
        return EulerRotation(pitch, yaw, roll);
    }
    
    float sqx = x*x;
    float sqy = y*y;
    float sqz = z*z;
    
    float yaw = atan2(2*y*w - 2*x*z, 1 - 2*sqy - 2*sqz);
    float pitch = asin(2*test);
    float roll = atan2(2*x*w - 2*y*z, 1 - 2*sqx - 2*sqz);
    
    return EulerRotation(pitch, yaw, roll);
}

std::string Quaternion::toString() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << "Quat: w=" << w << " x=" << x << " y=" << y << " z=" << z;
    return ss.str();
}

// ===== IMPLEMENTACIÓN ROTATION MATRIX =====

RotationMatrix::RotationMatrix() {
    identity();
}

RotationMatrix::RotationMatrix(const float m[16]) {
    for(int i = 0; i < 16; i++) {
        matrix[i] = m[i];
    }
}

void RotationMatrix::identity() {
    for(int i = 0; i < 16; i++) matrix[i] = 0.0f;
    matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
}

RotationMatrix RotationMatrix::rotateX(float angle) {
    float m[16] = {0};
    m[0] = m[15] = 1.0f;
    m[5] = m[10] = cos(angle);
    m[6] = sin(angle);
    m[9] = -sin(angle);
    return RotationMatrix(m);
}

RotationMatrix RotationMatrix::rotateY(float angle) {
    float m[16] = {0};
    m[5] = m[15] = 1.0f;
    m[0] = m[10] = cos(angle);
    m[8] = sin(angle);
    m[2] = -sin(angle);
    return RotationMatrix(m);
}

RotationMatrix RotationMatrix::rotateZ(float angle) {
    float m[16] = {0};
    m[10] = m[15] = 1.0f;
    m[0] = m[5] = cos(angle);
    m[1] = sin(angle);
    m[4] = -sin(angle);
    return RotationMatrix(m);
}

RotationMatrix RotationMatrix::fromEuler(float pitch, float yaw, float roll) {
    EulerRotation euler(pitch, yaw, roll);
    float m[16];
    euler.toMatrix(m);
    return RotationMatrix(m);
}

RotationMatrix RotationMatrix::fromQuaternion(const Quaternion& q) {
    float m[16];
    q.toMatrix(m);
    return RotationMatrix(m);
}

RotationMatrix RotationMatrix::operator*(const RotationMatrix& m) const {
    float result[16];
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result[i*4 + j] = 0;
            for(int k = 0; k < 4; k++) {
                result[i*4 + j] += matrix[i*4 + k] * m.matrix[k*4 + j];
            }
        }
    }
    
    return RotationMatrix(result);
}

Vector3 RotationMatrix::transform(const Vector3& v) const {
    return Vector3(
        matrix[0] * v.x + matrix[4] * v.y + matrix[8] * v.z,
        matrix[1] * v.x + matrix[5] * v.y + matrix[9] * v.z,
        matrix[2] * v.x + matrix[6] * v.y + matrix[10] * v.z
    );
}

void RotationMatrix::getMatrix(float m[16]) const {
    for(int i = 0; i < 16; i++) {
        m[i] = matrix[i];
    }
}

void RotationMatrix::applyToOpenGL() const {
    glMultMatrixf(matrix);
}

Quaternion RotationMatrix::toQuaternion() const {
    float trace = matrix[0] + matrix[5] + matrix[10];
    
    if (trace > 0) {
        float s = sqrt(trace + 1.0f) * 2; // s = 4 * qw
        return Quaternion(
            0.25f * s,
            (matrix[9] - matrix[6]) / s,
            (matrix[2] - matrix[8]) / s,
            (matrix[4] - matrix[1]) / s
        );
    } else if ((matrix[0] > matrix[5]) && (matrix[0] > matrix[10])) {
        float s = sqrt(1.0f + matrix[0] - matrix[5] - matrix[10]) * 2; // s = 4 * qx
        return Quaternion(
            (matrix[9] - matrix[6]) / s,
            0.25f * s,
            (matrix[1] + matrix[4]) / s,
            (matrix[2] + matrix[8]) / s
        );
    } else if (matrix[5] > matrix[10]) {
        float s = sqrt(1.0f + matrix[5] - matrix[0] - matrix[10]) * 2; // s = 4 * qy
        return Quaternion(
            (matrix[2] - matrix[8]) / s,
            (matrix[1] + matrix[4]) / s,
            0.25f * s,
            (matrix[6] + matrix[9]) / s
        );
    } else {
        float s = sqrt(1.0f + matrix[10] - matrix[0] - matrix[5]) * 2; // s = 4 * qz
        return Quaternion(
            (matrix[4] - matrix[1]) / s,
            (matrix[2] + matrix[8]) / s,
            (matrix[6] + matrix[9]) / s,
            0.25f * s
        );
    }
}

EulerRotation RotationMatrix::toEuler() const {
    return toQuaternion().toEuler();
}

std::string RotationMatrix::toString() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Matrix: [" << matrix[0] << " " << matrix[4] << " " << matrix[8] << "]";
    return ss.str();
}