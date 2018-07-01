from subprocess import call
import os

def granu():
    clave = "granu"
    for i in [1, 2, 4, 5, 10, 20, 25, 50]:
    # for i in [25, 50]:
        clavei = "_" + clave + "_" + str(i)
        print(clavei)
        # call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100",str(i), "100", "100", clavei])
        os.system("./tp3 ~/3TPMetodos/imgs_TC/atomo 0 100 "+ str(i) +" 100 100 "+ str(clavei))
        print("")

def ruido():
    clave = "ruido"
    for i in [x * 0.01 for x in [1,10,20,50,100]]:
        clavei = "_" + clave + "_" + str(i)
        print(clavei)
        os.system("./tp3 ~/3TPMetodos/imgs_TC/atomo "+ str(i) +" 100 10 100 100 "+ str(clavei))
        # call(["./tp3","~/3TPMetodos/imgs_TC/atomo", str(i), "100","10", "100", "100", clavei])
        print("")

def emisores():
    clave = "emisores"
    for i in range(1,100,30):
        clavei = "_" + clave + "_" + str(i)
        print(clavei)
        os.system("./tp3 ~/3TPMetodos/imgs_TC/atomo 0 100 10 "+ str(i) +" 100 "+ str(clavei))
        # call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100","10", str(i), "100", clavei])
        print("")


def rayos():
    clave = "rayos"
    for i in range(50,150,10):
        clavei = "_" + clave + "_" + str(i)
        print(clavei)
        os.system("./tp3 ~/3TPMetodos/imgs_TC/atomo 0 100 10 100 "+ str(i) +" 100 "+ str(clavei))
        # call(["./tp3","~/3TPMetodos/imgs_TC/atomo", "0", "100","10", "100", str(i), clavei])
        print("")


granu()
print("\n")
emisores()
print("\n")
rayos()
print("\n")