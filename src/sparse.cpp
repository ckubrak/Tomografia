#include "sparse.h"
// Crea una matriz cuadrada
DOK::DOK(size_t n)
{
    _n = n;
    _c = n;
    _m = 0;
}
DOK::DOK(size_t n, size_t c)
{
    _n = n;
    _c = c;
    _m = 0;
}

Vector DOK::CuadradosMinimos(Vector b)
{
    Vector x(_c - 1, 0.0);
    Vector auxb(_c - 1, 0.0);

    // (At * A) * x = At b
    auxb = matrizTraspuestaXVector(b);
    x = productoAtraspuestaPorA().eliminacionGauss(auxb, EPSILON_EG);
    
    return x;
}


Vector DOK::matrizTraspuestaXVector(Vector &b)
{
    size_t filasA = _n;
    size_t columnasA = _c;
    double acum=0.0; //acumulador de fila x columna

    // At * b tiene dimension columnasA * 1
    Vector Rta(_c);
    for (size_t i=0; i < columnasA; ++i)
    {
        // inicializar el acumulador en cero
        acum = 0.0;
        for (size_t k=0; k < filasA; ++k)
        {
            // si el elemento de At[i][k] no es cero multiplicarlo por el correspondiente de b y acumular el resultado
            // (At[i][k] = A[k][i])
            if ( _mat.count(k) > 0 && _mat[k].count(i) > 0)
            {
                acum += _mat[k][i] * b[k];
            }
        }
        // si el acumulador es mayor a cero agregamos el elemento en la matriz resultado
        Rta[i] = acum;
    }
    return Rta;    
}



// parametros: de entrada: matriz A esparza rectangular
// Salida: A traspuesta * A, esparza cuadrada
DOK DOK::productoAtraspuestaPorA()
{
    size_t filasA = this->_n;
    size_t columnasA = this->_c;
    double acum=0.0; //acumulador de fila x columna

    // At * A tiene dimension columnasA * columnasA
    DOK C(columnasA);
    for (size_t i=0; i < columnasA; ++i)
            for (size_t j=0; j < columnasA; ++j)
            {
                // inicializar el acumulador en cero
                acum = 0.0;
                for (size_t k=0; k < filasA; ++k)
                {
                    // si los elementos de At[i][k] y A[k][j] no son cero multiplicarlos y acumular el resultado
                    // (At[i][k] = A[k][i])
                    if ( (_mat.count(k)>0) && (_mat[k].count(i) > 0) && (_mat[k].count(j) > 0) )
                    {
                        acum += _mat[k][i] * _mat[k][j];
                    }
                }
                // si el acumulador es mayor a cero agregamos el elemento en la matriz resultado
                if (acum != 0)
                {
                    C._mat[i][j] = acum;
                }
            }
    //mostrarMatriz(C);
    return C;
}



Vector DOK::eliminacionGauss(Vector& b, double eps)
{
    double mult, matij, matkj;
    size_t n; //cantidad total de filas de la matriz

    // Se agrega a b en la ultima columna
    // esto aumenta en 1 la dimension de toda la matriz pero en realidad sigue
    // habiendo una fila menos
    n=_n;
    _n++;
    
    //mostrarMatriz(*this);
    for (int i=0; i < n; i++) //agregar b en la ultima columna
    {
        if (fabs(b[i]) > eps)
        {
            _mat[i][b.size()] = b[i];
        }
    }
    for (int k = 0; k < n-1 ; k++) //filas pivote
    {
        // TP3: Pivoteo parcial
        if (_mat.count(k) == 0 ||_mat[k].count(k) == 0)
        {
            // std::cout << "hay que pivotear: \n";
            // mostrarMatriz(*this);
            // recorrer las filas de abajo buscando un pivote y permutar
            for (int i = k+1 ; i < n ; i++) //filas a eliminar
            {
                if (_mat[i].count(k) != 0) //encontramos nuevo pivote.
                {
                    // Permutar: auxiliar=k,  k=i, i=auxiliar.
                    double auxiliar = 0.0;
                    for (int c = 0 ; c <= n ; ++c)  // OJO c <= n para incluir el termino independiente
                    {
                        if (_mat[k].count(c) != 0)
                        {
                            auxiliar = _mat[k][c];
                            _mat[k].erase(c);
                        }
                        if (_mat[i].count(c) != 0)
                        {
                            _mat[k][c]=_mat[i][c];
                            _mat[i].erase(c);
                        }
                        if (auxiliar != 0.0)
                        {
                            _mat[i][c] = auxiliar;
                            auxiliar = 0.0;
                        }                            
                    }
                    // std::cout << "k, i: " << k << " " << i << "\n";
                    // mostrarMatriz(*this);
                    break;
                }
                // else{
                //     //falla Gauss
                // }
            }
            // chequear si se encontro el pivote
        }
        if (_mat.count(k) == 0 ||_mat[k].count(k) == 0){
            std::cout << "fallo gauss\n" ;
            //mostrarMatriz(*this);    
            int z=0; // fallo gauss?
        }


        for (int i = k+1 ; i < n ; i++) //filas a eliminar
        {
            // verificar que en la primera columna de la fila a eliminar no haya cero
            // si ya hubiera cero pasar a eliminar la siguiente fila
            if (_mat[i].count(k) != 0)
            {
                mult = _mat[i][k] / _mat[k][k]; //calcular multiplicador
                //std::cout << "multiplicador, i, k: " << mult << " " << i << " " << k << "\n";

                for (iter_col col = _mat[k].begin(); col != _mat[k].end(); col++)
                {
                    int j = (*col).first;
                    if (j < k)
                        continue;
                    if (_mat[k].count(j) != 0)
                    {
                        matkj=_mat[k][j];
                        if (_mat[i].count(j) == 0)
                        {
                            matij=0;
                        }
                        else
                        {
                            matij=_mat[i][j];
                        }

                        double ij = matij - mult * matkj;
                        if (fabs(ij) < eps)
                        {
                            _mat[i].erase(j);
                        }
                        else
                        {
                            _mat[i][j] = ij;
                        }
                    }
                }
                // std::cout << "resultado multiplicar fila: \n";
                // mostrarMatriz(*this);
            } //fin no hay cero en la primera columna de la fila i
        } // fin filas a eliminar
        // std::cout << "paso k:" << k << "\n";
        // mostrarMatriz(*this);
    } // fin filas pivote
    // std::cout << "fin eliminacion gaussiana: \n"; 
    // mostrarMatriz(*this);
    return resolverSistema();
}

