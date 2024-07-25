//
// Created by yerik on 7/25/24.
//

#include <iostream>
#include <string>

// Función para mostrar los argumentos de la línea de comandos
void printArguments(int argc, char *argv[]) {
    std::string size;
    std::string outputFilePath;

    // Procesar los argumentos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-size" && i + 1 < argc) {
            size = argv[++i]; // Leer el siguiente argumento
        } else if (arg == "-output" && i + 1 < argc) {
            outputFilePath = argv[++i]; // Leer el siguiente argumento
        }
    }

    // Imprimir los resultados
    std::cout << "Tamaño: " << size << std::endl;
    std::cout << "Ruta del archivo de salida: " << outputFilePath << std::endl;
}

int main(int argc, char *argv[]) {
    printArguments(argc, argv);
    return 0;
}
