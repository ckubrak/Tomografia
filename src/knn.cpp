#include "knn.h"
#include "pca.h"
#include "imagen.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include <utility>
#include <cmath>

using namespace std;

const int cantClases (41); 

// 1) Guardar el valor del primer pixel
// 2) Recorrer la matriz de entrenamiento
// 3) Calcular distancia euclidea entre la imagen a evaluar y la imagen i, 1<= i <= k
// 4) Guardar los K mas valores mas chicos. 

double distanciaEuclidea(Imagen &imag1, Imagen &imag2) {
	double distancia = 0;
	int tam = imag1.getHeight()*imag1.getWidth();//Asumo que las dos imagenes son del mismo tamaño
	int i = 0;
	double x;
	int y;
	for(i = 0; i < tam; i++){
		x = imag2.getData()[i] - imag1.getData()[i];
		distancia += x*x;
	}
	return sqrt(distancia);
}

vector<int> k_cercanosSort(int k, baseDeDatos &matEntrenamiento, Imagen &imagenAEvaluar){

	int i = 0;
	int j = 0;
	double distancia = 0;
	std::pair<double,int> indiceDistancia;
    std::vector<std::pair<double,int>> vectorKCercanos;
    std::vector<int> k_vecinos_cercanos (k);//RESULTADO
	
	int tamMatEntrenamiento = 0;
	tamMatEntrenamiento =  matEntrenamiento.size();

	while(i < tamMatEntrenamiento){
		distancia = distanciaEuclidea(matEntrenamiento[i], imagenAEvaluar);
		indiceDistancia.first = distancia;
		indiceDistancia.second = matEntrenamiento[i].getId();
		vectorKCercanos.push_back(indiceDistancia);
		i++;
	}
	std::sort (std::begin(vectorKCercanos), std::end(vectorKCercanos));

	//Armo el vector que voy a devolver

	//Recorro lo que me quedó en la lista de prioridad y lo pongo en el vector. Agarro solo los indices. 
    for (int x = 0; x < k; x++){
		k_vecinos_cercanos[x] = vectorKCercanos[x].second;
	}
	return k_vecinos_cercanos;
}




vector<int> k_cercanos(int k, baseDeDatos &matEntrenamiento, Imagen &imagenAEvaluar){

	int i = 0;
	int j = 0;
	double distancia = 0;
	std::pair<double,int> indiceDistancia;
    std::priority_queue<std::pair<double,int>> colaKcercanos;
	int tamMatEntrenamiento = 0;
	tamMatEntrenamiento =  matEntrenamiento.size();

	while(i < tamMatEntrenamiento){
		distancia = distanciaEuclidea(matEntrenamiento[i], imagenAEvaluar);
		indiceDistancia.first = distancia;
		indiceDistancia.second = matEntrenamiento[i].getId();
		colaKcercanos.push(indiceDistancia);//Agrego el pair a la cola. Deberían quedar ordenados por distancia
		i++;
	}

	//La cola de prioridad ordena de mayor a menor. Le saco los n-k de adelante y me quedan los k elementos más chicos.
	int aux = tamMatEntrenamiento - k;
	while(aux > 0){
		colaKcercanos.pop();
		aux-- ;
	}

	//Armo el vector que voy a devolver
	std::vector<int> k_vecinos_cercanos(k, 0);//De longitud k, inicializado en ceros. 
	aux = 0;
	pair<double, int> pairAux;
	//Recorro lo que me quedó en la lista de prioridad y lo pongo en el vector. Agarro solo los indices. 
	while(aux < k){
		pairAux = colaKcercanos.top();//Accedo al tope de la cola
		k_vecinos_cercanos[k-aux] = pairAux.second;
		colaKcercanos.pop();
		aux++;
	}

	return k_vecinos_cercanos;
}
int moda(int k, baseDeDatos &matEntrenamiento, Imagen &imagen){
	std::vector<int> k_vecinos_cercanos(k, 0);
	k_vecinos_cercanos = k_cercanosSort(k, matEntrenamiento, imagen);
	std::vector<int> vectorModa(cantClases, 0);
	int i = 0;
	int aux;
	while(i < k_vecinos_cercanos.size() ){
		aux = k_vecinos_cercanos[i] - 1;
		vectorModa[aux] ++;
		i++;
	}
	int result = maximoIndice(vectorModa);
    return result;
}

