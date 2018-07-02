#include "misc.h"
#include <math.h>       /* ceil */

double ErrorCuadraticoMedio(Vector &b, Vector &bcalculado)
{
    double ecm=0;
    size_t cantElem;

    cantElem = b.size();

    for (int i=0; i<cantElem; ++i)
    {
        ecm += pow((b[i]-bcalculado[i]),2);
    }

    ecm = ecm/pow(cantElem,2);

    return ecm;
}

/*
Como resultado de la resolucion del sistema de cuadrados mínimos se puede obtener un vector
de resultados que representan intensidades de pixeles que puede estar fuera de los rangos
aceptados por la codificación ppm 16 bits que estamos usando, ademas que son doubles.
Cada pixel se guarda como enteros entre -2^15 y 2^15 donde los valores negativos indican ausencia de información.
Para transformar los resultados a un rango válido de valores se decide:
- los negativos cercanos a cero se convierten a cero.
- Los negativos no cercanos a cero se convierten en -2000 NO PASA
- Los positivos que excedan de 2^15 se convierten a 2^15

SIN_INFORMACION_PPM_16 -2000
MAX_PPM_16 2^15
CERO_PPM_16 0

Parametros de entrada:
Vector con las velocidades de cada celda de la imagen discretizada
Salida: Vector con las intensidades de cada celda de la imagen discretizada
Procedimiento: Se hallan las inversas de los valores recibidos y se redondean para obtener un entero
Si hay valores obtenidos estan fuera del rango valido de intensidades, se convierten a un valor dentro del rango
*/
Vector calcularIntensidad (Vector &v)
{
    size_t cant_elem = v.size();
    Vector I(cant_elem);

    for (int i=0; i<cant_elem; ++i)
    {
        if (fabs(v[i]) > 0.0000001)
            I[i] = round(1/v[i]);
        else
            I[i] = MAX_PPM_16;

        if ( I[i] < 0 )
            I[i]=0;
        else if (I[i] > MAX_PPM_16)
                I[i] = MAX_PPM_16;
    }
    return I;
}

/*
Dado un vector con los pixeles de una imagen codificada en 16 bits de 0 a 2^15
convertir en ppm 8 bits para poder grabarlo y visualizarlo con un editor de imágenes standard
Tomamos el menor valor de la imagen y lo equiparamos a cero, el maximo a 255. Los valores intermedios
se convierten proporcionalmente
*/
Vector to8bits(Vector &I)
{
    size_t cant_elem = I.size();
    Vector I8(cant_elem);
    double max_val, min_val;


    max_val =I[0];
    min_val = I[0];
    for (int i=0; i<cant_elem; ++i)
    {
        if (max_val < I[i])
            max_val =I[i];
        if (min_val > I[i])
            min_val =I[i];
    }

    for (int i=0; i<cant_elem; ++i)
    {
        // opcion 1: regla de tres simples
        //I8[i] = round( (I[i] * 255 ) / MAX_PPM_16);

        // opcion 2: tomar el minimo como cero y el maximo como 255
        I8[i] = round( (I[i]-min_val) * 255 / (max_val-min_val));

        if ( I8[i] > 255 ) // analizar si realmente puede irse de rango al redondear, lo ponemos x las dudas
        {
            I8[i]=255;
        }
    }
    // printVector(I8);
    return I8;

}


/*
Parametros:
Entrada: string con el nombde del archivo de salida sin extension,
 cantidad de filas de la imagen,
 cantidad de columnas de la imagen
 std::vector conteniendo en cada posicion un valor entre 0 y 255 que representa la intensidad de un pixel.
*/

void grabarPPM8Bits(std::string nombre_archivo, int filas, int columnas, Vector I)
{
    size_t cantPixeles = I.size();

    std::ofstream output(nombre_archivo += ".pgm" );
    output << "P5" << "\n";
    output << filas <<  " " << columnas << "\n";
    output << 255 << "\n";
    // escribir de corrido los unsigned char que representan los valores de intensidad de c/pixel de la imagen
    for (int i=0; i<cantPixeles; ++i)
    {
        output << (unsigned char) I[i];
    }
    output.close();
    return;
}

