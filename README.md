# Procesamiento de Imágenes Cross-Fading - Algoritmos Paralelos

## Primera Entrega: Implementación Secuencial
## Segunda Entrega: Implementación Paralela con MPI

Este repositorio contiene las entregas del Trabajo Práctico Final de la materia **Computación Avanzada (CA)**, correspondientes a las implementaciones secuencial y paralela con MPI del algoritmo de cross-fading de imágenes.

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
├── mpi_parallel.cpp            # Implementación paralela con MPI
├── README.md                   # Este archivo
├── create_video.sh             # Creación de video desde frames
├── test_image.png              # Imagen de prueba
├── test_image_2.png            # Imagen de prueba 2
├── test_image_3.png            # Imagen de prueba 3 (PNG)
```

## Requisitos del Sistema

### Dependencias

- **Compilador C++**: GCC con soporte para C++11
- **OpenCV 4.x**: Para procesamiento de imágenes
- **MPI**: OpenMPI o MPICH para implementación paralela
- **Sistema Operativo**: Ubuntu/Debian o similar

### Instalación de Dependencias

```bash
# Actualizar el sistema
sudo apt update

# Instalar OpenCV
sudo apt install libopencv-dev

# Instalar MPI (OpenMPI)
sudo apt install openmpi-bin openmpi-doc libopenmpi-dev

# Verificar instalación
pkg-config --modversion opencv4
mpirun --version
```

## Compilación

### Programa Secuencial

Para compilar el programa secuencial:

```bash
g++ -std=c++11 sequential.cpp -o sequential `pkg-config --cflags --libs opencv4`
```

Este comando:
- Usa el estándar C++11
- Compila `sequential.cpp`
- Genera el ejecutable `sequential`
- Enlaza automáticamente con las librerías de OpenCV

### Programa Paralelo con MPI

Para compilar el programa paralelo con MPI:

```bash
mpic++ -std=c++11 mpi_parallel.cpp -o mpi_parallel `pkg-config --cflags --libs opencv4`
```

Este comando:
- Usa el compilador MPI (mpic++)
- Compila `mpi_parallel.cpp`
- Genera el ejecutable `mpi_parallel`
- Enlaza con OpenCV y las librerías MPI

## Ejecución

### Programa Secuencial

#### Sintaxis Básica

```bash
./sequential <ruta_a_la_imagen>
```

#### Ejemplos con las Imágenes del Repositorio

##### 1. Imagen PNG (test_image.png)

```bash
./sequential test_image.png
```

##### 2. Imagen PNG (test_image_2.png)

```bash
./sequential test_image_2.png
```

##### 3. Imagen JPG (test_image_3.jpg)

```bash
./sequential test_image_3.jpg
```

#### Salida del Programa Secuencial

El programa generará:
- 96 archivos de imagen: `frame_0.png`, `frame_1.png`, ..., `frame_95.png`
- Un mensaje con el tiempo de ejecución en milisegundos

```
Tiempo de ejecucion secuencial: 5268 ms
```

### Programa Paralelo con MPI

#### Sintaxis Básica

```bash
mpirun -np <numero_de_procesos> ./mpi_parallel <ruta_a_la_imagen>
```

#### Ejemplos con las Imágenes del Repositorio

##### 1. Con 2 procesos

```bash
mpirun -np 2 ./mpi_parallel test_image.png
```

##### 2. Con 4 procesos

```bash
mpirun -np 4 ./mpi_parallel test_image.png
```

##### 3. Con 8 procesos

```bash
mpirun -np 8 ./mpi_parallel test_image.png
```

#### Salida del Programa Paralelo

El programa generará:
- 96 archivos de imagen: `frame_mpi_0.png`, `frame_mpi_1.png`, ..., `frame_mpi_95.png`
- Un mensaje con el tiempo de ejecución en milisegundos (solo del proceso raíz)

```
Tiempo de ejecucion de MPI: 2346 ms
```

#### Notas sobre la Ejecución Paralela

- **Distribución de Trabajo**: Las filas de la imagen se dividen entre los procesos MPI
- **Comunicación**: Se usa `MPI_Bcast` para distribuir la imagen inicial y `MPI_Gatherv` para recolectar los frames
- **Escalabilidad**: El rendimiento mejora con más procesos, pero puede haber overhead de comunicación
- **Archivos de Salida**: Solo el proceso raíz (rank 0) guarda los frames

## Descripción del Código

### Implementación Secuencial

#### Librerías Utilizadas

```cpp
#include <opencv2/opencv.hpp>  // Procesamiento de imágenes
#include <iostream>            // Entrada/salida estándar
#include <string>              // Manejo de strings
#include <chrono>              // Medición de tiempo
```

#### Funcionamiento Principal

1. **Carga de Imagen**: Lee la imagen especificada usando `imread()`
2. **Conversión a Escala de Grises**: Usa `cvtColor()` con `COLOR_BGR2GRAY`
3. **Preparación para Blending**: Convierte la imagen gris a 3 canales con `COLOR_GRAY2BGR`
4. **Generación de Frames**: Para cada frame, calcula el peso `P` y realiza el blending
5. **Guardado**: Cada frame se guarda como archivo PNG

### Implementación Paralela con MPI

#### Librerías Utilizadas

```cpp
#include <opencv2/opencv.hpp>  // Procesamiento de imágenes
#include <mpi.h>               // MPI para paralelización
#include <iostream>            // Entrada/salida estándar
#include <string>              // Manejo de strings
#include <vector>              // Vectores para arrays MPI
#include <chrono>              // Medición de tiempo
```

#### Funciones Paralelas

- **`convertToGrayscale`**: Convierte porciones de la imagen a escala de grises en paralelo
- **`crossFade`**: Realiza el blending de cross-fading en porciones de filas

#### Funcionamiento Principal

1. **Inicialización MPI**: `MPI_Init()` y obtención de rank/size
2. **Carga y Distribución**: Proceso raíz carga la imagen y la distribuye con `MPI_Bcast`
3. **Asignación de Memoria**: Todos los procesos asignan espacio para matrices locales
4. **Conversión Paralela a Escala de Grises**: Cada proceso convierte su porción de filas
5. **Recolección de Escala de Grises**: `MPI_Allgatherv` reúne todas las porciones
6. **Generación Paralela de Frames**:
   - Cada proceso calcula su porción de filas para cada frame
   - `MPI_Gatherv` reúne las porciones en el proceso raíz
   - Solo el proceso raíz guarda los frames
7. **Finalización**: `MPI_Finalize()`

### Medición de Rendimiento

El programa mide el tiempo total de procesamiento usando `std::chrono::high_resolution_clock`, incluyendo:
- Conversión a escala de grises
- Generación de 96 frames con cross-fading
- Guardado de archivos

## Formato de Salida

### Frames Secuenciales

Los frames generados por el programa secuencial siguen el patrón:
- `frame_0.png`: Imagen completamente color (P = 1.0)
- `frame_47.png`: Transición intermedia (P ≈ 0.5)
- `frame_95.png`: Imagen completamente en escala de grises (P = 0.0)

### Frames Paralelos con MPI

Los frames generados por el programa paralelo siguen el patrón:
- `frame_mpi_0.png`: Imagen completamente color (P = 1.0)
- `frame_mpi_47.png`: Transición intermedia (P ≈ 0.5)
- `frame_mpi_95.png`: Imagen completamente en escala de grises (P = 0.0)

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

#### Para Frames Secuenciales

```bash
# Hacer el script ejecutable
chmod +x create_video.sh

