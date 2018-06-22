#include "ruido.h"

//Esta función es para el otro método de agregado de ruido. Sobre el vector de tiempos de un rayo
//Recibimos una matriz esparsa y el alpha para regular el ruido
void agregarRuidoImagen(DOK& imag,float alpha){
	int tam = imag.cantidadLinks();//Voy a hacer un vector de ruido de la misma cantidad de elementos != 0 que tiene la imagen
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
	double maxVal; 
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