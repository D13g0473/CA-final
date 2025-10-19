# Procesamiento de Imágenes Cross-Fading - Algoritmo Secuencial

## Primera Entrega: Implementación Secuencial

Este repositorio contiene la primera entrega del Trabajo Práctico Final de la materia **Computación Avanzada (CA)**, correspondiente a la implementación secuencial del algoritmo de cross-fading de imágenes.

## Descripción del Problema

El algoritmo implementa una transición suave (cross-fading) entre una imagen a color y su versión en escala de grises. La transición se realiza generando 96 frames (equivalentes a 4 segundos de video a 24 fps), donde gradualmente la imagen color se desvanece mientras aparece la versión en escala de grises.

### Algoritmo de Cross-Fading

Para cada píxel en la posición (x,y), se calcula:

```
result[x][y].r = color[x][y].r * P + gray[x][y].r * (1-P)
result[x][y].g = color[x][y].g * P + gray[x][y].g * (1-P)
result[x][y].b = color[x][y].b * P + gray[x][y].b * (1-P)
```

Donde:
- `P` varía de 1.0 a 0.0 a lo largo de los 96 frames
- `color[x][y]` es el píxel original RGB
- `gray[x][y]` es el píxel en escala de grises (convertido a RGB)

## Estructura del Proyecto

```
.
├── sequential.cpp              # Implementación secuencial
├── README.md                   # Este archivo
├── IMG-20250919-WA0001.jpg     # Imagen de prueba 1
├── IMG-20250929-WA0019.jpg     # Imagen de prueba 2
└── IMG-20250929-WA0019.png     # Imagen de prueba 3
```

## Requisitos del Sistema

### Dependencias

- **Compilador C++**: GCC con soporte para C++11
- **OpenCV 4.x**: Para procesamiento de imágenes
- **Sistema Operativo**: Ubuntu/Debian o similar

### Instalación de Dependencias

```bash
# Actualizar el sistema
sudo apt update

# Instalar OpenCV
sudo apt install libopencv-dev

# Verificar instalación
pkg-config --modversion opencv4
```

## Compilación

Para compilar el programa secuencial:

```bash
g++ -std=c++11 sequential.cpp -o sequential `pkg-config --cflags --libs opencv4`
```

Este comando:
- Usa el estándar C++11
- Compila `sequential.cpp`
- Genera el ejecutable `sequential`
- Enlaza automáticamente con las librerías de OpenCV

## Ejecución

### Sintaxis Básica

```bash
./sequential <ruta_a_la_imagen>
```

### Ejemplos con las Imágenes del Repositorio

#### 1. Imagen JPG (IMG-20250919-WA0001.jpg)

```bash
./sequential IMG-20250919-WA0001.jpg
```

#### 2. Imagen JPG (IMG-20250929-WA0019.jpg)

```bash
./sequential IMG-20250929-WA0019.jpg
```

#### 3. Imagen PNG (IMG-20250929-WA0019.png)

```bash
./sequential IMG-20250929-WA0019.png
```

### Salida del Programa

El programa generará:
- 96 archivos de imagen: `frame_0.png`, `frame_1.png`, ..., `frame_95.png`
- Un mensaje con el tiempo de ejecución en milisegundos

```
Tiempo de ejecucion secuencial: 548 ms
```

## Descripción del Código

### Librerías Utilizadas

```cpp
#include <opencv2/opencv.hpp>  // Procesamiento de imágenes
#include <iostream>            // Entrada/salida estándar
#include <string>              // Manejo de strings
#include <chrono>              // Medición de tiempo
```

### Funcionamiento Principal

1. **Carga de Imagen**: Lee la imagen especificada usando `imread()`
2. **Conversión a Escala de Grises**: Usa `cvtColor()` con `COLOR_BGR2GRAY`
3. **Preparación para Blending**: Convierte la imagen gris a 3 canales con `COLOR_GRAY2BGR`
4. **Generación de Frames**: Para cada frame, calcula el peso `P` y realiza el blending
5. **Guardado**: Cada frame se guarda como archivo PNG

### Medición de Rendimiento

El programa mide el tiempo total de procesamiento usando `std::chrono::high_resolution_clock`, incluyendo:
- Conversión a escala de grises
- Generación de 96 frames con cross-fading
- Guardado de archivos

## Formato de Salida

Los frames generados siguen el patrón:
- `frame_0.png`: Imagen completamente color (P = 1.0)
- `frame_47.png`: Transición intermedia (P ≈ 0.5)
- `frame_95.png`: Imagen completamente en escala de grises (P = 0.0)

## Notas Técnicas

- **Formato de Imagen**: Soporta JPG, PNG, BMP y otros formatos que OpenCV maneje
- **Codificación**: Las imágenes se guardan en formato PNG sin compresión para máxima calidad
- **Precisión**: Usa `saturate_cast<uchar>()` para evitar overflow en operaciones aritméticas
- **Rendimiento**: Optimizado para claridad del código, no para velocidad máxima

## Creación de Video

Una vez generados los frames, puede crear un video usando el script `create_video.sh`:

### Requisitos para Crear Video

```bash
# Instalar FFmpeg
sudo apt install ffmpeg
```

### Uso del Script

```bash
# Hacer el script ejecutable
chmod +x create_video.sh

# Crear video a partir de los frames generados
./create_video.sh
```

Este script:
- Busca automáticamente los archivos `frame_*.png` generados
- Crea un video MP4 de 4 segundos a 24 fps
- El video resultante se llama `crossfade_sequential.mp4`

### Comando Manual (Alternativo)

Si prefiere crear el video manualmente:

```bash
ffmpeg -framerate 24 -i frame_%d.png -c:v libx264 -pix_fmt yuv420p crossfade_sequential.mp4
```

## Verificación de Resultados

Para verificar que el programa funciona correctamente:

1. Ejecute con una imagen de prueba
2. Verifique que se generen 96 archivos `frame_*.png`
3. Abra `frame_0.png` (debería ser la imagen original)
4. Abra `frame_95.png` (debería ser la versión en escala de grises)
5. Los frames intermedios deberían mostrar una transición suave
6. **Opcional**: Cree el video con `create_video.sh` y reproduzca `crossfade_sequential.mp4`

## Próximas Entregas

Este repositorio se expandirá con:
- **Segunda Entrega**: Implementación paralela con MPI (memoria distribuida)
- **Tercera Entrega**: Implementación paralela con OpenMP (memoria compartida)
- **Análisis Comparativo**: Rendimiento y métricas de speedup/eficiencia

---

**Materia**: Computación Avanzada (CA) - UADER
**Trabajo**: Práctico Final - Cross-Fading de Imágenes
**Entrega**: Primera (Algoritmo Secuencial)

