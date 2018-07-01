
ruidoAditivo <- read.table("/home/idefix/Documents/Metnum/TP3-Metodos/informe/resultados/vectorTiemposConRuidoAditivo.txt", header = TRUE)
ruidoMultiplicativo <- read.table("/home/idefix/Documents/Metnum/TP3-Metodos/informe/resultados/vectorTiemposConRuidoMultiplicativo.txt", header = TRUE)
sinRuido <- read.table("/home/idefix/Documents/Metnum/TP3-Metodos/informe/resultados/vectorTiemposSinRuido.txt", header = TRUE)
vector1 <- unname(unlist(ruidoAditivo))

> hist(vector1, main="Ruido aditivo", xlab="Tiempo", border="blue", col="grey")
