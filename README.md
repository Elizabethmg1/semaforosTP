# SOR-Semáforos-1S-2021
# Trabajo Práctico Semáforos primer semestre año 2021

## Materia:Sistemas Operativos y Redes
## Docentes:Mariano Vargas, Ignacio Tula y Noelia Sosa.
## Alumna:Micaela Elizabeth Gimenez
## Legajo:40860713/14
## Comisión:01
## Fecha:18/04/2021



Este trabajo práctico trata sobre la resolución de una consigna en la cual se debe desarrollar un programa en lenguaje C que debe contener una competencia en la que 4 equipos tienen que hacer un sanguche cada uno, para coordinar los pasos y el uso de las herramientas de cocina se deben implementar semáforos los cuáles deben ser ubicados estratégicamente. En primer lugar se presenta el pseudocodigo que es la base sobre la cual se organizaron los semáforos y luego se explica paso por paso las partes del cádigo, se listan los problemas al desarrollar el trabajo práctico y soluciones, y por último se desarrola una conclusión sobre el trabajo realizado.

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
* 1.En la parte superior del código se declaran las bibliotecas a utilizar y las variables globales para que se puedan utilizar en todo el programa, en este caso los 3 semáforos que corresponden a el horno, sarten y salero se declaran en esa parte del codigo ya que son recursos compartidos, tambien se declara un dato de tipo entero llamado "ganar" el cual se inicializa en 1 el cual es modificado cuando un hilo llega primero a la función "ganar" esta se vuelve 0 y no se vuelve a mostrar a otro equipo como ganador.
* 2.En la parte inferior al código mencionado se declara la estructura _struct_ "parametro" la cual contiene los diferentes tipos de datos que son la informacion de cada equipo, como semáforos, numero de equipo y pasos de la receta. 
* 3.Luego se encuentra la función para imprimir las acciones y los ingredientes, a la cual se le pasa la informacion del equipo y la accion a realizar, se recorre el array de pasos buscando la acción a realizar, lo mismo con los pasos y esta informacion es mostrada y luego escrita en un archivo de texto "salida.txt" con el método _fprintf()_ la cual es encuentra su referencia en la variable "salida" la cual fue declarada en la parte superior del código y luego inicializada con el método _fopen()_ para ser utilizada por el resto del programa.
* 4.A continuación también se declaran las funciones que se encargan de que cada paso sea impreso a través del método _imprimirAccion()_ como "cortar", en particular estas funciones que simulan ser los pasos pueden ser accedidas en relación al estado de los semáforos declarados compartidos entre los hilos de un solo equipo y también de los semáforos compartidos entre los hilos que corresponden a los equipos, esto se puede realizar a través de los metodos _sem_wait()_ (para esperar a que el semáforo se encuentre en 1) y _sem_post()_(para sumarle 1 al valor de un semáforo para que pueda ser accedido). Estas funciones permiten que haya una entrada hacia los metodos  que simulan ser los pasos mas organizada.
* 5.Dentro de la funcion _ejecutarReceta()_ que es ejecutada por todos los equipos representados por hilos:
* 5a. En la primera parte se declaran los semáforos que van a ser contenidos en la estructura de datos del equipo, se declaran los hilos que corresponden a todos los pasos a ejecutar para completar la receta y se setea el _struct_ del equipo colocandole los datos necesarios.
* 5b. Se lee el archivo que contiene la receta con la función _fopen()_ y se guarda su contenido en una cadena de String llamada "receta", que luego sera utilizada para fraccionarla con la función _strtok()_ en 8 _arrays_ de _char_ auxiliares con un _while_ el cual itera obteniendo todos los _token_(String) y colocandolo en cada array auxiliar.
* 5c. Luego de obtener estas 8 cadenas de texto auxiliares se declara otra cadena de texto auxiliar "paso" el cual con un _while_ toma el valor de cada una de las anteriores cadenas de texto mencionadas con _strcpy()_ , con el objetivo de volver a aplicar el metodo _strtok()_, esta vez para obtener la accion y los ingredientes contenido en cada cadena de texto y colocarlas en la estructura de datos del equipo con el metodo _strcpy_.
* 5d. A continuación se inicializan los semáforos que seran compartidos solamente dentro del equipo con _sem_init()_ , se crean los hilos que simulan los pasos de la receta y se le pasa la estructura con los datos cargados, se hace el join de todos los hilos, se los destruye con _sem_destroy()_ y se hace la salida de los hilos.
* 6.En la función _main()_ se crean los nombres de los equipos guardando espacio para su valor, se crean las variables hilos de cada equipo, se inicializan los semáforos compartidos por todos los equipos con la receta a ejecutar y su nombre de equipo, luego de ejecutarse por completo la función otorgada como parametro a cada hilo se hace un join de todos los hilos y se los finaliza.

Problemas encontrados al realizar el trabajo práctico:
* La asignación de valor en C produce aliassing por lo cual dio dificultad al utilizar el método _strtok()_.
* Al utilizar métodos referidos a semáforos en C debia pasar como parámetro el puntero hacia el semáforo declarado.
* Al formular el código para dividir el contenido de la receta del archivo de texto con el método _strtok()_ para dividirlo por el delimitador y luego en un bucle interno dividir ese mismo token en otros token, dio el problema de que no era posible ejecutarse el _while_ externo lo que significa que este método no puede ser usado concurrentemente, y este problema fue solucionado haciendo las dos divisiones de token por separado utilizando cadenas de texto auxiliares.
* En el momento que fueron declarados los semáforos compartidos por todos equipos estos no eran considerados como inicializados por las funciones y fue solucionado declarando dichos semaforos en la parte superior del código.
* En general los problemas relacionados con la sintaxis propia del lenguaje C fueron solucionados buscando informacion y ejemplos relacionados en internet y leyendo la documentación otorgada como el libro de C de Ritchie Kernighan.

Conclusión:
Este trabajo práctico es una buena práctica para practica y aprender la sintaxis del lenguaje C y sus particularides, entender como funcionan los semáforos y como aplicarlos en un lenguaje a partir de una base en pseudocódigo. También es una forma útil de habituarse a buscar métodos y formas de solucionar problemas en cualquier lenguaje de programación.

