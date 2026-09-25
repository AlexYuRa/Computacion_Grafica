# Skybox Textures

Para que el skybox funcione correctamente, necesitas colocar 6 imágenes en esta carpeta:

## Archivos requeridos:
- `right.jpg` - Cara derecha (+X)
- `left.jpg` - Cara izquierda (-X)  
- `top.jpg` - Cara superior (+Y)
- `bottom.jpg` - Cara inferior (-Y)
- `front.jpg` - Cara frontal (+Z)
- `back.jpg` - Cara trasera (-Z)

## Recomendaciones:
- Las imágenes deben ser cuadradas (mismo ancho y alto)
- Tamaño recomendado: 512x512 o 1024x1024 pixels
- Formato: JPG o PNG
- Las imágenes deben coincidir perfectamente en los bordes para evitar costuras

## Fuentes de skyboxes gratuitos:
- Poly Haven: https://polyhaven.com/hdris
- OpenGameArt: https://opengameart.org/art-search-advanced?keys=skybox
- Free HDR: https://www.hdrlabs.com/sibl/archive.html

## Ejemplo de uso:
```cpp
vector<string> faces = {
    "assets/skybox_textures/right.jpg",
    "assets/skybox_textures/left.jpg", 
    "assets/skybox_textures/top.jpg",
    "assets/skybox_textures/bottom.jpg",
    "assets/skybox_textures/front.jpg",
    "assets/skybox_textures/back.jpg"
};
```