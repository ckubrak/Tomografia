
#ifndef _RAYOS_H
#define _RAYOS_H
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <map>
#include <unordered_map>

using namespace std;

typedef std::vector <std::pair<std::pair<double,double>,double>> info;

void pasa(int n, int m, double x, double y, double angulo, vector<pair<int, int>> &solucion);
typedef map<size_t, map<size_t , double> > matriz;
double tiemporayo(int n, int m , matriz velocidad, double x, double y, double angulo);



#endif