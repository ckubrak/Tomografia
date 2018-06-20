#ifndef _MISC_H_
#define _MISC_H_

#include "imagen.h"
#include "kfold.h"
#include "pca.h"
#include "knn.h"
#include "sparse.h"

#define SIN_INFORMACION_PPM_16 -2000
#define MAX_PPM_16 2^15
#define CERO_PPM_16 0

int main_kfold(int pca, int k, int alfa, int kknn);
double ErrorCuadraticoMedio(Vector &b, Vector &bcalculado);
int grabarPPM8Bits(std::string nombre_archivo, int filas, int columnas, Vector I);
#endif
