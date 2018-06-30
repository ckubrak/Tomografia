#include "sparse.h"
#include <stdlib.h>
#include <chrono>
#include "rayos.h"
#include "misc.h"
#include "ruido.h"

//
//armar vector con los tiempos de todos los rayos
// una posicion por rayo
// para cada rayo llamar a tiemporayo y cargar una posicion del vector tiempos
//void armarVectorTiempos(int n, DOK &Imagen, std::vector <pair<pair<double, double>, double>> &rayos, Vector &tiempos)
void armarVectorTiempos(int n, DOK &Imagen, info &rayos, Vector &tiempos)
{

    for (int i=0; i<rayos.size(); ++i)
    {
        // double tiemporayo(int n, int m , matriz velocidad, double x, double y, double angulo)
        tiempos[i] = tiemporayo(n, n, Imagen.data(), rayos[i].first.first, rayos[i].first.second, rayos[i].second);
    }
    return;

}
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
    //archivoSalida += ".ppm";

    // nivel de ruido. Valor entre cero y uno
    float ruido = atof(argv[2]);

    // dimension de la imagen (ejemplo si la imagen es de 512 x 512 pixeles, n=512)
    int n = atoi(argv[3]);

    int dimCelda = atoi(argv[4]);

    int cantEmisores = atoi(argv[5]);
    //cantidad de rayos que sale de cada emisor
    int cantRayos = atoi(argv[6]);

    // epsilon para EG
    double eps;
    eps = 0.00001;
    
    // opcionalmente el epsilon se puede recibir como parametro    
    if (argc > 7)
    {
        eps = atof(argv[7]);
    }
    
    //levantar la imagen
    DOK Imagen(n);
    Imagen.cargarCsv(archivoEntrada, n);
    //mostrarMatriz(Imagen);
    // generar los rayos, guardar pto origen y angulo para cada rayo
    //std::vector <pair<pair<double, double>, double>> rayos;
    info rayos;
    //rayos = generarRayosVertices(n,cantRayos);

    //std::vector <pair<pair<double, double>, double>> rayosCuadricula;
    //info rayosCuadricula;
    //rayos = generarRayosCuadricula(n, cantRayos);
    cantEmisores = 4*(n/dimCelda);
    cantRayos = n;
    //Cuidado que cantEmisores son los emisores de cada eje, o sea, vamos a tener el doble en realidad
    rayos = generarRayosEjeY(n, cantEmisores, cantRayos);
    

    std::cout << "rayos" << std::endl;



//    calcular la matriz de distancias
    
    DOK mDistancias( (cantRayos*cantEmisores), int ( (n/dimCelda)*(n/dimCelda)) ) ;
    mDistancias.distancia(n, n, dimCelda, dimCelda, rayos);

    std::cout << "distancia" << std::endl;
// calcular el vector de tiempos exactos de cada rayo

    std::vector<double> vtiempos(cantRayos*cantEmisores);
    armarVectorTiempos(n, Imagen, rayos, vtiempos);

    std::cout << "Vector tiempos" << std::endl;
// Calcular las intensidades promedio de las celdas de la matriz discreta
    Vector intensidadPromedioXCelda;
    intensidadPromedioXCelda = promediarIntensidadesXCelda(Imagen, dimCelda);
    std::cout << "Intensidad Celda" << std::endl;
    // se agrega ruido al vector de tiempos de rayos
    agregarRuidoRayo(vtiempos, ruido);

// cuadrados minimos: se llama con el vector de tiempos con ruido y la matriz de distancias
    
    Vector resultadoCM;
    resultadoCM = mDistancias.CuadradosMinimos(vtiempos);

// convertir los valores calculados en velocidades
    
    // Vector intensidadCM;
    // intensidadCM = calcularIntensidad (resultadoCM);

// Resultado de CM
// promedios de intensidad
    std::cout << "Intensidad Promedio X Celda: \n";
    printVector(intensidadPromedioXCelda);

    std::cout << "Resultado Calculado X Cuadrados Minimos: \n";
    printVector(resultadoCM);

//    error cuadratico reportar a la salida estandar
    std::cout << "ECM: " << ErrorCuadraticoMedio(intensidadPromedioXCelda, resultadoCM) <<  "\n";

//    convertir y grabar la imagen
    grabarPPM8Bits(archivoSalida, n/dimCelda, n/dimCelda, to8bits(resultadoCM));
    
    return 0;
}
