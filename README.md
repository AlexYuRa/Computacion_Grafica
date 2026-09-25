# Proyectos de Computación Gráfica

Colección de proyectos de gráficos 3D, rotaciones y OpenGL desarrollados en C++, JavaScript y React.

| Carpeta | Descripción | Tecnologías |
|---|---|---|
| [01_Rotaciones_3D](01_Rotaciones_3D/) | Visualizador web interactivo de rotaciones 3D: ángulos de Euler, cuaterniones, matrices, Gimbal Lock y SLERP. | HTML, CSS, JavaScript, Three.js |
| [03_CPP_OpenGL_Projects](03_CPP_OpenGL_Projects/) | Programas de OpenGL/FreeGLUT: demostrador de rotaciones, brazo 3D, cubos 3D y dibujos 2D. | C++, OpenGL, FreeGLUT |
| [04_Documentacion_y_Notas](04_Documentacion_y_Notas/) | Notas y fragmentos de código de apoyo. | — |
| [Cuadrupedo_IIIU](Cuadrupedo_IIIU/) | Simulador 3D de implantes dentales. Es un submódulo que apunta a [Dental-Implant-Simulator](https://github.com/AlexYuRa/Dental-Implant-Simulator). | C++17, OpenGL 4.6, CMake |
| [cuaterniones](cuaterniones/) | Presentación interactiva sobre cuaterniones. | React, TypeScript, Vite, Tailwind |
| [Obj_tas](Obj_tas/) | Personaje articulado (Freddy) armado a partir de modelos `.obj` por partes, con animación y tres cámaras orbitales. | C++, OpenGL, GLUT |
| [Skybox](Skybox/) | Escena con skybox y carga de un modelo animado `.glb` con huesos. | C++, OpenGL 4.6, GLFW, GLAD, GLM |

## Clonar

`Cuadrupedo_IIIU` es un submódulo, así que hay que clonar con:

```bash
git clone --recurse-submodules <url-de-este-repo>
```

Si ya lo clonaste sin submódulos:

```bash
git submodule update --init
```

## Compilar

### Proyectos con FreeGLUT (`03_CPP_OpenGL_Projects`, `Obj_tas`)

Con MSYS2 / MinGW-w64:

```bash
g++ -std=c++11 -O2 archivo.cpp -o archivo.exe -lfreeglut -lopengl32 -lglu32 -lgdi32
```

`Obj_tas` busca los `.obj` en la carpeta de trabajo, así que el ejecutable se debe correr desde `Obj_tas/`. Controles: `Espacio` pausa, `1/2/3` cambian de cámara, `Z/X/C` eligen qué cámara mover, `A/D` y `Q/E` (o las flechas) la rotan, `W/S` acercan o alejan.

### `cuaterniones`

```bash
cd cuaterniones
npm install
npm run dev
```

### `01_Rotaciones_3D`

Abrir `index.html` en el navegador.
