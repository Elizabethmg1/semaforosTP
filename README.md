# SOR-Semáforos-1S-2021
# Trabajo Práctico Semáforos primer semestre año 2021

## Materia:Sistemas Operativos y Redes
## Docentes:Mariano Vargas, Ignacio Tula y Noelia Sosa.
## Alumna:Micaela Elizabeth Gimenez
## Legajo:40860713/14
## Comisión:01
## Fecha:18/04/2021



Este trabajo práctico trata sobre la resolución de una consigna en la cual se debe desarrollar un programa en lenguaje C que debe contener una competencia en la que 4 equipos tienen que hacer un sanguche cada uno, para coordinar los pasos y el uso de las herramientas de cocina se deben implementar semáforos los cuáles deben ser ubicados estratégicamente. En primer lugar se presenta el pseudocodigo que es la base sobre la cual se organizaron los semáforos y luego se explica paso por paso las partes del cádigo y por último se desarrola una conclusión sobre el trabajo realizado.

En primer lugar, el pseudocodigo utilizado para la implementación de semaforos:
```
Semaforos globales:
Sem_salero      =1
Sem_sarten      =1
Sem_hornoPan    =2

Semaforos individuales:
Sem_cortar  =1 (corta ajo y perejil)
Sem_mezclar =0
Sem_salar   =0
Sem_empanar =0
Sem_fritar  =0
Sem_hornear =1
Sem_cortar2  =1  (corta lechuga, tomate,etc)
Sem_milaLista=0
Sem_panListo =0

P(sem_salar)
P(sem_cortar)	        P(sem_mezclar)        P(sem_salero)		   P(sem_empanar)
     Cortar             mezclar	        	    Salar		            empanar
v(sem_mezclar)       	 V(sem_salar)		     V(sem_salero)		    V(sem_fritar)
V(sem_empanar)


P(sem_fritar)		  P(sem_hornear)                            P(sem_panListo)
P(sem_sarten)	    P(sem_horno)		   P(sem_cortar2)        P(sem_milaLista)
  Fritar	    	     Hornear		       Cortar2               Armar sanguche
V(sem_sarten)	     V(sem_horno)
V(sem_milaLista)   V(sem_panListo)

```
Prueba de escritorio de un solo equipo, con el orden de los procesos que se fueron ejecutando:
```
Sem_salero      =1 1 1 0 1 1 1
Sem_sarten      =1 1 1 1 1 0 1 
Sem_hornoPan    =2 1 2 2 2 2 2

Sem_cortar   =1 0 0 0 0 0 0
Sem_mezclar  =0 1 0 0 0 0 0
Sem_salar    =0 0 1 0 0 0 0
Sem_empanar  =0 0 0 1 0 0 0
Sem_fritar   =0 0 0 0 1 0 0
Sem_hornear  =1 0 0 0 0 0 0
Sem_cortar2  =1 0 0 0 0 0 0
Sem_milaLista=0 0 0 0 0 1 0
Sem_panListo =0 1 1 1 1 1 0

cortar - hornear - cortar 2 - mezclar - salar - empanar - fritar - armar sanguche

```
En segundo lugar se organizó y completó el código brindado para que pueda cumplir las consignas dadas en el enunciado del trabajo práctico:
En la parte superior del código se declaran las bibliotecas a utilizar y las variables globales para que se puedan utilizar en todo el programa, en este caso los 3 semáforos que corresponden a el horno, sarten y salero se declaran en esa parte del codigo ya que son recursos compartidos, tambien se declara un dato de tipo entero llamado "ganar" el cual se inicializa en 1 el cual es modificado cuando un hilo llega primero a la función "ganar"  
