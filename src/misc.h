#ifndef _MISC_H_
#define _MISC_H_

#include "imagen.h"
#include "kfold.h"
#include "pca.h"
#include "knn.h"


int main_kfold(int pca, int k, int alfa, int kknn);
double ErrorCuadraticoMedio(Vector &b, Vector &bcalculado);
#endif
