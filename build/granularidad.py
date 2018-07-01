from subprocess import call

def granu():
    clave = "_granu"
    for i in [1, 2, 4, 5, 10, 20, 25, 50]:
        print(clave + " " + str(i))
        call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100",str(i), "100", "100", (clave+str(i))])

def ruido():
    clave = "_ruido"
    for i in [x * 0.01 for x in [1,10,20,50,100]]:
        clavei = clave + "_" + str(i)
        call(["./tp3","~/3TPMetodos/imgs_TC/atomo", str(i), "100","10", "100", "100", clavei])

def emisores():
    clave = "_emisores"
    for i in range(1,100,30):
        print(clave + " " + str(i))
        call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100","10", str(i), "100", (clave+str(i))])


def rayos():
    clave = "_rayos"
    for i in range(80,5,100):
        print(clave + " " + str(i))
        call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100","10", "100", str(i), (clave+str(i))])


emisores()