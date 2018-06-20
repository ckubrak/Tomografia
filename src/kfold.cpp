#include "kfold.h"
#include "imagen.h"
//#include <cstdlib>
#include "pca.h"
#include "knn.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>       /* floor */
#include "sparse.h"



int cantClases;
int imgPorClase;

using namespace std;


vector<vector<pair<int, int>>> k_fold(baseDeDatos bd, int k, int pca, int alfa, int kknn){

	cantClases = cantidadClases(bd);
	imgPorClase = cantidadImagenesPorClase(bd);

	int cantImagATestear = (imgPorClase/k)*cantClases;
	vector<vector<pair<int, int>> > resultado (k, vector<pair<int, int>>(cantImagATestear));
	int i = 0;

	while(i < k){

		int cantImagATestearPorClase = imgPorClase/k;
		std::vector<std::vector<int>> vectorDeIndices;
		int j1 = 0;
		while(j1+1 < imgPorClase+1){
			std::vector<int> aux;
			for(int j2 = 0; j2 < cantImagATestearPorClase ; j2++){
				aux.push_back(j1+1);
				j1++;
			}
			vectorDeIndices.push_back(aux);
		}
		resultado[i] = (iFold(bd, vectorDeIndices[i], pca, alfa, kknn));

		i++;
	}
	return resultado;
}

//Los indices que recibe iFold son qué imagenes de las 10 vamos a tomar para testear.
vector<pair<int, int>> iFold(baseDeDatos bd, vector<int> indices, int pca, int alfa	,int k){

	cantClases = cantidadClases(bd);
	imgPorClase = cantidadImagenesPorClase(bd);

	int cantidad_imagenes_test = indices.size()*cantClases;
	baseDeDatos trainingBase = {};//Para construir base de datos para testear.
	//Aca van a estar las imagenes que vamos a usar para testear.
 	baseDeDatos imagenesTest{};

	int i = 0;
	int cantImagenesTotales = cantClases * imgPorClase;//en este caso 41*10
	while(i < cantImagenesTotales){
		if(!apareceEn(bd[i], indices)){
			trainingBase.push_back(bd[i]);
		}else{
			imagenesTest.push_back(bd[i]);
		}
		i++;
	}

	i = 0;
	//Armo vector resultado vacío. En cada índice va el resultado de evaluar cada una de las imagenes a testear
	std::vector<pair<int,int>> resultado(imagenesTest.size());
	if(pca == 1){

		int filas, columnas;
		filas = bd.size();
	    columnas=bd[0].getWidth()*bd[0].getHeight();

		doubleMatrix matrizCaracteristicaMuestra(trainingBase.size(), doubleVector(alfa));
	  doubleMatrix cambioDeBaseTras (alfa, doubleVector(columnas));
		doubleVector media (columnas);

		PCA(trainingBase, cambioDeBaseTras, media, matrizCaracteristicaMuestra, alfa);

		while(i < imagenesTest.size()){
			resultado[i].first = imagenesTest[i].getId();//El id original, el que debería dar.

		    doubleVector imagenNormalizada(columnas);
		    imagenNormalizada = ucharToDoubleVector (imagenesTest[i].getData(), columnas);

			imagenNormalizada = normalizarImagen(imagenNormalizada, media, cambioDeBaseTras, matrizCaracteristicaMuestra);
			resultado[i].second = modaPCA(k, matrizCaracteristicaMuestra, imagenNormalizada, trainingBase);//PCA+KNN
			i++;
		}
	}
	if(pca == 0){//Solo KNN
		while(i < imagenesTest.size()){
			std::pair <int,int> parResultado;
			parResultado = std::make_pair (imagenesTest[i].getId(),moda(k,trainingBase,imagenesTest[i]));
			resultado[i] = parResultado;
			i++;
		}
	}
	return resultado;
}

bool apareceEn(Imagen img, vector<int> indices){
	int i = 0;
	while(i < indices.size()){
		if(img.getName() == indices[i]){
			return true;
		}
		i++;
	}
	return false;
}

// Se asume que todas las clases tienen la misma cantidad de imagenes, es decir que la base esta balanceada
int cantidadImagenesPorClase(baseDeDatos& bd)
{
  //recorro la bd contando los elementos que tienen igual id
  int tam=bd.size();
  int idanterior;
  int contador=0;
  if (tam > 0)
      idanterior = bd[0].getId();
  int i=0;
  while(i<tam && idanterior==bd[i].getId())
  {
    contador++;
    i++;
  }
  return contador;
}

