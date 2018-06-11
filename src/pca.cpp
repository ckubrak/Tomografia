#include "imagen.h"
#include "pca.h"
#include <cmath>

void printVector(doubleVector v)
{
  int filas = v.size();
  //imprimir vector
    for (int i=0;i<filas;i++){

        std::cout << v[i] << " ";

      std::cout << "\n";
    }

}

void printMatrix(doubleMatrix M)
{
  int filas, columnas;
  filas = M.size();
  columnas = M[0].size();
  for (int i=0;i<filas;i++){
    for (int j=0;j<columnas;j++){
      std::cout << M[i][j] << " ";
    }
    std::cout << "\n";
  }
}

double norma2Vectorial(doubleVector v)
{
  double acumulador=0.0;

  int n = v.size();
  for (int i=0; i<n; ++i)
  {
      acumulador+=pow(v[i],2);
  }
  return (sqrt(acumulador));
}

double normaInfinitoVectorial(doubleVector v, int &indice)
{
  double acumulador=0.0;
  indice = 0;
  int n = v.size();

  for (int i=0; i<n; ++i)
  {
      if (fabs(v[i])>acumulador){
        acumulador=fabs(v[i]);
        indice=i;
      }
  }
  return acumulador;
}

double normaInfinitoMatricial(doubleMatrix x, int &indice)
{
  double temp=0.0;
  double norma=0.0;
  indice = 0;
  int n = x.size();
  int m;
  if (n>0) m=x[0].size();

  for (int i=0; i<n; ++i)
  {
    for (int j=0; j<m; ++j){
      temp += fabs(x[i][j]);
    }
      if (temp>norma){
        norma=temp;
        indice=i;
      }
      temp = 0.0;
  }
  return norma;
}




// matriz y vectores no vacios
doubleVector matrizXVector(doubleMatrix A, doubleVector v)
{
  int n=A.size();
  doubleVector y(n);
  double acumulador = 0.0;
  int m;

  if (n>0)
  {
    m = A[0].size();
  }
  for (int i = 0 ; i != n; ++i)
  {
      for (int j=0 ; j != m; ++j)
      {
        acumulador += A[i][j]*v[j];
      }
      y[i]=acumulador;
      acumulador=0.0;
  }
  return y;
}

////// matriz  por matriz: asumimos que las dimensiones son validas  //////////
doubleMatrix matrizXMatriz(doubleMatrix A, doubleMatrix B)
{
  int n=A.size(); //filas de A
  int m=B.size(); //filas de B, columnas de A
  int r; //columnas de B

  if (A.size()>0)
    m = B.size();

  if (m>0)
    r=B[0].size();

  doubleMatrix R(n, doubleVector(r));
  double acumulador = 0.0;
  //calcular R[i][j]
  for (int i = 0 ; i != n; ++i)
  {
    for (int j=0 ; j != r; ++j)
    {
      acumulador=0.0;
      for (int k=0; k<m; ++k)
      {
            acumulador += A[i][k]*B[k][j];
      }
      R[i][j]=acumulador;
    }
  }
  return R;
}

doubleMatrix vvt(doubleVector v)
{
  int n=v.size(); //fstd::vlas de A
  doubleMatrix R (n, doubleVector(n));
  double acumulador = 0.0;

  //calcular R[i][j]
  for (int i = 0 ; i != n; ++i)
  {
    for (int j=0 ; j != n; ++j)
    {
      acumulador = 0.0;
      acumulador += v[i]*v[j];
      R[i][j]=acumulador;
    }
  }
  return R;
}

doubleMatrix matrizXEscalar(doubleMatrix A, double esc)
{
  int m;

  int n = A.size();
  if (A.size() > 0) m=A[0].size();

  for (int i = 0; i != n; ++i)
  {
      for (int j=0; j != m; ++j)
      {
        A[i][j] *= esc;
      }
  }  return A;
}

doubleVector vectorXEscalar(doubleVector v, double esc)
{

      for (doubleVector::iterator j = v.begin() ; j != v.end(); ++j)
      {
        *j *= esc;
      }

  return v;
}


doubleVector sumaVectorial(doubleVector a, doubleVector b)
{
  int s=a.size();
  for (int i=0; i<s; ++i)
  {
    a[i] += b[i];
  }
  return a;
}

doubleVector restaVectorial(doubleVector a, doubleVector b)
{
  return sumaVectorial(a,vectorXEscalar(b, -1.0));
}


doubleMatrix sumaMatricial(doubleMatrix a, doubleMatrix b)
{
  int s=a.size();
  int t=a[0].size();
  for (int i=0; i<s; ++i)
  {
    for (int j=0; j<t; ++j)
      a[i][j] += b[i][j];
  }
  return a;
}

doubleMatrix restaMatricial(doubleMatrix a, doubleMatrix b)
{
  return sumaMatricial(a,matrizXEscalar(b, -1.0));
}


