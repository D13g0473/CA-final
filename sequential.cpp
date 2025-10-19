#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <image_path>" << endl;
        return -1;
    }

    //  Mat es una clase en OpenCV que representa una matriz de datos, 
    // comúnmente utilizada para almacenar imágenes. 
    // Cada elemento de la matriz puede representar un píxel de la imagen,
    // y la matriz puede tener múltiples canales 
    // (por ejemplo, para imágenes en color).
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
        // mezcla las dos imagenes usando el mismo algoritmo que los paralelos
        // CV_8UC3 indica que la imagen resultante es de 8 bits sin signo con 3 canales (BGR)
        Mat result(colorImg.rows, colorImg.cols, CV_8UC3);
        for (int y = 0; y < colorImg.rows; ++y) {
            for (int x = 0; x < colorImg.cols; ++x) {
                Vec3b colorPixel = colorImg.at<Vec3b>(y, x);
                Vec3b grayPixel = gray3.at<Vec3b>(y, x);
                result.at<Vec3b>(y, x) = Vec3b(
                    saturate_cast<uchar>(colorPixel[0] * p + grayPixel[0] * (1 - p)),
                    saturate_cast<uchar>(colorPixel[1] * p + grayPixel[1] * (1 - p)),
                    saturate_cast<uchar>(colorPixel[2] * p + grayPixel[2] * (1 - p))
                );
            }
        }
        // guarda cada frame como una imagen PNG
        string filename = "frame_" + to_string(i) + ".png";
        // guarda la imagen resultante en un archivo con el nombre especificado
        imwrite(filename, result);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Tiempo de ejecucion secuencial: " << duration.count() << " ms" << endl;

    return 0;
}