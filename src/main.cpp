#include "sparse.h"
#include <stdlib.h>
#include <chrono>
#include "rayos.hpp"
#include "misc.h"
#include "ruido.h"

//
//armar vector con los tiempos de todos los rayos
// una posicion por rayo
// para cada rayo llamar a tiemporayo y cargar una posicion del vector tiempos
void armarVectorTiempos(int n, DOK &Imagen, std::vector <pair<pair<double, double>, double>> &rayos, Vector &tiempos)
{

    for (int i=0; i<rayos.size(); ++i)
    {
         cout<<"hola"<<i<<endl;
        // double tiemporayo(int n, int m , matriz velocidad, double x, double y, double angulo)
        tiempos[i] = tiemporayo(n, n, Imagen.data(), rayos[i].first.first, rayos[i].first.second, rayos[i].second);
    }
    return;

}
int main()
{

    /*
    leer los nombres de archivos de archivos de E y S
    */

   // archivo csv conteniendo la imagen de entrada
    //char* archivoEntrada = argv[1];
    std::string archivoEntrada = "tomo";
    //std::string in = argv[1];
    archivoEntrada += ".csv";

    // archivo ppm conteniendo la imagen reconstruida, se llama igual que la entrada con extension .ppm
    std::string archivoSalida = "tomo";
    archivoSalida += ".ppm";

    // nivel de ruido. Valor entre cero y uno
    float ruido = 0.2;

    // dimension de la imagen (ejemplo si la imagen es de 512 x 512 pixeles, n=512)
    int n = 512;

    int dimCelda = 32;

    int cantEmisores = 4;
    //cantidad de rayos que sale de cada emisor
    int cantRayos = 10;

    // epsilon para EG
    double eps;
    eps = 0.00001;

    // opcionalmente el epsilon se puede recibir como parametro


    //levantar la imagen
    DOK Imagen(n);
    Imagen.cargarCsv(archivoEntrada, n);


    // generar los rayos, guardar pto origen y angulo para cada rayo. Luis:
    //Vector<pair<int, int>, double> generarRayosVertices(int tam,int cantRayos)

  //vector<pair<pair<double, double>, double>> generarRayosVertices(int tam, int cantRayos){
    //std::vector <pair<pair<double, double>, double>> rayos(cantRayos*cantEmisores);
    std::vector <pair<pair<double, double>, double>> rayos;
    rayos = generarRayosVertices(n,cantRayos);

    //vector<pair<pair<double, double>, double>> generarRayosVertices(int tam, int cantRayos){

//    calcular la matriz de distancias
    DOK mDistancias( (cantRayos*cantEmisores), int ( (n/dimCelda)*(n/dimCelda)) ) ;
   distancia(n, n, dimCelda, dimCelda, rayos, mDistancias);

// calcular el vector de tiempos exactos de cada rayo
    //Vector vtiempos(cantRayos*cantEmisores);
    std::vector<double> vtiempos(cantRayos*cantEmisores);
    armarVectorTiempos(n, Imagen, rayos, vtiempos);


    // Calcular las intensidades promedio de las celdas de la matriz discreta
//Vector promediarIntensidadesXCelda(DOK I, int n, int d)
    Vector intensidadPromedioXCelda( (n/dimCelda)*(n/dimCelda));

    // TODO
    intensidadPromedioXCelda = promediarIntensidadesXCelda(Imagen, dimCelda);
cout<<"hola"<<endl;
    // se agrega ruido al vector de tiempos de rayos
    //void agregarRuidoRayo(std::vector<double> vectorRayo,float alpha){
    //TODO
   agregarRuidoRayo(vtiempos, ruido);

// cuadrados minimos: se llama con el vector de tiempos con ruido y la matriz de distancias
    Vector resultadoCM ((n/dimCelda)*(n/dimCelda));
    resultadoCM = mDistancias.CuadradosMinimos(vtiempos);

// convertir los valores calculados en velocidades
    Vector intensidadCM (resultadoCM.size());
    intensidadCM = calcularIntensidad (resultadoCM);

//    error cuadratico reportar a la salida estandar
    //std::cout << "ECM: " << ErrorCuadraticoMedio(intensidadPromedioXCelda, intensidadCM);

//    convertir y grabar la imagen
    grabarPPM8Bits(archivoSalida, n/dimCelda, n/dimCelda, to8bits(intensidadCM));

    return 0;
}



