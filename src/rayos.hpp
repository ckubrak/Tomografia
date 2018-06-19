#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
void pasa(int n, int m, double x, double y, double angulo, vector<pair<int, int>> &solucion)
//n es cantidad de filas de pixeles
//m es cantidad de columnas de pixeles
//el angulo se escribe como en una circunferencia. Por ejemplo un rayo hacia abajo seria 270
//x e y que son el punto desde donde se traza el rayo, siedno x e y sus coordenadas x e y respectivamente
//solucion devuelve pixeles por los que pasa el rayo numerados x*n+y siendo x e y las coordenas de su vertice superior izquierdo
//la imagen tiene su vertice inferior izquierdo en 0, 0. los otros pixeles se encuentran en las coordenas enteras con 0<=x<m y 0<=y<n
{
    double tangent=tan(angulo*3.14159265/180);
    if (abs(tangent)>1) //mas cercano a ser paralelo al eye y que al x
    {
        double interseccion=x+(-y)/tangent;
        for (int i=1;(i<=n);i++)
        {
            double corte=interseccion+1/tangent;
            if (corte==interseccion) //caso especial para que un rayo que pase por los limites de pixeles no sea considerado inexistente
            {
                int j=floor(corte);
                if (j>=0 && j<m)
                {
                    solucion.push_back(make_pair(j,i-1));
                }
            }else if (corte>interseccion)
            {
                for (int j=floor(interseccion);j<=floor(corte);j++) //si el rayo pasa por una esquina entre pixeles ocurren comportamientos interesantes
                {
                        if (j>=0 && j<m)
                        {
                            solucion.push_back(make_pair(j,i-1));
                        }
                }

            } else if (corte<interseccion)
            {
                 for (int j=floor(interseccion);j<=floor(corte);j--)//si el rayo pasa por una esquina entre pixeles ocurren comportamientos interesantes
                 {
                      if (j>=0 && j<m)
                        {
                            solucion.push_back(make_pair(j,i-1));
                        }
                 }
            }
            interseccion=corte;

        }
    } else
    {
        double interseccion=y+(-x)*tangent;
        for (int i=1;(i<=m);i++)
        {
            double corte=interseccion+tangent;
            if (corte==interseccion) //caso especial para que un rayo que pase por los limites de pixeles no sea considerado inexistente
            {
                int j=floor(corte);
                if (j>=0 && j<n)
                {
                    solucion.push_back(make_pair(i-1,j));
                }
            } else if (corte>interseccion)
            {
                for (int j=floor(interseccion);j<=floor(corte);j++) //si el rayo pasa por una esquina entre pixeles ocurren comportamientos interesantes
                {
                        if (j>=0 && j<n)
                        {
                            solucion.push_back(make_pair(i-1,j));
                        }
                }

            } else if (corte<interseccion)
            {
                 for (int j=floor(interseccion);j<=floor(corte);j--)//si el rayo pasa por una esquina entre pixeles ocurren comportamientos interesantes
                 {
                      if (j>=0 && j<n)
                        {
                            solucion.push_back(make_pair(i-1,j));
                        }
                 }
            }
            interseccion=corte;

        }
    }

}
