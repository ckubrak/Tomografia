
Parámetros de entrada: Para correr el programa se deben utilizar los siguientes parámetros:
* Nombre del archivo de entrada sin extensión. Se asume que el archivo es de tipo csv conteniendo imágenes de 16 bits.
* Nivel de ruido: valor entre 0 y 1.
* Dimension de la imagen: cantidad de pixeles por fila. Asumimos que la imagen es cuadrada.
* Dimensión de celda. Número entero que debe s.er divisor de la cantidad de pixeles por fila de la imagen. Se asume que las celdas van a ser cuadradas.
* Cantidad de Emisores. Cantidad total de emisores.
* Cantidad de rayos. Cantidad de rayos que se van a trazar desde cada emisor.


Ejemplo: $./tp3 ./imgs_TC/tomo 0.1 100 5 20 120$
Con estos parámetros se toma el archivo tomo.csv, nivel de ruido 0.1, dimension de la imagen 100x100 pixeles, dimensión de la celda 5x5 pixeles, 20 emisores, 120 rayos por emisor.

En el mismo directorio en que se encuentran las imágenes de entrada se genera el archivo que contiene la imagen reconstruida en format pgm de 8 bits. Tiene el mismo nombre que el archivo de entrada y extensión '.pgm'.