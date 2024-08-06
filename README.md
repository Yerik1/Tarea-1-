
# Tarea #1 de Algoritmos y Estructuras de Datos 2

Este proyecto consta de buenos usos de la administracion de memoria, mediante el uso de archivos, algoritmos de ordenamiento y estructuras de datos lineales





## Pasos a seguir para utilizar primer programa

- Asegurarse de que el repositorio se haya copiado en la carpeta de preferencia
- Dentro de su editor de codigo de preferencia que sea compatible con c++, crear una build de run para el archivo generator.cpp
- Una vez corrido el programa, en terminal debe de aparecer el mensaje: Uso: <RUTA DEL ARCHIVO.cpp> -size <SIZE> -output <OUTPUT FILE PATH>
- En caso de que haya funcionado el caso anterior en consola se deben de poner dos comandos en la terminal, el primero es: g++ -o Tarea-1-  generator.cpp
- El comando anterior lo que hace es compilar el archivo cpp
- El segundo comando es: ./generator -size <SMALL, MEDIUM o LARGE> -output <OUTPUT FILE PATH>
- En este comando despues de size existen tres tamaños a elegir, SMALL, MEDIUM y LARGE, que generan un archivo de 512MB, 1GB y 2GB respectivamente y en el output se debe colocar la RUTA DE LA CARPETA donde se va a generar el archivo (Para elegir la carpeta del proyecto se puede escribir un punto ".").

## Pasos a seguir para utilizar segundo programa
- Una vez generado el archivo "entrada" al realizar los pasos anteriores se puede utilizar el segundo programa
- Dentro de su editor de codigo de preferencia que sea compatible con c++, crear una build de run para el archivo sorter.cpp
- Una vez corrido el programa, en terminal debe de aparecer el mensaje: Uso: <RUTA DEL ARCHIVO.cpp> -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <ALGORITMO>
- En caso de que haya funcionado el caso anterior en consola se deben de poner dos comandos en la terminal, el primero es: g++ -o Tarea-1-  sorter.cpp
- El comando anterior lo que hace es compilar el archivo cpp
- El segundo comando es: ./sorter -input <INPUT FILE PATH> -output <OUTPUT FILE PATH> -alg <QS, IS o BS>
- En este caso existen tres atributos los cuales hay que colocar, el input es la ruta del archivo entrada que se genero con el primer programa, ejemplo ./entrada
- El segundo atributo corresponde a la carpeta de salida donde se van a guardar los archivos de salida (Para elegir la carpeta del proyecto se puede escribir un punto ".").
- El tercero corresponde al algoritmo de ordenamiento que se requiere utilizar donde QS indica QuickSort, IS indica InsertionSort y BS indica BubbleSort. El agortimo recomendado de usar es QS, esto debido a que los otros dos duran mucho tiempo en lograr realizar el ordenamiento incluso con el archivo de 512MB




## Authors

Yerik Chaves Serrano @Yerik1
