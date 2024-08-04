//
// Created by yerik on 7/25/24.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

//peso de los archivos dependiendo del tamaño
enum Size { SMALL = 512LL * 1000 * 1000, MEDIUM = 1000LL * 1000 * 1000, LARGE = 2LL * 1000 * 1000 * 1000 };

//Metodo que genera los archivos que recibe como parametros el lugar a crear el archivo y el tamaño de este
void generateFile(const std::string& outputFilePath, Size size) {

    // Construye la ruta completa del archivo
    std::string fullPath = outputFilePath + "/prueba";

    std::ofstream file(fullPath, std::ios::binary); // Abrir archivo en modo binario

    //En caso de que no exita el directorio
    if (!file) {
        std::cerr << "No se pudo abrir el archivo para escribir: " << fullPath << std::endl;
        return;
    }

    // Inicializar la semilla para números aleatorios
    std::srand(std::time(nullptr));

    // Número de enteros a escribir
    size_t numInts = size / sizeof(int);

    //Ciclo para generear todos los numeros
    for (size_t i = 0; i < numInts; ++i) {
        // Generar un número entero aleatorio
        int randomInt = std::rand();
        // Escribir el entero en el archivo
        file.write(reinterpret_cast<const char*>(&randomInt), sizeof(randomInt));
    }

    file.close();
    std::cout << "Archivo generado con éxito: " << fullPath << std::endl;
}



int main(int argc, char *argv[]) {

        //Cantidad de argumentos diferente a 5
        if (argc != 5) {
            std::cerr << "Uso: " << argv[0] << " -size <SIZE> -output <OUTPUT FILE PATH>" << std::endl;
            return 1;
        }

        //Variable donde se va a guardar el tamaño
        Size size;
        //Variable para guardar el argumento del la ruta
        std::string outputFilePath;

        //ciclo para ir guardadando los argumentos
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            //Encuentra el argumento size
            if (arg == "-size") {
                //Agarra el siguiente y lo categoriza
                if (i + 1 < argc) {
                    std::string sizeStr = argv[++i];
                    if (sizeStr == "SMALL") {
                        size = SMALL;
                    } else if (sizeStr == "MEDIUM") {
                        size = MEDIUM;
                    } else if (sizeStr == "LARGE") {
                        size = LARGE;
                    } else {
                        std::cerr << "Tamaño inválido. Use SMALL, MEDIUM o LARGE." << std::endl;
                        return 1;
                    }
                }

                //Encuentra el argumento output y lo guarda si no esta vacio
                } else if (arg == "-output") {
                if (i + 1 < argc) {
                    outputFilePath = argv[++i];
                }
            }
        }

        if (outputFilePath.empty()) {
            std::cerr << "Debe especificar una ruta de archivo de salida." << std::endl;
            return 1;
        }
    //Llama a funcion para generar el archivo
        generateFile(outputFilePath, size);
        return 0;
}
