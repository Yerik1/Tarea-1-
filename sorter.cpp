//
// Created by yerik on 7/27/24.
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

void sorter(std::string input, std::string output, std::string alg) {
    std::cout << "Argumento 1: " << input<< std::endl;
    std::cout << "Argumento 2: " << output << std::endl;
    std::cout << "Argumento 3: " << alg << std::endl;
}

int main(int argc, char *argv[]) {

    //Cantidad de argumentos diferente a 5
    if (argc != 7) {
        std::cerr << "Uso: " << argv[0] << "  –input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO>" << std::endl;
        return 1;
    }

    //Variable donde se va a guardar el argumento del archivo a leer
    std::string inputFilePath;
    //Variable para guardar el argumento de la ruta
    std::string outputFilePath;
    //Variable donde se va a guardar el algoritmo a usar
    std::string algoritmo;

    //ciclo para ir guardadando los argumentos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        //Encuentra el argumento size
        if (arg == "-input") {
            //Agarra el siguiente y lo categoriza
            if (i + 1 < argc) {
                if (i + 1 < argc) {
                    inputFilePath = argv[++i];
                }
            }

            //Encuentra el argumento output y lo guarda si no esta vacio
        } else if (arg == "-output") {
            if (i + 1 < argc) {
                outputFilePath = argv[++i];
            }
        } else if (arg == "-alg") {
            if (i + 1 < argc) {
                algoritmo = argv[++i];
            }
        }
    }

    if (outputFilePath.empty()) {
        std::cerr << "Debe especificar una ruta de archivo de salida." << std::endl;
        return 1;
    }
    //Llama a funcion para generar el archivo
    sorter(inputFilePath, outputFilePath, algoritmo);
    return 0;
}