Vector DOK::resolverSistema()
{
    //mostrarMatriz(*this); //int n = n-2;
    Vector x(_n-1,0.0);
    for (int i = _n-2; i >= 0; i--)
    {
        x[i] = _mat[i][_n-1];

        for(int j = i+1; j <= _n-2; j++)
        {
            x[i] = x[i] - _mat[i][j]*x[j];
        }
        x[i] = x[i]/_mat[i][i];
    }
    return x;
}


// Testing only
// TODO borrar antes de entregar matrizCompleta
std::vector<Vector> DOK::matrizCompleta()
{
    std::vector<Vector> res(_n);
    std::vector<double> ceros(_c,0.0);
    // Creo una matriz nula
    for (int i = 0; i < _n; i++)
    {
        res[i] = ceros;
    }


    for (int i = 0; i < _n; i++)
    {
        for (int j = 0; j < _c; j++)
        {
            if (_mat[i].count(j) > 0)
                res[i][j] += _mat[i][j];
        }
    }

    return res;
}

// TODO borrar antes de entregar mostrarMatriz
void mostrarMatriz(DOK& dok)
{
    std::ofstream output("inputmatriks.txt"); 
    std::vector<Vector> vectorDeVectores = dok.matrizCompleta();
    for (int i = 0; i < vectorDeVectores[0].size(); i++)
    {
        for (int j = 0; j < vectorDeVectores[0].size(); j++)
        {
            output << vectorDeVectores[i][j] << "\t";
            // std::cout << vectorDeVectores[i][j] << "\t";

        }
        output << "\n";
        // std::cout << "\n";
    }
    // std::cout << "\n";
}

// carga en una matriz esparza una imagen cuadrada de nxn pixeles desde un archivo csv
void DOK::cargarCsv(std::string csv, int n)
{
    std::ifstream infile(csv);
    std::string line;
    int temp;
    char coma;
    _m = 0;
    _n = n;
    _c = n; // se asume que los datos a cargar corresponden a una matriz cuadrada
    for (int i = 0; i < n; ++i)
    {
            int contador = 0;
            for (int j = 0; j < n; ++j)
            {
              
                infile >> temp;

                ++ contador; //contar los caracteres leido
                if (contador < _c)
                    infile >> coma;
                if (temp > 0)
                {
                    _mat[i][j] = temp;
                    _m++;
                }
            }
    }
}

void DOK::distancia(int n, int m, int a, int b, info rayos)
//importante que matrix debe ser una matriz de las dimensiones adecuadas con todos los valores incializados en 0. crear con vector<vector<int>> salida(rayos.size(), vector<int>(n/a*m/b));
{
    //n es cantidad de filas de pixeles en imagen total
    //m es cantidad de columnas de pixeles en imagen total
    //a es cantidad de filas de pixeles dentro de una supercelda
    //b es cantidad de columnas de pixeles dentro de una supercelda
    //rayos guarda corrdenas de origen y angulo
    //salida guarda para cada rayo por cuantos pixeles pasa en cada supercelda
    for (int i=0;i<rayos.size();i++)
    {
        std::vector<std::pair<int, int>> solucion;

        pasa(n, m, rayos[i].first.first, rayos[i].first.second, rayos[i].second, solucion);
        for (int j=0;j<solucion.size();j++)
        {
            int c=solucion[j].first; //numero de columna
            int d=n-1-(solucion[j].second); //numero de fila
            int auxiliar=(d/a)*(m/b)+c/b; // d/a seria numero de fila de superdeldas . m/b seria cantidad de columnas de superceldas. c/b seria numero de columna de superceldas
            if (_mat.count(i) == 0 || _mat[i].count(auxiliar) == 0)
            {
                _mat[i][auxiliar] = 1;
                //int asd = _mat[i][auxiliar];
                //bool a = true;

            }
            else
                _mat[i][auxiliar]++ ;
        }
    }

}