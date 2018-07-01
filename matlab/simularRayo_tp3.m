function simularRayo_tp3(I, rayos, dibujar)
%SIMULARRAYO_tp3 Simula la emision todos los rayos sobre una imagen particular tomografico sobre una imagen.
    % en base a datos recolectados durante la ejecucion
    % Simula un rayo del tomagrafo a traves de la imagen I.
    % El parametro dibujar es opcional y si vale 1 se dibuja el rayo sobre
    % la imagen.
    
    y = 0;
    [f, c] = size(I);
    
    figure;
    imagesc(I), colormap(gray), axis image;
    xlabel('columnas (px)');ylabel('filas (px)')
    title('(Tomo.png) Rayos sobre discretizacion de 5 x 5 px');
    hold on;
           
    cantRayos = size(rayos);
    for i = 1:cantRayos(1)
    
        %recorrer los rayos y graficarlos
        %calcular los puntos extremos de cada rayo y dibujar la recta
        xinicio = rayos(i, 1);%+1;
        yinicio = f-rayos(i, 2); %sin -1
        if yinicio==1
            yinicio=0;
        
        end

        if yinicio==f
            yinicio=f+1;
        end
        if xinicio==c-1
            xinicio=c+1;
        end
        

        alfa = rayos (i, 3);
        talfa = tan(alfa);
        
        plot(xinicio, yinicio, '*b');
        %el rayo NO se origina en el margen derecho
        if xinicio < c
            for j = xinicio: c+1 %c
                xfinal=j;
                y = talfa * j + yinicio;
                if y>f 
                    yfinal = f+1;%25
                    break;
                end
                if y<0 
                    yfinal=0; %1
                    break;
                end
                yfinal=y;
            end
        else
           for j = xinicio: 0 %1
               xfinal=j;
                y = talfa * j + yinicio;
                if y>c 
                    yfinal=c+1; %c;
                    break ;
                end
                if y<0 
                    yfinal=0;
                    break;
                end
                yfinal=y;
           end 

        end
        
        if yfinal==f
            yfinal=f+1;
        end
        if xfinal==c
            xfinal=c+1;
        end

        plot([xinicio, j], [yinicio, yfinal], 'Color', 'r', 'LineStyle', '-', 'LineWidth',1.5);      
    end
   
end