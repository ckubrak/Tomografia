%% Levanto la imagen

%archivo conteniendo la info de rayos en formato csv: columna, fila, angulo
%las columnas se numeran de izq a der, las filas de abajo hacia arriba
% los angulos crecientes en sentido antihorario. (el origen de coordenadas
% se asume en la esquina inferior izquierda de la imagen)
arch = '/home/marcela/CsComputacion/metnum/labo/tp3/Recursos/tomo25x25px.txt_rayos.csv';
archimagen = '/home/marcela/CsComputacion/metnum/labo/tp3/Recursos/conversor_csv/imagenes_convertidas/tomo25x25px.csv';

rayos = csvread (arch);
I = csvread(archimagen);

dibujar = 1;

simularRayo_tp3(I, rayos, dibujar);


