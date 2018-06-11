#include "imagen.h"
#include "pca.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include<libgen.h> 
#include<stdlib.h> 

baseDeDatos cargarBD(std::string csv)
{
    baseDeDatos base = {};
    std::ifstream input(csv);

    std::string imagen;
    std::string line;
    int id;


    while (std::getline(input, line))
    {

      std::size_t found = line.find_first_of(", ");      //primer delimitador
        imagen = line.substr(0, found);
        line = line.substr(found, line.size()-1);

        std::size_t inicio = line.find_first_not_of(", "); //inicio de datos significativos
          line = line.substr(inicio, line.size()-1);
          id = std::stoi (line);
          Imagen nuevaImagen(imagen,id);

          base.push_back(Imagen(imagen,id));
    }
    return base;
}

double Imagen::restarYnorma (const Imagen& otra)
{
    Imagen resta;
    for (int i=0; i < _width * _height; i++)
    {
        resta._data[i] =  _data[i] - otra._data[i];
    }

    double res = 0;

    for (int j = 0; j < _width * _height; j++)
    {
        res += resta._data[j];
    }
    return sqrt(double(res));
}


Imagen::Imagen(std::string archivo, int id)
{
    uchar* data = NULL;
    int width = 0, height = 0;

    std::string nombre;

    std::size_t s = archivo.find_last_of(".");
    std::size_t d = archivo.find_last_of("/");
    nombre = archivo.substr(d+1, s);
    std::size_t s2 = nombre.find_first_of(".");
    std::string nombre1 = nombre.substr(0, s2);
    std::stringstream(nombre1) >> _name; 
    PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
    bool ret = LoadPPMFile(&_data, &_width, &_height, &pt, archivo.c_str());
    if (!ret || _width == 0|| _height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_GRAY_8B )
    {
        throw std::runtime_error("test_load failed");
    }
    std::vector<uchar> resultado;
    _id = id;
}

Imagen::Imagen()
{
    _data = {};
    _width = 0;
    _height = 0;
    _maxVal = 0;
    _id = -1;
    _name = 0;
}

Imagen::~Imagen()
{
    delete _data;
}

uchar* Imagen::getData()
{
    return _data;
}

int Imagen::getHeight()
{
    return _height;
}


int Imagen::getWidth()
{
    return _width;
}

int Imagen::getId()
{
    return _id;
}


int Imagen::getName(){
  return _name;
}

Imagen::Imagen (const Imagen &viejo)
{
    int tam = viejo._width * viejo._height;
    _data = new uchar[tam];
    for (int i=0; i<tam; ++i)
    {
      _data[i] = viejo._data[i];
    }
    _width = viejo._width;
    _height = viejo._height;
    _maxVal = viejo._maxVal;
    _id = viejo._id;
    _name = viejo._name;
}

Imagen& Imagen::operator=(const Imagen& viejo)
{

  int tam =_width * _height;

    uchar* localdata = new uchar(tam);
    for (int i=0; i<tam; ++i)
    {
      localdata[i] = viejo._data[i];
    }
    delete[] _data;
    _data = localdata;

    _width = viejo._width;
    _height = viejo._height;
    _maxVal = viejo._maxVal;
    _id = viejo._id;
    _name = viejo._name;
    return *this;
}