//se asume todas las clases tienen la misma cantidad de imagenes
int cantidadClases(baseDeDatos& bd)
{
	if (bd.size()>0)
		return (bd.size()/cantidadImagenesPorClase(bd));
	else
		return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////

for (int i = 0; i < _n; i++)
    {
        for (int j = 0; j < _m; j++)
        {
            
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////

for (int i = 0; i <size(); i++)
{
        for (int j = 0; j < _m; j++)
        {
            if (_mat[i].count(j) > 0){
                _mat[i][j] = c * _mat[i][j];
            }
        }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////




//Esta función es para el otro método de agregado de ruido. Sobre el vector de tiempos de un rayo
//Recibimos una matriz esparsa y el alpha para regular el ruido
void DOK::agregarRuidoImagen(DOK& imag,float alpha){
	int tam = imag.size();
	std::vector<double> ruido (tam);
	srand (time(NULL));

	//Creo el vector con ruido que voy a sumar a la entrada
	for(int j = 0; j < tam; j++){
		double randomNum = rand() % 131071 + (-65535);
		ruido[j] = randomNum;
	}

	//Multiplico el vector ruido por el alpha para regular la cantidad de ruido
	for(int j = 0; j < tam; j++){
		double aux = ruido[j];
		ruido[j] = floor(aux*alpha);//Uso floor para sacarnos los decimales
	}

	//Ahora los sumamos
	int x1 = 0;
	for (int i = 0; i < imag.size(); i++)
	{
	        for (int j = 0; j < _m; j++)
	        {
	            if (imag._mat[i].count(j) > 0){
	                imag._mat[i][j] = .imag_mat[i][j] + ruido[x1];
	                x1++;
            		
            		if(imag._mat[i][j] > 65535){
						imag._mat[i][j] = 65535;
					} else if(imag._mat[i][j] < 0){
						imag._mat[i][j] = 0;
					}

	            }
	        }
	}




	/*//Ahora los sumamos
	for(int i = 0; i < tam; i++){
		if(imag[i].second() + ruido[i] > 65535){
			imag[i].second() = 65535;
		} else if(imag[i].second() + ruido[i] < 0){
			imag[i].second() = 0;
		} else {
			double aux1 = imag[i] + ruido[i];
			imag[i] = aux1;
		}
	}*/
}

//Asumo que el vectorRayo tiene esa forma
//El vector<pair<int, double>> guardamos el tiempo de recorrido. El int es la celda y el double es el tiempo
//alpha debería ir entre 0 y 1 por cómo lo dijo la catedra. Regula cuanto ruido agregamos
void agregarRuidoRayo(std::vector<double> vectorRayo,float alpha){
	int tam = vectorRayo.size();
	std::vector<double> ruido (tam);
	srand (time(NULL));

	//Calculamos el mayor tiempo, lo vamos a usar para el rango de ruido. ELECCION NUESTRA
	double maxVal [0]; 
	for (int m = 0; m < tam; m++){
		if(vectorRayo[m] > maxVal){
			maxVal = vectorRayo[m];
		}
	}

	int hasta = 2*(maxVal) - 1;
	int desde;
	//Esto es solo en caso de que la imagen esté completamente en blanco.
	//No es un caso muy probable pero lo cubro para que no rompa
	if(maxVal = 0){
		desde = 0;
	} else{
		desde = -(maxVal-1);
	}

	//Creo el vector con ruido que voy a sumar a la entrada
	for(int j = 0; j < tam; j++){
		double randomNum = rand() % hasta + (desde);
		ruido[j] = randomNum;
	}

	//Multiplico el vector ruido por el alpha para regular la cantidad de ruido
	for(int j = 0; j < tam; j++){
		double aux = ruido[j];
		ruido[j] = floor(aux*alpha);//Uso floor para sacarnos los decimales
	}

	//Ahora los sumamos
	for(int i = 0; i < tam; i++){
		if(vectorRayo[i] + ruido[i] > 65535){
			vectorRayo[i] = 65535;
		} else if(vectorRayo[i] + ruido[i] < 0){
			vectorRayo[i] = 0;
		} else {
			double aux1 = vectorRayo[i] + ruido[i];
			vectorRayo[i] = aux1;
		}
	}

}