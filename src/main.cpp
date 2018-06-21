#include "sparse.h"
#include <stdlib.h>
#include <chrono>
#include "rayos.hpp"
#include "misc.h"



int main (int argc, char** argv)
{

    /*
    leer los nombres de archivos de archivos de E y S
    */

   // archivo csv conteniendo la imagen de entrada
    //char* archivoEntrada = argv[1];
    std::string archivoEntrada = argv[1];
    //std::string in = argv[1];
    archivoEntrada += ".csv";

    // archivo ppm conteniendo la imagen reconstruida, se llama igual que la entrada con extension .ppm
    std::string archivoSalida = argv[1];
    archivoSalida += ".ppm";

    // nivel de ruido. Valor entre cero y uno
    double p = atof(argv[2]);

    // dimension de la imagen (ejemplo si la imagen es de 512 x 512 pixeles, n=512)
    int n = atof(argv[3]);

    int dimCelda = atoi(argv[4]);

    int cantEmisores = atoi(argv[5]);
    //cantidad de rayos que sale de cada emisor
    int cantRayos = atoi(argv[6]);

    // epsilon para EG
    double eps;
    eps = 0.00001;
    
    // opcionalmente el epsilon se puede recibir como parametro    
    if (sizeof(argv) >= 7)
    {
        eps = atof(argv[7]);
    }
    
    //levantar la imagen
    DOK Imagen(n);
    Imagen.cargarCsv(archivoEntrada, n);


    // generar los rayos, guardar pto origen y angulo para cada rayo. Luis: 
    //Vector<pair<int, int>, double> generarRayosVertices(int tam,int cantRayos)
    std::vector <pair<pair<int, int>, double>> rayos;
    //rayos = generarRayosVertices(n,cantRayos);

//    calcule la matriz de distancias alejo
    //void distancia(int n, int m, int a, int b, info rayos, DOK& salida)

    DOK mDistancias( (cantRayos*cantEmisores), int ( (n/dimCelda)*(n/dimCelda)) ) ;
    distancia(n, n, dimCelda, dimCelda, rayos, mDistancias);

// calcular el vector de tiempos alejo tiempoRayo, para cada rayo calcula el tiempo y hay que meterla en un vector. 
// pendiente hacer una funcion que calcule para todos los rayos el vector
    Vector tiempos((n/dimCelda)*(n/dimCelda));
//    armarVectorTiempos();

//    meter el ruido en el vector esta en k-fold hay que moverlo luis agregarRuidorayo vector de rayo y double entre 0 y 1


//    cuadrados minimos

//    error cuadratico reportar a la salida estandar
//    convertir y grabar la imagen



    return 0;
}
