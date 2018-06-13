#include <iostream>
#include "ppmloader.h"

using namespace std;

typedef unsigned char uchar;

// Ejemplo de como  acceder a los pixeles de una imagen RGB
unsigned int get_pixel_average(uchar* data, int i, int j, int height, int width){
  if(i > height)
    throw std::runtime_error("El direccionamiento vertical no puede ser mayor a la altura.");
  if(j > width)
    throw std::runtime_error("El direccionamiento horizontal no puede ser mayor al ancho.");
  unsigned int red = (unsigned int)(data[i*width*3 + j*3 + 0]);
  unsigned int green = (unsigned int)(data[i*width*3 + j*3 + 1]);
  unsigned int blue = (unsigned int)(data[i*width*3 + j*3 + 2]);
  return (unsigned int)((red+green+blue) / 3);
}

void read_image(std::string filename, uchar** data, int* width, int* height){
  *data = NULL;
  *width = 0;
  *height = 0;
  PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;

  bool ret = LoadPPMFile(data, width, height, &pt, filename.c_str());
  if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_RGB_8B){
    throw std::runtime_error("Fallo al leer la imagen.");
  }
}

void test_image(){
  uchar* data = NULL;
  int width = 0, height = 0;
  std::string filename = "prueba.ppm";
  read_image(filename, &data, &width, &height); // Ejemplo de llamada

  for (int h = 0; h < height; ++h){
    for (int w = 0; w < width; ++w){
      cout << get_pixel_average(data, h, w, height, width) << " "; // Ejemplo de lectura de un pixel
    }
    cout << endl;
  }
  delete [] data;
}

void test_load(){

  uchar* data = NULL;
  int width = 0, height = 0;
  PPM_LOADER_PIXEL_TYPE pt = PPM_LOADER_PIXEL_TYPE_INVALID;
//  std::string filename = "buda.0.ppm";
  std::string filename = "prueba.ppm";

  bool ret = LoadPPMFile(&data, &width, &height, &pt, filename.c_str());
  if (!ret || width == 0|| height == 0|| pt!=PPM_LOADER_PIXEL_TYPE_RGB_8B)
  {
    throw std::runtime_error("test_load failed");
  }

  delete [] data;
}

void test_save(){

  char comments[100];
  sprintf(comments, "%s", "Hello world");

  int width = 3, height =1;
  uchar* data = new uchar[width*height*3];
  data[0] = data[1] = data[2] = 100; // RGB
  data[3] = data[4] = data[5] = 150; // RGB
  data[6] = data[7] = data[8] = 245; // RGB
  std::string filename = "prueba.ppm";

  bool ret = SavePPMFile(filename.c_str(),data,width,height,PPM_LOADER_PIXEL_TYPE_RGB_8B, comments);
  if (!ret)
  {
    std::cout << "ERROR: couldn't save Image to ppm file" << std::endl;
  }
}

int main() {

  test_load();
  test_save();
  test_image();

  return 0;
}
