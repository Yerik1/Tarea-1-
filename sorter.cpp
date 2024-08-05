#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include <stdexcept>

const int ARRAY_SIZE = 8000;
const int MAX_FRAMES = 4;

class PagedArray {
public:
    class Frames {
    private:
        int numeroPagina;
        int numeros[ARRAY_SIZE];

    public:
        Frames(int numeroPagina) : numeroPagina(numeroPagina) {
            std::fill(std::begin(numeros), std::end(numeros), 0);
        }

        int getNumeroPagina() const {
            return numeroPagina;
        }

        void setNumeroPagina(int numeroPagina) {
            this->numeroPagina = numeroPagina;
        }

        const int* getNumeros() const {
            return numeros;
        }

        int* getNumeros() {
            return numeros;
        }

        void setNumeros(const int newNumeros[ARRAY_SIZE]) {
            std::copy(newNumeros, newNumeros + ARRAY_SIZE, numeros);
        }

        int& operator[](int index) {
            return numeros[index];
        }
    };

public:
    Frames* frames[MAX_FRAMES];
    std::string inputFilePath;
    std::string outputFilePath;

    void guardarFrameEnArchivo(Frames* frame, int frameIndex) {
        std::fstream outputFile(outputFilePath + "/salida", std::ios::binary | std::ios::in | std::ios::out);

        if (!outputFile) {
            throw std::runtime_error("Error al abrir el archivo para guardar el frame");
        }

        // Calcula la posición donde se debe escribir el frame
        std::streampos position = frameIndex * ARRAY_SIZE * sizeof(int);
        outputFile.seekp(position);

        if (!outputFile) {
            throw std::runtime_error("Error al mover el puntero de escritura en el archivo");
        }

        // Reemplaza el contenido existente con los datos del frame
        outputFile.write(reinterpret_cast<char*>(frame->getNumeros()), ARRAY_SIZE * sizeof(int));

        if (!outputFile) {
            throw std::runtime_error("Error al escribir en el archivo");
        }

        //std::cout << "Frame " << frame->getNumeroPagina() << " guardado en posición " << position << std::endl;

        outputFile.close();
    }


    void guardarTodosLosFrames() {
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (frames[i]) {
                guardarFrameEnArchivo(frames[i], frames[i]->getNumeroPagina());
                delete frames[i];
                frames[i] = nullptr;
            }
        }
    }

public:
    PagedArray(const std::string& inputPath, const std::string& outputPath)
        : inputFilePath(inputPath), outputFilePath(outputPath) {
        std::fill(std::begin(frames), std::end(frames), nullptr);
        inicializarArchivoDeSalida();
    }

    ~PagedArray() {
        guardarTodosLosFrames(); // Asegúrate de guardar cualquier frame restante
        for (int i = 0; i < MAX_FRAMES; ++i) {
            delete frames[i];
        }
    }

    void inicializarArchivoDeSalida() {
        std::ofstream outputFile(outputFilePath + "/salida", std::ios::binary | std::ios::out | std::ios::trunc);
        if (!outputFile) {
            throw std::runtime_error("Error al abrir el archivo para inicializar");
        }
        int zero = 0;
        for (int i = 0; i < 128000000; ++i) {
            outputFile.write(reinterpret_cast<const char*>(&zero), sizeof(zero));
        }

        outputFile.close();
    }

    int& operator[](int index) {
        int frameIndex = index / ARRAY_SIZE;
        int offset = index % ARRAY_SIZE;

        // Verifica si el frame está cargado
        bool frameLoaded = false;
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (frames[i] && frames[i]->getNumeroPagina() == frameIndex) {
                frameLoaded = true;
                return frames[i]->getNumeros()[offset];
            }
        }
        cargarFrameDesdeArchivo(frameIndex);

        // Regresa el valor solicitado
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (frames[i] && frames[i]->getNumeroPagina() == frameIndex) {
                return frames[i]->getNumeros()[offset];
            }
        }

        throw std::runtime_error("Frame no cargado correctamente");
    }

    void cargarFrameDesdeArchivo(int frameIndex) {
        // Verifica si hay un frame vacío
        int freeFrameIndex = -1;
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (!frames[i]) {
                freeFrameIndex = i;
                break;
            }
        }

        if (freeFrameIndex == -1) {
            // No hay frames libres, reemplazar uno aleatorio
            freeFrameIndex = rand() % MAX_FRAMES;
            // Guardar el frame existente en el archivo de salida
            guardarFrameEnArchivo(frames[freeFrameIndex], frames[freeFrameIndex]->getNumeroPagina());
            delete frames[freeFrameIndex];
        }

        // Verificar si el frame ya está en el archivo de salida
        std::ifstream outputFile(outputFilePath + "/salida", std::ios::binary);
        if (!outputFile) {
            throw std::runtime_error("Error al abrir el archivo de salida para leer el frame");
        }

        int* data = new int[ARRAY_SIZE];
        std::streampos position = frameIndex * ARRAY_SIZE * sizeof(int);
        outputFile.seekg(position);
        outputFile.read(reinterpret_cast<char*>(data), ARRAY_SIZE * sizeof(int));

        bool loadFromOutput = false;
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            if (data[i] != 0) {
                loadFromOutput = true;
                break;
            }
        }

        outputFile.close();

        if (!loadFromOutput) {
            // Leer el nuevo frame desde el archivo de entrada
            std::ifstream inputFile(inputFilePath, std::ios::binary);
            if (!inputFile) {
                throw std::runtime_error("Error al abrir el archivo binario de entrada");
            }

            inputFile.seekg(position);
            if (!inputFile.read(reinterpret_cast<char*>(data), ARRAY_SIZE * sizeof(int))) {
                throw std::runtime_error("Error al leer el archivo binario de entrada");
            }

            inputFile.close();
        }

        frames[freeFrameIndex] = new Frames(frameIndex);
        frames[freeFrameIndex]->setNumeros(data);
        delete[] data;

        //std::cout << "Frame " << frameIndex << " cargado en posición " << position << std::endl;
    }

    int verificarFrames() const {
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (!frames[i]) return i + 1;
        }
        return 0;
    }
};

