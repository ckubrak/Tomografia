#ifndef _KFOLD_H_
#define _KFOLD_H_

#include <vector>
#include <math.h>       /* sqrt */
#include <string>
#include <iostream>
#include <fstream>
#include "pca.h"
#include "knn.h"



std::vector<std::vector<std::pair<int, int>>> k_fold(baseDeDatos bd, int k, int pca, int alfa, int kknn);
std::vector<std::pair<int, int>> iFold(baseDeDatos bd, std::vector<int> indices, int pca, int alfa, int kknn);
bool apareceEn(Imagen img, std::vector<int> indices);
int cantidadImagenesPorClase(baseDeDatos& bd);
int cantidadClases(baseDeDatos& bd);
#endif
