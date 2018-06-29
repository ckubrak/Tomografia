#ifndef SPARCE_H
#define SPARCE_H
#include <limits>
#include <iomanip>
#include <type_traits>
#include <algorithm>
#include <cmath>       /* fabs */
#include <vector>
#include <iostream>
#include <map>
#include <iterator>
#include <fstream>
#include <unordered_map>
#include "rayos.h"

#define EPSILON_EG 0.00001
typedef std::vector<double > Vector;

//typedef std::vector <std::pair<std::pair<double,double>,double>> info;
class DOK
{
public:
    typedef std::map<size_t, std::map<size_t , double> > matriz;
    typedef matriz::iterator iter_fila;

    typedef std::map<size_t, double> columnas;
    typedef columnas::iterator iter_col;

    DOK (size_t n);
    DOK (size_t n, size_t c);   // Crea una matriz de n filas por c columnas
//    DOK (size_t n, double val); // Crea matriz diagonal
    DOK(const char* input);
    DOK(DOK& m);

    void sumarMatrices(DOK& m);
    void restarMatrices(DOK& m);
    Vector eliminacionGauss (Vector& b, double eps);
    Vector operator*(const Vector& x);

    void multiplicarConstante(double c);
    DOK multiplicarMatriz(DOK& m);
    DOK productoAtraspuestaPorA();
    Vector CuadradosMinimos(Vector b);


    double& operator()(size_t i, size_t j)
    {
        return _mat[i][j];
    }

    std::vector<Vector> matrizCompleta();
    DOK crearD();

    size_t size()
    {
        return _n;
    }

    matriz data()
    {
        return _mat;
    }
    
    size_t cols()
    {
        return _c;
    }

    size_t cantidadLinks()
    {
        return _m;
    }

    void cargarCsv(std::string csv, int n);

    void distancia(int n, int m, int a, int b, info rayos);

private:
    matriz _mat;
    size_t _n; // cantidad de filas
    size_t _c; // cantidad de columnas
    size_t _m; // Cantidad de links, elementos no nulos

    int Cj(int j, DOK& m);
    Vector resolverSistema();
    Vector matrizTraspuestaXVector(Vector &b);
};


void mostrarMatriz(DOK&);
#endif
