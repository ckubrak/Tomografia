#ifndef _RUIDO_H_
#define _RUIDO_H_

#include "sparse.h"
#include "misc.h"

void agregarRuidoImagen(DOK&,float alpha);
void agregarRuidoRayo(std::vector<double> &vectorRayo,float alpha);
void agregarRuidoRayoAditivo(std::vector<double> &vectorRayo,float alpha);

#endif