/*
Parametros:
Entrada:
- I: matriz esparza conteniendo una imagen cuadrada
- n: cantidad de pixeles x lado de la imagen
- d: int divisor de n que indica la cantidad de pixeles de cada celda de la discretización
Salida:
Vector con el promedio de las intensidades de los pixeles de cada celda
*/
//Vector promediarIntensidadesXCelda(DOK I, int n, int d)
Vector promediarIntensidadesXCelda(DOK &I, int d)
{
    int i=0;
    int j=0;

    int a;
    int b;

    int n = I.size();
    int celdasXFila=n/d; // asumimos que d es divisor de n, cantidad de celdas por fila
    int celdasTotales=celdasXFila * celdasXFila;

    Vector R(celdasTotales, 0.0);
    std::map<size_t, std::map<size_t , double> > data = I.data();

    for (int i=0; i<n; ++i)
    {
        //i=a*d; //fila de los pixeles de las celdas de la fila a. Se inicializa en la primera fila de la celda.
        for (int j=0; j<n; ++j)
        {
            //std::cout << "(I.data().count(i): " << (I.data()).count(i) << " ((I.data())[i])).count(j): " << ((I.data())[i]).count(j) << "\n";
            if ( (data).count(i) && ( (data)[i]).count(j))
            {

                //if ((I.data())[i][j] > 0) // ignorar pixeles sin informacion (valores negativos)
                //{
                //std::cout << "i j valor pixel: " << i << " " << j << " " << (I.data())[i][j] << "\n";
                    // calcular en que celda esta el pixel
                    a = i/d;
                    b = j/d;
                    R[a*celdasXFila+b] += (data)[i][j];
                //}
            }
        }
    }
    // hallar el promedio de la intensidad de cada celda
    for (int a=0; a<celdasTotales; ++a)
    {
        R[a] = R[a]/(d*d);
    }
    return R;
}

//Recibo el tamano de la imagen y la cantidad de rayos.
//Supongo que la matriz es de tamano tam*tam
//Devuelvo el vector que en cada lugar tiene ((coordenadas de salida), anmgulo)
//La cantidad de rayos posta es cantRayos*4, para no tener problemas de divisibilidad
vector<pair<pair<double, double>, double>> generarRayosVertices(int tam, int cantRayos){

	std::vector<pair<pair<double, double>, double>> resultado (4*cantRayos);

	double aux = tam;
	pair<double, double> SI;// = (tam , 0) Extremo Superior Izquierdo
	SI.first = aux;
	SI.second = 0;

	pair<double, double> SD;// = (tam , tam ) Extremo Superior Derecho
	SD.first = aux;
	SD.second = aux;

	pair<double, double> II;// = (0, 0) Extremo Inferior Izquierdo
	II.first = 0;
	II.second = 0;

	pair<double, double> ID;// = (0, tam) Extremo Inferior Derecho
	ID.first = 0;
	ID.second = aux;

	double paso = 90/(cantRayos+1.0);//Cada vertice tiene 90̣̣̣̣̣° Esto son las particiones que tenemos en cada vertice
	int i = 0;

	//Armamos los ratos de la parte SI
	double x = 90 + paso;
	for (int j=0;j<cantRayos;j++){
		resultado[i].first = SI;
		resultado[i].second = x;
		i++;
		x = x + paso;
	}

	//Armamos los ratos de la parte SD
	x = 180 + paso;
	for (int j=0;j<cantRayos;j++){
		resultado[i].first = SD;
		resultado[i].second = x;
		i++;
		x = x + paso;
	}

	//Armamos los ratos de la parte II
	x = 0 + paso;
	for (int j=0;j<cantRayos;j++){
		resultado[i].first = II;
		resultado[i].second = x;
		i++;
		x = x + paso;
	}

	//Armamos los ratos de la parte ID
	x = 270 + paso;
	for (int j=0;j<cantRayos;j++){
		resultado[i].first = ID;
		resultado[i].second = x;
		i++;
		x = x + paso;
	}
	return resultado;


}

//Esta función lo que hace es generar cantRayos paralelos y equidistantes, tanto en el eje x como en el eje y
vector<pair<pair<double, double>, double>> generarRayosCuadricula(int tam, int cantRayos){

    cantRayos = tam;
	std::vector<pair<pair<double, double>, double>> resultado (2*cantRayos);

	double aux = tam;

	//Hago esto para tener los decimales y hacer el ceil. Si no, puede calcular más rayos de los pedidos.
	float tamAux = tam;
	double cantRayosAux = cantRayos;
	double paso = tamAux/cantRayosAux;

    paso = 1;

	//double paso = ceil(pasoAux);

	//El paso es la cantidad de celdas que hay entre cada rayo. Están más o menos equidistantes
	//Para que queden bien equidistantes tenemos que pasar (cantRayos+1) que divida a 512. Eso sería lo más recomendable
	double x = 0;
	int i = 0;

	pair<double, double> coordenadas;
	coordenadas.first = 0;
	coordenadas.second = 0;

	//Acá se generan los rayos del eje y
	for (int j=0;j<cantRayos;j++){
		coordenadas.first = x;
		resultado[i].first = coordenadas;//coordenadas = (x, 0)
		resultado[i].second = 90;//El ángulo

		i++;
		x = x + paso;
	}

	x = 0;
	coordenadas.first = 0;
	coordenadas.second = 0;
	//Acá se generan los rayos del eje x
	for (int j=0;j<cantRayos;j++){
		coordenadas.second = x;
		resultado[i].first = coordenadas;//coordenadas = (0, x)
		resultado[i].second = 0;//Sí, es medio redundante pero lo dejo para que quede claro que estamos haciendo
		//Podría usar 0 o 180 dependiendo de qué punta salga el rayo
		i++;
		x = x + paso;
	}
	return resultado;
}

