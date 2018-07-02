#include "sparse.h"
#include <stdlib.h>
#include <chrono>
#include "rayos.h"
#include "misc.h"
#include "ruido.h"
#include <ctime>


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
    int start_s=clock();

    /*
    leer los nombres de archivos de archivos de E y S
    */

    std::string archivoEntrada = argv[1];
    archivoEntrada += ".csv";

    std::string archivoSalida = argv[1];

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
    
    // opcionalmente la clave (ver script de python para testear) se puede recibir como parametro    
    std::string clave;
    if (argc > 7)
    {
        clave = argv[7];
        archivoSalida += clave;
    }
    else 
    {
        clave = "";
    }

    //levantar la imagen
    DOK Imagen(n);
    Imagen.cargarCsv(archivoEntrada, n);
    info rayos;

    rayos = generarRayosUniformes(n, cantEmisores, cantRayos);
    




//    calcular la matriz de distancias
    
    DOK mDistancias( (cantRayos*cantEmisores), int ( (n/dimCelda)*(n/dimCelda)) ) ;
    mDistancias.distancia(n, n, dimCelda, dimCelda, rayos);

// calcular el vector de tiempos exactos de cada rayo

    std::vector<double> vtiempos(cantRayos*cantEmisores);
    armarVectorTiempos(n, Imagen, rayos, vtiempos);

// Calcular las intensidades promedio de las celdas de la matriz discreta
    Vector intensidadPromedioXCelda;
    intensidadPromedioXCelda = promediarIntensidadesXCelda(Imagen, dimCelda);
    // se agrega ruido al vector de tiempos de rayos
    agregarRuidoRayo(vtiempos, ruido);

// cuadrados minimos: se llama con el vector de tiempos con ruido y la matriz de distancias
    
    Vector resultadoCM;
    resultadoCM = mDistancias.CuadradosMinimos(vtiempos);


    std::cout << " " << ErrorCuadraticoMedio(intensidadPromedioXCelda, resultadoCM);

//    convertir y grabar la imagen
    grabarPPM8Bits(archivoSalida, n/dimCelda, n/dimCelda, to8bits(resultadoCM));

    int stop_s=clock();
    cout << " " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;


    return 0;

}
