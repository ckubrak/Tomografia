#ifndef _PCA_H_
#define _PCA_H_
#include "imagen.h"
typedef std::vector<Imagen> baseDeDatos; //matriz de Imagenes: id, ancho, alto y bytes de cada imagen
typedef std::vector<double> doubleVector;
typedef std::vector<doubleVector> doubleMatrix; //matriz para deflacion

double norma2Vectorial(doubleVector v);
double normaInfinitoVectorial(doubleVector v, int &indice);

doubleVector sumaVectorial(doubleVector a, doubleVector b);
doubleVector restaVectorial(doubleVector a, doubleVector b);


// matriz y vectores no vacios
doubleVector matrizXVector(doubleMatrix A, doubleVector v);

doubleMatrix matrizXEscalar(doubleMatrix A, double esc);

doubleVector vectorXEscalar(doubleVector v, double esc);

doubleMatrix matrizTraspuesta (doubleMatrix A);

doubleVector ucharToDoubleVector (uchar* data, int longitud);

int MetodoPotencias(doubleMatrix A, doubleVector x,int nroIter,float tol, double &autoval, doubleVector &autovec);

// se calcula X*Xt. Por ahora no lo uso: se obtiene una matriz de n x n (n=cantidad de imagenes)
void CalcularCovarianza(doubleMatrix X, int filas, int columnas, doubleMatrix &mcov);

//Arma en el parametro de Salida X una matriz con las imagenes normalizadas
void ArmarMatrizX(baseDeDatos muestra, doubleVector media, int filas, int columnas, doubleMatrix &X );

doubleMatrix MatrizX(baseDeDatos muestra, doubleVector media, int filas, int columnas );

//void PCA (baseDeDatos muestra, doubleMatrix &mtrcar, int modo);
void PCA (baseDeDatos muestra, doubleMatrix &cambioDeBase, doubleVector &media, doubleMatrix &matrizCaracteristicaMuestra, int alfa);
doubleVector normalizarImagen (doubleVector img, doubleVector media, doubleMatrix cambioDeBaseTras, doubleMatrix matrizCaracteristicaMuestra);
void printVector(doubleVector v);
void printMatrix(doubleMatrix M);

#endif
