#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <image_path>" << endl;
        return -1;
    }

    //  Mat es una clase en OpenCV que representa una matriz de datos, 
    // comúnmente utilizada para almacenar imágenes. Cada elemento de la matriz puede representar un píxel de la imagen,
    // y la matriz puede tener múltiples canales (por ejemplo, para imágenes en color).
    Mat colorImg = imread(argv[1], IMREAD_COLOR);
    if (colorImg.empty()) {
        cout << "No se logro abrir o encontrar la imagen" << endl;
        return -1;
    }

    // convertir a escala de grises
    Mat grayImg;
    cvtColor(colorImg, grayImg, COLOR_BGR2GRAY);

    // Convertir escala de grises a 3 canales para mezclar
    Mat gray3;
    cvtColor(grayImg, gray3, COLOR_GRAY2BGR);

    // cantidad de frames
    int frames = 96; // 4 segundos * 24 fps
    // inicia el cronometro para medir el tiempo de ejecucion
    auto start = high_resolution_clock::now();

    // por cada uno de los frames que se van a generar
    for (int i = 0; i < frames; ++i) {
        // calcula el peso de la imagen a color
        double p = 1.0 - (double)i / (frames - 1);
        // mezcla las dos imagenes
        Mat result;
        // que hace la siguiente linea?
        // lo que hace es mezclar dos imagenes con un peso determinado
        addWeighted(colorImg, p, gray3, 1 - p, 0, result);
        string filename = "frame_" + to_string(i) + ".png";
        imwrite(filename, result);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Tiempo de ejecucion secuencial: " << duration.count() << " ms" << endl;

    return 0;
}