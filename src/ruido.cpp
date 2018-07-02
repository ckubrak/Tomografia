#include "ruido.h"
#include <chrono>
#include <ctime>
#include <iostream>
#include <random>


//Esta función es para el otro método de agregado de ruido. Sobre el vector de tiempos de un rayo
//Recibimos una matriz esparsa y el alpha para regular el ruido
void agregarRuidoImagen(DOK& imag,float alpha){
	int tam = imag.cantidadLinks();//Voy a hacer un vector de ruido de la misma cantidad de elementos != 0 que tiene la imagen
	std::vector<double> ruido (tam);
	srand (time(NULL));

	//Con esto creo los numeros aleatorios con distribución N(0;1)
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);

	std::normal_distribution<double> distribution (0.0,1.0);

	//Creamos el vector de ruido
	for (int i=0; i < tam; ++i){
	  ruido[i] = distribution(generator);
	}

	//Multiplicamos el vector ruido por el alpha para regular la cantidad de ruido
	for(int j = 0; j < tam; j++){
		double aux = ruido[j];
		ruido[j] = floor(aux*alpha*1000);//Uso floor para sacarnos los decimales
	}

	//Ahora los sumamos
	int x1 = 0;
	for (int i = 0; i < imag.size(); i++)
	{
	        for (int j = 0; j < imag.size(); j++)
	        {
	            if ( (imag.data().count(i) > 0) && (imag.data()[i].count(j) > 0) ){//Si hay algo en la posicion (i,j) de la matriz
	                imag.data()[i][j] = imag.data()[i][j] + ruido[x1];
	                x1++;
            		
            		if(imag.data()[i][j] > 65535){
						imag.data()[i][j] = 65535;
					} else if(imag.data()[i][j] < 0){
						imag.data()[i][j] = 0;
					}

	            }
	        }
	}//Ahora x1 debería ser igual a tam
}



//Asumo que el vectorRayo tiene esa forma
//El vector<pair<int, double>> guardamos el tiempo de recorrido. El int es la celda y el double es el tiempo
//alpha debería ir entre 0 y 1 por cómo lo dijo la catedra. Regula cuanto ruido agregamos
void agregarRuidoRayo(std::vector<double> &vectorRayo,float alpha){


	if (alpha != 0)
	{
		
		int tam = vectorRayo.size();
		std::vector<double> ruido (tam);
		srand (time(NULL));

		// construct a trivial random generator engine from a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);

		std::normal_distribution<double> distribution (0,1);

		for (int i=0; i < tam; ++i){
		ruido[i] = distribution(generator);
		}

		//Multiplico el vector ruido por el alpha para regular la cantidad de ruido
		for(int j = 0; j < tam; j++){
		double aux = ruido[j];
		ruido[j] = floor(aux*alpha*vectorRayo[j]);//Uso floor para sacarnos los decimales
		}//y multiplico por 1000 para que el alpha este entre 0 y 1, solo por eso.
		//Los valores del vectorRuido van a ser chicos por que es una normal(0,1)

		//Ahora los sumamos
		for(int i = 0; i < tam; i++){
			if(vectorRayo[i] + ruido[i] < 0)
			{
				vectorRayo[i] = 0;
			} else
			{
				double aux1 = vectorRayo[i] + ruido[i];
				vectorRayo[i] = aux1;
			}
		}
	} // alpha != 0
}



void agregarRuidoRayoAditivo(std::vector<double> &vectorRayo,float alpha){

	if (alpha != 0)
	{	
		int tam = vectorRayo.size();
		std::vector<double> ruido (tam);
		srand (time(NULL));

		// construct a trivial random generator engine from a time-based seed:
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);
		std::normal_distribution<double> distribution (0,1.0);

		for (int i=0; i < tam; ++i){
		ruido[i] = distribution(generator);
		}
		//Multiplico el vector ruido por el alpha para regular la cantidad de ruido
		for(int j = 0; j < tam; j++){
		double aux = ruido[j];
		ruido[j] = floor(aux*alpha*5000);//Uso floor para sacarnos los decimales
		}//y multiplico por 1000 para que el alpha este entre 0 y 1, solo por eso.
		//Los valores del vectorRuido van a ser chicos por que es una normal(0,1)

		//Ahora los sumamos
		for(int i = 0; i < tam; i++){
			if(vectorRayo[i] + ruido[i] < 0)
			{
				vectorRayo[i] = 0;
			} else
			{
				double aux1 = vectorRayo[i] + ruido[i];
				vectorRayo[i] = aux1;
			}
		}
	}
}