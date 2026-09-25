# � Demostrador de Rotaciones 3D en OpenGL

### 📖 Descripción
Aplicación educativa interactiva que demuestra las diferencias entre tres representaciones de rotación en 3D:
- **Ángulos de Euler** (Pitch, Yaw, Roll)
- **Cuaterniones** (w, x, y, z)
- **Matrices de Rotación** (3x3)

### ✨ Características Principales

#### 🎯 Representaciones de Rotación
- **Ángulos de Euler:** Intuitivos pero propensos a Gimbal Lock
- **Cuaterniones:** Sin singularidades, interpolación suave (SLERP)
- **Matrices:** Aplicación directa, composición por multiplicación

#### 🎮 Modos de Visualización
- **Vista Simple:** Un cubo con la representación seleccionada
- **Vista Comparativa:** Tres cubos lado a lado mostrando todas las representaciones
- **Sincronización Automática:** Cambios en una representación se reflejan en las otras

#### 🎨 Características Visuales
- Cubo con caras de colores para visualizar orientación
- Sistema de coordenadas (ejes X, Y, Z)
- HUD informativo con valores en tiempo real
- Advertencias de Gimbal Lock
- Iluminación y sombreado

#### 🔧 Funcionalidades Interactivas
- **Controles de Rotación:** Teclas para rotar en los tres ejes
- **Cambio de Modo:** Alternar entre representaciones
- **Auto-rotación:** Movimiento automático suave
- **Demo Gimbal Lock:** Demostración del problema de Euler
- **Interpolación SLERP:** Demostración de interpolación suave
- **Reset:** Volver a posición inicial

### 🎮 Controles

#### Navegación Principal
- **1/2/3** - Cambiar modo (Euler/Cuaternión/Matriz)
- **C** - Activar/desactivar vista comparativa
- **I** - Mostrar/ocultar información
- **R** - Reset a posición inicial
- **ESC** - Salir

#### Rotación Manual (Solo en modo Euler)
- **Q/A** - Pitch (rotación en X)
- **W/S** - Yaw (rotación en Y)  
- **E/D** - Roll (rotación en Z)

#### Demostraciones
- **G** - Demo Gimbal Lock
- **T** - Demo interpolación SLERP
- **SPACE** - Auto-rotación

### 📁 Archivos Principales

- **`RotationDemo.cpp`** - Programa principal con interfaz OpenGL/FreeGLUT
- **`RotationMath.h`** - Definiciones de clases matemáticas para rotaciones
- **`RotationMath.cpp`** - Implementación de matemáticas 3D (Euler, Cuaterniones, Matrices)

### 🔧 Compilación

```bash
g++ -Wall -Wextra -std=c++11 -O2 RotationDemo.cpp RotationMath.cpp -o RotationDemo.exe -lfreeglut -lopengl32 -lglu32 -lgdi32
```

### 🎓 Conceptos Educativos

#### Gimbal Lock
- **Problema:** Pérdida de un grado de libertad cuando un eje se alinea
- **Ocurrencia:** Ángulos de Euler cerca de ±90° en pitch
- **Visualización:** Demo integrada que muestra el problema
- **Solución:** Usar cuaterniones en su lugar

#### Interpolación SLERP
- **Concepto:** Interpolación Esférica Lineal
- **Ventaja:** Rotación suave y natural
- **Aplicación:** Animaciones y transiciones
- **Demo:** Interpolación automática entre dos orientaciones

#### Conversiones entre Representaciones
- **Euler ↔ Cuaternión:** Conversión bidireccional
- **Cuaternión ↔ Matriz:** Transformación sin pérdida
- **Matriz ↔ Euler:** Con posibles ambigüedades

### � Uso Educativo

Este programa demuestra visualmente las diferencias entre las tres representaciones principales de rotación en 3D, siendo ideal para:

- **Estudiantes de gráficos por computadora**
- **Cursos de matemáticas aplicadas** 
- **Desarrollo de videojuegos**
- **Investigación en robótica**

---
*Implementación completa en C++/OpenGL para demostrar conceptos fundamentales de rotaciones 3D.*