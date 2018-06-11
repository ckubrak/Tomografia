#include "kfold.h"
#include "imagen.h"
//#include <cstdlib>
#include "pca.h"
#include "knn.h"

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
