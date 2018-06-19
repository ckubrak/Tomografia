#include "misc.h"

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


    max_val =I[0]; min_val = I[0];
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
    return I8;

}


/*
    std::string out = argv[1];
    //TODO cambiar a .out a la hora de entregar
    out += ".outi";
    std::ofstream output(out);
    output << p <<"\n";
    for (int i =0; i< resultado.size();i++)
    {
        output << resultado[i] << "\n";
    }
    output.close();
    return 0;

*/

int grabarPPM8Bits(std::string nombre_archivo, int filas, int columnas, Vector I)
{
    size_t cantPixeles = I.size();

    std::ofstream output(nombre_archivo += ".pgm" );
    output << "P5" << "\n";
    output << filas <<  " " << columnas << "\n";
    output << 255 << "\n";
    // escribir de corrido los unsigned char que representan los valores de la imagen
    for (int i=0; i<cantPixeles; ++i)
    {
        output << (unsigned char) I[i];
    }
    output.close();
    return 0;
}