//Esta funcion genera rayos que salen de emisores uniformemente distribuidos en cada lado de la matriz 
//cantRayosPorEmisor desde cada uno de esos puntos.
//Acá el ángulo va a ir desde 270 a 90
vector<pair<pair<double, double>, double>> generarRayosUniformes(int tam, int cantEmisores, int cantRayosPorEmisor){
    std::vector<pair<pair<double, double>, double>> resultado;
    pair<double, double> coordenadas;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, tam-2);//No agarro 0 y 511 para excluir vertices

    pair<pair<double, double>, double> parAux;
    for(int i = 0; i < cantEmisores/4; i++){//Para cada emisor
    coordenadas.first = 0;//Eje x
    coordenadas.second =  dis(gen);//Eje y. Numero aleatorio entre 1 y 510
        for(int j = 0; j < cantRayosPorEmisor; j++){//Para cada rayo desde ese emisor
            std::uniform_int_distribution<> disAUX(-90, 90);
            int k = disAUX(gen);
            if(k < 0){
                k = k + 360;
            }
            parAux.first = coordenadas;
            parAux.second = k;
            resultado.push_back(parAux);

        }
    }

    for(int i = cantEmisores/4; i < cantEmisores/2; i++){//Para cada emisor
    coordenadas.first = dis(gen);//Eje x
    coordenadas.second =  0;//Eje y. Numero aleatorio entre 1 y 510
        for(int j = 0; j < cantRayosPorEmisor; j++){//Para cada rayo desde ese emisor
            std::uniform_int_distribution<> disAUX(0, 180);
            int k = disAUX(gen);
            parAux.first = coordenadas;
            parAux.second = k;
            resultado.push_back(parAux);

        }
    }

    //Acá vamos con el lado superior
    for(int i = cantEmisores/2; i < 3*(cantEmisores/4); i++){//Para cada emisor
    coordenadas.first = dis(gen);
    coordenadas.second =  tam-1;//El ultimo lugar de la matriz en el eje y
        for(int j = 0; j < cantRayosPorEmisor; j++){//Para cada rayo desde ese emisor
            std::uniform_int_distribution<> disAUX(180, 360);
            int k = disAUX(gen);
            parAux.first = coordenadas;
            parAux.second = k;
            resultado.push_back(parAux);

        }
    }

    //Y por último el lado derecho
    for(int i = 3*(cantEmisores/4); i < cantEmisores; i++){//Para cada emisor
    coordenadas.first =  tam-1;//El último lugar de la matriz en eje x
    coordenadas.second = dis(gen);
        for(int j = 0; j < cantRayosPorEmisor; j++){//Para cada rayo desde ese emisor
            std::uniform_int_distribution<> disAUX(90, 270);
            int k = disAUX(gen);
            parAux.first = coordenadas;
            parAux.second = k;
            resultado.push_back(parAux);

        }
    }


    return resultado;
}




void printVector(Vector v)
{
  int filas = v.size();
  //imprimir vector
    for (int i=0;i<filas;i++){

        std::cout << v[i] << " ";

      std::cout << "\n";
    }

}


pair<pair<double, double>, double> generarUnRayo(double x, double y, double k)
{
    pair<pair<double, double>, double> resultado;
    pair<double, double> coordenadas;
    
    pair<pair<double, double>, double> parAux;
    
    coordenadas.first = x;//Eje x
    coordenadas.second = y;//Eje y. Numero aleatorio entre 1 y 510
    resultado.first = coordenadas;
    resultado.second = k;

    return resultado;
}



void grabarRayos(std::string nombre_archivo, int n, info rayos)
{
    size_t cantRayos = rayos.size();

    std::ofstream output(nombre_archivo += "_rayos.csv" );
    
    // escribir de corrido los unsigned char que representan los valores de intensidad de c/pixel de la imagen
    for (int i=0; i<cantRayos; ++i)
    {
        //output << (n-1)-rayos[i].first.second; // fila del pixel
        output << rayos[i].first.second << ","; // fila del pixel
        output << rayos[i].first.first << ","; // columna del pixel
        output << rayos[i].second << "\n"; // angulo
    }
    output.close();
    return;
}

