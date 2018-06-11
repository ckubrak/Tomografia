#ifndef _IMAGEN_H_
#define _IMAGEN_H_

#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>       /* sqrt */
#include <string>
#include <iostream>
#include <fstream>

#include "../ppmloader/ppmloader.h"

typedef unsigned char uchar;


class Imagen
{

    public:
        Imagen(std::string archivo, int id);
        Imagen();
        Imagen& operator=(const Imagen& viejo);
        Imagen (const Imagen &viejo);
        ~Imagen();

        uchar* getData();
        int getName();
        int size();
        int getId();
        int getHeight();
        int getWidth();
        double restarYnorma (const Imagen& otra);
    private:
        uchar* _data;
        int _name;
        int _width;
        int _height;
        int _maxVal;
        int _id;//sujeto
};

std::vector<Imagen> cargarBD(std::string archivo);


#endif
