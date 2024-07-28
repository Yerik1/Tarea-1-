#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <ctime>

// Tamaño del bloque en bytes
const size_t BLOCK_SIZE = 1024 * 1024 * 10; // 10 MB

// Función para ordenar un bloque y guardarlo en un archivo temporal en formato binario
void sortBlock(const std::string& inputFilePath, size_t start, size_t end, const std::string& tempFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream tempFile(tempFilePath, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Error al abrir el archivo de entrada: " << inputFilePath << std::endl;
        return;
    }

    if (!tempFile) {
        std::cerr << "Error al abrir el archivo temporal: " << tempFilePath << std::endl;
        return;
    }

    inputFile.seekg(start);

    std::vector<int> buffer((end - start) / sizeof(int));
    inputFile.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(int));

    if (inputFile.fail() && !inputFile.eof()) {
        std::cerr << "Error al leer el bloque desde el archivo. Start: " << start << ", End: " << end << std::endl;
        return;
    }

    std::cout << "Bloque leído de " << start << " a " << end << ". Cantidad de enteros leídos: " << buffer.size() << std::endl;

    std::sort(buffer.begin(), buffer.end());

    tempFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(int));

    if (!tempFile) {
        std::cerr << "Error al escribir el archivo temporal." << std::endl;
    }

    inputFile.close();
    tempFile.close();
}

// Función para combinar archivos ordenados usando k-way merge en formato binario
void mergeFiles(const std::vector<std::string>& sortedFiles, const std::string& outputFilePath) {
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    std::vector<std::ifstream> inputFiles;

    for (const auto& file : sortedFiles) {
        inputFiles.emplace_back(file, std::ios::binary);
    }

    auto compare = [](const std::pair<int, size_t>& lhs, const std::pair<int, size_t>& rhs) {
        return lhs.first > rhs.first;
    };
    std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, decltype(compare)> minHeap(compare);

    std::vector<int> currentValues(inputFiles.size());

    for (size_t i = 0; i < inputFiles.size(); ++i) {
        if (inputFiles[i].read(reinterpret_cast<char*>(&currentValues[i]), sizeof(int))) {
            minHeap.emplace(currentValues[i], i);
        }
    }

    while (!minHeap.empty()) {
        auto [value, fileIndex] = minHeap.top();
        minHeap.pop();
        outputFile.write(reinterpret_cast<const char*>(&value), sizeof(int));

        if (inputFiles[fileIndex].read(reinterpret_cast<char*>(&currentValues[fileIndex]), sizeof(int))) {
            minHeap.emplace(currentValues[fileIndex], fileIndex);
        }
    }

    if (!outputFile) {
        std::cerr << "Error al escribir el archivo de salida." << std::endl;
    }

    outputFile.close();
    for (auto& file : inputFiles) {
        file.close();
    }
}

// Función para convertir el archivo binario ordenado a texto con enteros separados por comas
void convertBinaryToText(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile) {
        std::cerr << "Error al abrir el archivo de entrada: " << inputFilePath << std::endl;
        return;
    }

    if (!outputFile) {
        std::cerr << "Error al abrir el archivo de salida: " << outputFilePath << std::endl;
        return;
    }

    int number;
    bool first = true;

    while (inputFile.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        if (!first) {
            outputFile << ", ";
        }
        outputFile << number;
        first = false;
    }

    if (!inputFile.eof() && inputFile.fail()) {
        std::cerr << "Error al leer el archivo binario." << std::endl;
    }

    inputFile.close();
    outputFile.close();
}

// Función para convertir el archivo de entrada binario a texto con enteros separados por comas
void convertInputBinaryToText(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath);

    if (!inputFile) {
        std::cerr << "Error al abrir el archivo de entrada: " << inputFilePath << std::endl;
        return;
    }

    if (!outputFile) {
        std::cerr << "Error al abrir el archivo de salida: " << outputFilePath << std::endl;
        return;
    }

    int number;
    bool first = true;

    while (inputFile.read(reinterpret_cast<char*>(&number), sizeof(number))) {
        if (!first) {
            outputFile << ", ";
        }
        outputFile << number;
        first = false;
    }

    if (!inputFile.eof() && inputFile.fail()) {
        std::cerr << "Error al leer el archivo binario." << std::endl;
    }

    inputFile.close();
    outputFile.close();
}

// Función principal de ordenamiento
void sorter(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& algoritmo) {
    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    size_t fileSize = inputFile.tellg();
    inputFile.close();

    size_t blockSize = BLOCK_SIZE;
    size_t numBlocks = (fileSize + blockSize - 1) / blockSize;

    std::vector<std::string> tempFiles(numBlocks);

    for (size_t i = 0; i < numBlocks; ++i) {
        size_t start = i * blockSize;
        size_t end = std::min(start + blockSize, fileSize);
        tempFiles[i] = outputFilePath + "/temp_block_" + std::to_string(i) + ".bin";
        sortBlock(inputFilePath, start, end, tempFiles[i]);
    }

    std::string sortedBinaryPath = outputFilePath + "/sorted_data.bin";
    mergeFiles(tempFiles, sortedBinaryPath);

    for (const auto& file : tempFiles) {
        std::remove(file.c_str());
    }

    // Convertir el archivo binario ordenado a texto con enteros separados por comas
    std::string textOutputPath = outputFilePath + "/salida_entera.txt";
    convertBinaryToText(sortedBinaryPath, textOutputPath);

    // Convertir el archivo de entrada binario a texto con enteros separados por comas (sin ordenar)
    std::string inputTextOutputPath = outputFilePath + "/entrada_entera.txt";
    convertInputBinaryToText(inputFilePath, inputTextOutputPath);
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr << "Uso: " << argv[0] << " -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO>" << std::endl;
        return 1;
    }

    std::string inputFilePath;
    std::string outputFilePath;
    std::string algoritmo;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-input" && i + 1 < argc) {
            inputFilePath = argv[++i];
        } else if (arg == "-output" && i + 1 < argc) {
            outputFilePath = argv[++i];
        } else if (arg == "-alg" && i + 1 < argc) {
            algoritmo = argv[++i];
        }
    }

    if (inputFilePath.empty() || outputFilePath.empty()) {
        std::cerr << "Debe especificar las rutas de los archivos de entrada y salida." << std::endl;
        return 1;
    }

    // Ordenar el archivo de entrada y guardar el resultado
    sorter(inputFilePath, outputFilePath, algoritmo);

    return 0;
}