doubleVector ucharToDoubleVector (uchar* data, int longitud)
{
  doubleVector local(longitud);

  for (int i=0; i<longitud; i++){
        local[i] = data[i] + 0.0;
  }
  return local;
}


int MetodoPotencias(doubleMatrix A, doubleVector x,int nroIter,float tol, double &autoval, doubleVector &autovec)
{

  double normax, normay, lambda;//, auxlambda;
  int sz=x.size();
  double r=1.0;

  //normalizo el vector x
  normax = norma2Vectorial(x);
  lambda = normax;
  autovec = x;

  x = vectorXEscalar(x, 1.0/normax);//normalizamos x

  doubleVector y(sz); // guarda el valor de la ultima iteracion
  int k=0;

  while (k<nroIter && r > tol || (k>=nroIter && k<nroIter*10 && r>0.001))
  {

    y = matrizXVector(A, x);
    normay = norma2Vectorial(y);

    if (normay < 0.0000001)  // considero que la norma es cero, no puedo dividir por ella
    {
        lambda = 0;
        break;  //no hubo convergencia
    }
    y = vectorXEscalar(y, 1/normay);

    r = fabs(normax-normay); // variacion del autovalor en dos iteraciones sucesivas

    x = y;
    normax = normay;

    k++;
  }

  // analizar si hubo convergencia. Si no la hubo habria que intentar con otro x inicial
  if (r > tol)
    return 0; //no hubo convergencia
  else
  {
    autovec = x;
    autoval = normax;
    return 1; // hubo convergencia
  }
}





//Entrada: matriz, cantidad de autovalores significativos.
//Salida: vector con autovalores, matriz de autovectores (uno por fila)
// los autovectores se almacenan por fila (ojo!!! No por columna!!!)
int Deflacion(doubleMatrix A, int alfa, doubleVector &autovalores, doubleMatrix &autovectores)
{
  long dimension=A[0].size();
  doubleVector x(dimension);
  double tol=0.00001;
  int nroIter=10000;
  double lambda;

  doubleVector autovec(dimension);
  doubleMatrix auxA(dimension, doubleVector(dimension));

  //averiguo autoval ppal y vector asociado
  //ir almacenando los autovalores y autovectores que se van hallando
  //repetir para A-lambda*v1*v1t

  // armar vector x inicial en funcion de la dimension de la matriz A
  // analizar opciones de como armarlo.

  srand (time(NULL));
  for (int i=0; i<dimension; ++i)
    x[i]=rand() % dimension + 1;

  for (int d=0; d<dimension && d<alfa; ++d)
  {

    if (MetodoPotencias(A, x, nroIter, tol, lambda, autovec) == 1) //hubo convergencia
    {
      //Almacenar valores obtenidos para el autovalor y autovector ppal
      autovalores[d]=lambda;
      autovectores[d]=autovec;
      if (d<dimension-1){
        auxA = vvt(autovec);
        auxA = matrizXEscalar(auxA, lambda);
        A=restaMatricial(A, auxA);
      }
    }
    else
    {
      //fallo el metodo de la potencia
       std::cout << "no hubo convergencia metodo de las potencias\n";
       std::cout << "d: " << d << " " << autovalores[d];
    }
  }
}

// se calcula X*Xt. Por ahora no lo uso: se obtiene una matriz de n x n (n=cantidad de imagenes)
void CalcularCovarianza(doubleMatrix X, int filas, int columnas, doubleMatrix &mcov){
    //Xt*X
    double acumulador=0;


    for (int i=0; i<filas; i++){
      for (int j=0; j<filas; j++){
        acumulador=0.0;
        for (int k=0; k<columnas; k++)
          acumulador+=X[i][k]*X[j][k];
        mcov[i][j] = acumulador;
      }
    }
}



//Arma en el parametro de Salida X una matriz con las imagenes normalizadas
void ArmarMatrizX(baseDeDatos muestra, doubleVector media, int filas, int columnas, doubleMatrix &X )
{
  double den;

  den = sqrt(filas-1);
  for (int i=0; i<filas; i++){
    for (int j=0; j<columnas; j++){
        X[i][j] = (muestra[i].getData()[j] - media[j]) / den ;
    }
  }
}


// resultado: alfa * columnas
doubleMatrix matrizTraspuesta (doubleMatrix A)
{
  int filas, columnas;

  filas = A.size();
  if (filas > 0) columnas = A[0].size();

  doubleMatrix B (columnas, doubleVector(filas));

  for (int i=0; i<filas; ++i){
    for (int j=0; j<columnas; ++j){
      B[j][i]=A[i][j];
    }
  }
  return B;
}