int maximoIndice(vector<int> &vectorModa){
	int maximoElemento = *max_element(vectorModa.begin(), vectorModa.end());
	for(int i = 0; i < vectorModa.size(); i++){
		if(vectorModa[i] == maximoElemento){
			return i+1;
		}
	}
}

//PCA + KNN
double distanciaEuclideaPCA(doubleVector &imag1, doubleVector &imag2) {
	double distancia = 0;						
	int tam = imag1.size();//Asumo que las dos imagenes son del mismo tamaño
	int i = 0;
	double x;
	int y;
	for(i = 0; i < tam; i++){
		x = imag2[i] - imag1[i];
		distancia += x*x;
	}
	return sqrt(distancia);
}

vector<int> k_cercanosPCA(int k, doubleMatrix &matEntrenamiento, doubleVector &imagenAEvaluar, baseDeDatos &baseSinTransformar){

	int i = 0;
	double distancia = 0;

	std::pair<double,int> indiceDistancia;
    std::priority_queue<std::pair<double,int>> colaKcercanos;

	int tamMatEntrenamiento = 0;
	tamMatEntrenamiento =  matEntrenamiento.size();
	while(i < tamMatEntrenamiento){
		distancia = distanciaEuclideaPCA(matEntrenamiento[i], imagenAEvaluar);
		indiceDistancia.first = distancia;
		indiceDistancia.second = baseSinTransformar[i].getId();//el "id" de esa imagen
		colaKcercanos.push(indiceDistancia);//Agrego el pair a la cola. Deberían quedar ordenados por distancia
		i++;
	}

	//La cola de prioridad ordena de mayor a menor. Le saco los n-k de adelante y me quedan los k elementos más chicos.
	int aux = tamMatEntrenamiento - k;
	while(aux > 0){
		colaKcercanos.pop();
		aux-- ;
	}

	//Armo el vector que voy a devolver
	std::vector<int> k_vecinos_cercanos(k, 0);//De longitud k, inicializado en ceros. 
	aux = 0;
	pair<double, int> pairAux;
	//Recorro lo que me quedó en la lista de prioridad y lo pongo en el vector. Agarro solo los indices. 
	while(aux < k){
		pairAux = colaKcercanos.top();//Accedo al tope de la cola
		k_vecinos_cercanos[aux] = pairAux.second;
		colaKcercanos.pop();
		aux++;
	}

	return k_vecinos_cercanos;
}

vector<int> k_cercanosPCASort(int k, doubleMatrix &matEntrenamiento, doubleVector &imagenAEvaluar, baseDeDatos &baseSinTransformar){

	int i = 0;

	double distancia = 0;
	std::pair<double,int> indiceDistancia;
    std::vector<std::pair<double,int>> vectorKCercanos;
	
	std::vector<int> k_vecinos_cercanos;//Resultado 

	int tamMatEntrenamiento = 0;
	tamMatEntrenamiento =  matEntrenamiento.size();

	while( i < matEntrenamiento.size() ){
		distancia = distanciaEuclideaPCA(matEntrenamiento[i], imagenAEvaluar);
		indiceDistancia.first = distancia;
		indiceDistancia.second = baseSinTransformar[i].getId();
		vectorKCercanos.push_back(indiceDistancia);
		i++;
	}
	std::sort (std::begin(vectorKCercanos), std::end(vectorKCercanos));

	for(int x = 0; x < k; x++){
		k_vecinos_cercanos.push_back(vectorKCercanos[x].second);
	}

	return k_vecinos_cercanos;
}



int modaPCA(int k, doubleMatrix &matEntrenamiento, doubleVector &imagen, baseDeDatos &baseSinTransformar){
	std::vector<int> k_vecinos_cercanos(k, 0);
	k_vecinos_cercanos = k_cercanosPCASort(k, matEntrenamiento, imagen, baseSinTransformar);
	std::vector<int> vectorModa(cantClases, 0);
	int i = 0;
	int aux = 0;
	while( i < k_vecinos_cercanos.size() ){
		aux = k_vecinos_cercanos[i] - 1;
		vectorModa[aux] ++;
		i++;
	}
	int result = maximoIndice(vectorModa);
    return result;
}