// QuickSort
// Función de partición para Quick Sort
int partition(PagedArray& arr, int start, int end) {
    int pivot = arr[start]; // Obtener el pivote

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot) {
            count++;
        }
    }

    // Dar al elemento pivote su posición correcta
    int pivotIndex = start + count;
    std::swap(arr[pivotIndex], arr[start]);

    // Ordenar las partes izquierda y derecha del pivote
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            std::swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}

// Función de Quick Sort adaptada para PagedArray
void quickSort(PagedArray& arr, int start, int end) {
    // Caso base
    if (start >= end)
        return;

    // Particionar el array
    int p = partition(arr, start, end);

    // Ordenar la parte izquierda
    quickSort(arr, start, p - 1);

    // Ordenar la parte derecha
    quickSort(arr, p + 1, end);
}

// InsertionSort
void insertionSort(PagedArray& arr, int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// BubbleSort
void bubbleSort(PagedArray& arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void convertirBinarioAEnteros(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "No se pudo abrir el archivo binario de entrada.\n";
        return;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        // Intentar crear el archivo si no existe
        outputFile.open(outputFileName, std::ios::out | std::ios::trunc);
        if (!outputFile) {
            std::cerr << "No se pudo crear el archivo de texto de salida.\n";
            return;
        }
    }

    int number;
    bool first = true;

    // Lee el archivo binario y escribe los enteros separados por comas en el archivo de salida
    while (inputFile.read(reinterpret_cast<char*>(&number), sizeof(int))) {
        if (!first) {
            outputFile << ",";
        }
        outputFile << number;
        first = false;
    }

    inputFile.close();
    outputFile.close();
}

// Función principal de ordenamiento
void sorter(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& algoritmo) {
    PagedArray pagedArray(inputFilePath, outputFilePath);
    std::ifstream inputFile(inputFilePath, std::ios::binary | std::ios::ate);
    std::streamsize fileSize = inputFile.tellg();
    inputFile.close();

    int totalIntegers = 128000000;
    int current = 0;

    if (algoritmo == "QS") {
        quickSort(pagedArray, 0, totalIntegers - 1);
    } else if (algoritmo == "IS") {
        insertionSort(pagedArray, totalIntegers);
    } else if (algoritmo == "BS") {
        bubbleSort(pagedArray, totalIntegers);

    } else {
        std::cerr << "Algoritmo no reconocido: " << algoritmo << std::endl;
        return;
    }

}

// Función para leer los argumentos
int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr << "Uso: " << argv[0] << " -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO>" << std::endl;
        sorter("./prueba",".","QS");
        convertirBinarioAEnteros("./salida","./salidaEntera.txt");
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
    convertirBinarioAEnteros(outputFilePath+"/salida",outputFilePath+"/salidaEntera.txt");
    return 0;
}