// Recibe como parametro la base de datos con todas las imagenes
// parametro de salida una matriz con la transformacion caracteristica de cada imagen
// columnas = cantidad de bytes por imagenes. Se puede calcular a partir de una de las imagenes cualquiera
// multiplicando ancho por alto.
// cambioDeBase: salida, tc muestra: salida, media: salida,
void PCA (baseDeDatos muestra, doubleMatrix &cambioDeBaseTras, doubleVector &media, doubleMatrix &matrizCaracteristicaMuestra, int alfa){

  int filas, columnas;

  filas = muestra.size();

  columnas=muestra[0].getWidth()*muestra[0].getHeight(); // cantidad de pixeles de la imagen. Asumiendo que son todas de la misma dimension uso lo de la primera

  media.assign (columnas, 0.0);

  //calcular la media de cada columnas
  for (int j=0;j<columnas;j++){
    for (int i=0;i<filas;i++){
      media[j] += (double) muestra[i].getData()[j];
    }
  }

  for (int j=0;j<columnas;j++){
    media[j]=media[j]/filas;
  }

  doubleMatrix X(filas, doubleVector(columnas));
//  std::cout << "antes de armar matriz\n";
  ArmarMatrizX(muestra, media, filas, columnas, X);

//Calcular Xt*X = Matriz de covarianza. Por tamaños, se calcula X * Xt y despues se recalculan los valores singulares de la otra
  doubleMatrix mcov(filas, doubleVector(filas));

  CalcularCovarianza (X, filas, columnas, mcov);

  // LLamar al metodo de deflacion para calcular los autovalores y autovectores
  // usar el parametro alfa para ver cuantos hay que averiguar

  doubleVector autovalores(alfa);
  doubleMatrix autovectoresTras(alfa, doubleVector(mcov.size()) ); // es la V traspuesta, tiene un autovector x fila
  int r= Deflacion(mcov, alfa, autovalores, autovectoresTras);

  // Convertir los autovalores y autovectores calculados en los correspondientes de la matriz Xt * X
  //Ui=Xt*autovectores(i)/sqrt(autovalores(i))
  //Primero calcular los autovalores
  for (int i=0;i<alfa;i++)
  {
    autovalores[i]=sqrt(autovalores[i]);
  }

  //Usando los autovalores calculados recalcular los autovectores de Xt * X (que es la verdadera matriz covarianza)
  //for (int i=0;i<alfa;i++){
    // Hay que multiplicar Xt * autovectores[i] / autovalores[i]
    // matrizXVector (Tras(X),Tras(autovectores)) //nuestra implementacion tiene un autovector por fila, por eso hay que trasponer
    // dimensiones: autovectores(alfa * filas), X(filas * columnas)
    // filas=cantidad de imagenes, columnas= cantidad de variables originales
    // resultado: alfa * columnas. Traspuesto: columnas * alfa

    //cambioDeBase = matrizTraspuesta (matrizXMatriz (autovectores, X));
    // dimension cambioDeBase: alfa * filas (cantidad de imagenes)
    for (int i=0; i<alfa; ++i)
      autovectoresTras[i] = vectorXEscalar (autovectoresTras[i],(1/autovalores[i])); // dividir el autovector i por el autovalor correspondiente

    for (int i=0; i<alfa; ++i)
      cambioDeBaseTras[i].assign(columnas, 0.0);
//    cambioDeBaseTras = matrizXMatriz (autovectoresTras, X); // un autovector x fila
      for (int i=0; i<alfa; ++i)
      {
        cambioDeBaseTras[i]=matrizXVector(matrizTraspuesta(X), autovectoresTras[i]);
        // multiplicar la matriz Xt por el i-esimo autovector

      }

    // TC: traspuesta (Ut * Xt) = X * U  (ycomo nuestra U ya esta traspuesta hay que trasponerla)
    for (int i=0; i<filas; ++i)
      matrizCaracteristicaMuestra[i].assign(alfa, 0.0);
    matrizCaracteristicaMuestra = matrizXMatriz(X, matrizTraspuesta (cambioDeBaseTras));

}

// Aplicar la TC a la imagen a reconocer. Parametros:
// Entrada: imagen, matrizCaracteristica de la Muestra, media de la muestra, matriz de cambio de base traspuesta
// Salida: imagen preprocesada
// valores calculables: catidad de imagenes de la muestra
doubleVector normalizarImagen (doubleVector img, doubleVector media, doubleMatrix cambioDeBaseTras, doubleMatrix matrizCaracteristicaMuestra)
{
  // TODO
  doubleVector imgNormalizada(cambioDeBaseTras.size()); // retenemos las alfa componentes ppales de la imagen

  // restar la media
  img = restaVectorial (img, media);

  // dividir por la raiz de n-1
  double den = sqrt(matrizCaracteristicaMuestra.size() - 1);
  img =  vectorXEscalar (img, 1/den );

  // multiplicar por la matriz de cambio de base matrizTraspuesta
  imgNormalizada = matrizXVector (cambioDeBaseTras, img);

  return imgNormalizada;
}