# Crear video a partir de los frames secuenciales
./create_video.sh
```

Este script:
- Busca automáticamente los archivos `frame_*.png` generados
- Crea un video MP4 de 4 segundos a 24 fps
- El video resultante se llama `crossfade_sequential.mp4`

#### Para Frames Paralelos con MPI

```bash
# Crear video a partir de los frames MPI
./create_video.sh mpi
```

Este comando crea el video `crossfade_mpi.mp4` a partir de los frames `frame_mpi_*.png`.

### Comando Manual (Alternativo)

Si prefiere crear el video manualmente:

#### Video Secuencial

```bash
ffmpeg -framerate 24 -i frame_%d.png -c:v libx264 -pix_fmt yuv420p crossfade_sequential.mp4
```

#### Video MPI

```bash
ffmpeg -framerate 24 -i frame_mpi_%d.png -c:v libx264 -pix_fmt yuv420p crossfade_mpi.mp4
```

## Verificación de Resultados

### Verificación del Programa Secuencial

Para verificar que el programa secuencial funciona correctamente:

1. Ejecute con una imagen de prueba: `./sequential test_image.png`
2. Verifique que se generen 96 archivos `frame_*.png`
3. Abra `frame_0.png` (debería ser la imagen original)
4. Abra `frame_95.png` (debería ser la versión en escala de grises)
5. Los frames intermedios deberían mostrar una transición suave
6. **Opcional**: Cree el video con `./create_video.sh` y reproduzca `crossfade_sequential.mp4`

### Verificación del Programa Paralelo con MPI

Para verificar que el programa paralelo funciona correctamente:

1. Ejecute con una imagen de prueba: `mpirun -np 4 ./mpi_parallel test_image.png`
2. Verifique que se generen 96 archivos `frame_mpi_*.png`
3. Abra `frame_mpi_0.png` (debería ser la imagen original)
4. Abra `frame_mpi_95.png` (debería ser la versión en escala de grises)
5. Los frames intermedios deberían mostrar una transición suave idéntica a la versión secuencial
6. **Opcional**: Cree el video con `./create_video.sh mpi` y reproduzca `crossfade_mpi.mp4`
7. Compare los tiempos de ejecución entre versiones secuencial y paralela

## Análisis de Rendimiento

### Resultados de Benchmarking

Ejecutando el script `benchmark.sh` se obtienen mediciones comparativas:

```bash
# Hacer el script ejecutable y ejecutarlo
chmod +x benchmark.sh
./benchmark.sh
```

#### Resultados Típicos

| Implementación | Procesos | Tiempo (ms) | Speedup | Eficiencia |
|----------------|----------|-------------|---------|------------|
| Secuencial     | 1        | 5268        | 1.00x   | 100%       |
| MPI            | 2        | 3488        | 1.51x   | 75.5%      |
| MPI            | 4        | 2346        | 2.25x   | 56.2%      |

### Factores que Afectan el Rendimiento

- **Overhead de Comunicación**: MPI introduce latencia en la distribución/recolecta de datos
- **Escalabilidad**: Mejor rendimiento con más procesos para imágenes grandes
- **I/O Limitado**: El guardado de frames puede convertirse en bottleneck
- **Distribución de Carga**: Manejo apropiado de filas irregulares con `MPI_Allgatherv`/`MPI_Gatherv`

## Próximas Entregas

Este repositorio se expandirá con:
- **Tercera Entrega**: Implementación paralela con OpenMP (memoria compartida)
- **Análisis Comparativo**: Rendimiento detallado y métricas de speedup/eficiencia entre todas las implementaciones

---

**Materia**: Computación Avanzada (CA) - UADER
**Trabajo**: Práctico Final - Cross-Fading de Imágenes
**Entregas**: Primera (Secuencial) y Segunda (MPI Paralelo)

