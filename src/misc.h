#ifndef _MISC_H_
#define _MISC_H_

#include "imagen.h"
//#include "kfold.h"
#include "sparse.h"

#define SIN_INFORMACION_PPM_16 -2000
#define MAX_PPM_16 2^15
#define CERO_PPM_16 0

typedef std::vector<double> doubleVector;
typedef std::vector<doubleVector> doubleMatrix; //matriz para deflacion

int main_kfold(int pca, int k, int alfa, int kknn);
double ErrorCuadraticoMedio(Vector &b, Vector &bcalculado);
Vector to8bits(Vector &I);
void grabarPPM8Bits(std::string nombre_archivo, int filas, int columnas, Vector I);
Vector calcularIntensidad (Vector &v);
Vector promediarIntensidadesXCelda(DOK &I, int d);
vector<pair<pair<double, double>, double>> generarRayosVertices(int tam,int cantRayos);
vector<pair<pair<double, double>, double>> generarRayosCuadricula(int tam, int cantRayos);
void printVector(doubleVector v);
void printMatrix(doubleMatrix M);

vector<pair<pair<double, double>, double>> generarRayosUniformes(int tam, int cantEmisores, int cantRayosPorEmisor);
void grabarRayos(std::string nombre_archivo, int n, info rayos);
#endif
