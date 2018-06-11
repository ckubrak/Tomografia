#include "sparse.h"
DOK::DOK(size_t n)
{
    _n = n;
    _m = 0;
}

DOK::DOK(const char* input)
{

    int from,to;
    std::ifstream infile(input);
    std::string line;
    infile >> _n;
    infile >> _m;

    for( std::string line; std::getline( infile, line ); )

    {
        infile >> from >> to;
        _mat[to-1][from-1] = 1;
        _m++;
    }

}

DOK::DOK(size_t n, double val)
{
    _n = n;
    for (int i = 0; i < _n; i++)
    {
        _mat[i][i] = val;
        _m ++;
    }
}

DOK DOK::multiplicarMatriz(DOK &m)
{
    DOK C(this->_n);
    for (size_t i=0; i < this->size(); ++i)
            for (size_t j=0; j < this->size(); ++j)
            {
                if(_mat[i].count(j))
                    for (size_t k=0; k < this->size(); ++k)
                    {
                        if (m._mat[k].count(j) > 0)
                        {
                            if (C._mat[j].count(j) > 0)
                                C._mat[i][j] += this->_mat[i][k] * m._mat[k][j];
                            else
                                C._mat[i][j] = this->_mat[i][k] * m._mat[k][j];
                        }
                    }
            }
    return C;
}
void DOK::restarMatrices(DOK& m)
{
    m.multiplicarConstante(-1.0);
    sumarMatrices(m);
}
void DOK::sumarMatrices(DOK& m)
{
    for (int i = 0; i < _n; i++)
    {
        for (int j = 0; j < _n; j++)
        {
            if (_mat[i].count(j) > 0)
            {
                if(m._mat[i].count(j) > 0)
                {
                    _mat[i][j] += m._mat[i][j];
                }
            }
            else
            {
                if( m._mat[i].count(j) > 0)
                {
                    _mat[i][j] = m._mat[i][j];
                }
            }
        }
    }
}


void DOK::multiplicarConstante(double c)
{

    for (int i = 0; i <size(); i++)
    {
            for (int j = 0; j < size(); j++)
            {
                if (_mat[i].count(j) > 0){
                    _mat[i][j] = c * _mat[i][j];
                }

            }
    }
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
    for (int i=0; i < n; i++) //agregar b en la ultima columna
    {
        if (fabs(b[i]) > eps)
        {
            _mat[i][b.size()] = b[i];
        }
    }
    for (int k = 0; k < n-1 ; k++) //filas pivote
    {
        // controlar que el pivote no sea cero
        // por seguridad: la estructura de la matriz nos garantiza que no va a pasar

        for (int i = k+1 ; i < n ; i++) //filas a eliminar
        {
            // verificar que en la primera columna de la fila a eliminar no haya cero
            // si ya hubiera cero pasar a eliminar la siguiente fila
            if (_mat[i].count(k) != 0)
            {
                mult = _mat[i][k] / _mat[k][k]; //calcular multiplicador

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
            } //fin no hay cero en la primera columna de la fila i
        } // fin filas a eliminar
    } // fin filas pivote

    return resolverSistema();
}

Vector DOK::resolverSistema()
{

    int n = n-2;
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
    std::vector<double> ceros(_n,0.0);
    // Creo una matriz nula
    for (int i = 0; i < _n; i++)
    {
        res[i] = ceros;
    }


    for (int i = 0; i < _n; i++)
    {
        for (int j = 0; j < _n; j++)
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
    std::vector<Vector> vectorDeVectores = dok.matrizCompleta();
    for (int i = 0; i < vectorDeVectores[0].size(); i++)
    {
        for (int j = 0; j < vectorDeVectores[0].size(); j++)
        {
            std::cout << vectorDeVectores[i][j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

//Armar la matriz diagonal D, "balance"
DOK::DOK(DOK& m)
{
    _n = m._n;
    for (int j = 0; j < _n; j++)
    {
        int cj = Cj(j,m);

        if(cj != 0)
        {
            _mat[j][j] = (1.0 / double(cj));
        }
    }
}

int DOK::Cj(int j, DOK& m){
	int Cj = 0;
	for (int i = 0; i < _n; i++) //i son filas, j es la columna fija
	{
		if(m._mat[i].count(j) > 0)
      Cj += 1;
	}
	return Cj; //Cj son la cantidad de links salientes de la pagina j
}

//Caminante aleatorio:
