#ifndef _KNN_H_
#define _KNN_H_

#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <queue>
#include <cmath>
#include "pca.h"
#include "imagen.h"

using namespace std;


double distanciaEuclidea(const doubleVector &v1, const doubleVector &v2);
vector<int> k_cercanos(int k, baseDeDatos &matEntrenamiento, Imagen &imagenAEvaluar);
vector<int> k_cercanosSort(int k, baseDeDatos &matEntrenamiento, Imagen &imagenAEvaluar);
int moda(int k, baseDeDatos &matEntrenamiento, Imagen &imagen);
int maximoIndice(vector<int> &vectorModa);


double distanciaEuclideaPCA(doubleVector &imag1, doubleVector &imag2);
vector<int> k_cercanosPCA(int k, doubleMatrix &matEntrenamiento, doubleVector &imagenAEvaluar, baseDeDatos &baseSinTransformar);
vector<int> k_cercanosPCASort(int k, doubleMatrix &matEntrenamiento, doubleVector &imagenAEvaluar, baseDeDatos &baseSinTransformar);
int modaPCA(int k, doubleMatrix &matEntrenamiento, doubleVector &imagen, baseDeDatos &baseSinTransformar);

#endif
