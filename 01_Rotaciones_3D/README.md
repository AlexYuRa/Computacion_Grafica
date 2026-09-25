# 🎯 Visualizador de Rotaciones 3D

Este proyecto es una aplicación web interactiva que demuestra diferentes representaciones de rotación en 3D.

## 🚀 Características

- **Representaciones de Rotación:**
  - Ángulos de Euler (XYZ)
  - Cuaterniones
  - Matrices de Rotación

- **Funcionalidades:**
  - Visualización 3D interactiva con Three.js
  - Controles deslizantes para cada representación
  - Sincronización automática entre representaciones
  - Demostración de Gimbal Lock
  - Interpolación suave con cuaterniones (SLERP)
  - Modo comparación lado a lado

## 📁 Archivos

- `index.html` - Página principal
- `styles.css` - Estilos CSS
- `app.js` - Lógica principal en JavaScript

## 🎮 Cómo usar

1. Abrir `index.html` en un navegador web
2. Seleccionar tipo de representación (Euler, Cuaternión, Matriz)
3. Usar los controles deslizantes para rotar el cubo
4. Probar las demostraciones educativas
5. Activar modo comparación para ver diferencias

## 🎓 Conceptos Educativos

- **Gimbal Lock:** Problema de los ángulos de Euler
- **Cuaterniones:** Rotaciones sin singularidades
- **Matrices:** Aplicación directa de transformaciones
- **SLERP:** Interpolación esférica lineal

## 🔧 Tecnologías

- HTML5, CSS3, JavaScript ES6
- Three.js para gráficos 3D
- Matemáticas 3D avanzadas