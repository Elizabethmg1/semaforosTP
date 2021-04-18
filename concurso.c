#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>     //para usar strtok
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#include <stddef.h> //NULL



#define LIMITE 50

//declaro semaforos globales
sem_t sem_salero;
sem_t sem_sarten;
sem_t sem_horno;

int ganar=1;


//escribo la salida en un archivo de texto
FILE *salida;

//creo estructura de semaforos 
struct semaforos {
        sem_t sem_cortar1; //corta ajo y perejil
        sem_t sem_mezclar;
        sem_t sem_salar;
        sem_t sem_empanar;
        sem_t sem_fritar;
        sem_t sem_hornear;
        sem_t sem_cortar2; //corta lechuga, tomate, cebolla morada, y pepino
        sem_t sem_milaLista;
        sem_t sem_panListo;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
        fprintf(salida,"\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
        fprintf(salida,"\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
                            fprintf(salida,"\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
    // fclose(salida);
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
    	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_cortar1);
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 100000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_mezclar);
	char *accion = "mezclar";
	imprimirAccion(mydata,accion);
	usleep( 300000 );
    sem_post(&mydata->semaforos_param.sem_salar);
	
    pthread_exit(NULL);
}

void* salar(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_salar);
    sem_wait(&sem_salero);
	char *accion = "salar";
	imprimirAccion(mydata,accion);
	usleep( 300000 );
    sem_post(&sem_salero);
    sem_post(&mydata->semaforos_param.sem_empanar);
	
    pthread_exit(NULL);
}

void* empanar(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_empanar);
	char *accion = "empanar";
	imprimirAccion(mydata,accion);
	usleep( 300000);
    sem_post(&mydata->semaforos_param.sem_fritar);
	
    pthread_exit(NULL);
}

void* fritar(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_fritar);
    sem_wait(&sem_sarten);
	char *accion = "fritar";
	imprimirAccion(mydata,accion);
	usleep( 300000);
    sem_post(&sem_sarten);
    sem_post(&mydata->semaforos_param.sem_milaLista);
	
    pthread_exit(NULL);
}

void* hornear(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_hornear);
    sem_wait(&sem_horno);
	char *accion = "hornear";
	imprimirAccion(mydata,accion);
	usleep( 300000 );
    sem_post(&sem_horno);
    sem_post(&mydata->semaforos_param.sem_panListo);
	
    pthread_exit(NULL);
}

void* cortar2(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_cortar2);
	char *accion = "cortar2"; 
	imprimirAccion(mydata,accion);
	usleep( 300000 );

    pthread_exit(NULL);
}

void* armar(void *data) {
    struct parametro *mydata = data;
    sem_wait(&mydata->semaforos_param.sem_milaLista);
    sem_wait(&mydata->semaforos_param.sem_panListo);
	char *accion = "armar";
	imprimirAccion(mydata,accion);
    if(ganar==1){
        printf("\tEQUIPO %d GANO!\n",mydata->equipo_param);
        fprintf(salida,"\tEQUIPO %d GANO!\n",mydata->equipo_param);
        ganar=0;
    }
	usleep( 300000 );
	
    pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {
	
	//variables semaforos
        sem_t sem_cortar1; //corta ajo y perejil
        sem_t sem_mezclar;
        sem_t sem_salar;
        sem_t sem_empanar;
        sem_t sem_fritar;
        sem_t sem_hornear;
        sem_t sem_cortar2; //corta lechuga, tomate, cebolla morada, y pepino
        sem_t sem_milaLista;
        sem_t sem_panListo;
	
	//variables hilos
	pthread_t p1; 
    pthread_t p2; 
    pthread_t p3; 
    pthread_t p4; 
    pthread_t p5; 
    pthread_t p6; 
    pthread_t p7; 
    pthread_t p8;
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);
    fprintf(salida,"Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_cortar1 = sem_cortar1;
    pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
    pthread_data->semaforos_param.sem_empanar = sem_empanar;
    pthread_data->semaforos_param.sem_fritar = sem_fritar;
    pthread_data->semaforos_param.sem_hornear = sem_hornear;
    pthread_data->semaforos_param.sem_cortar2 = sem_cortar2;
    pthread_data->semaforos_param.sem_milaLista = sem_milaLista;
    pthread_data->semaforos_param.sem_panListo = sem_panListo;
    
        FILE *archivo;
        char caracter;

        archivo = fopen("receta.txt","r");

        char receta[400];
        if (archivo == NULL)
        {
            printf("\nError de apertura del archivo. \n\n");
        }
        else
        {
        int indice=0;
            while((caracter = fgetc(archivo)) != EOF)
            {
                receta[indice]=caracter;
                indice++;
            }
        }
        fclose(archivo);

        char *lineas= strtok(receta,"\n\t");
        //declaro Strings donde se van a guardar el contenido del token que se obtiene de lineas
         char aux1[100]="";
            char aux2[100]="";
            char aux3[100]="";
            char aux4[100]="";
            char aux5[100]="";
            char aux6[100]="";
            char aux7[100]="";
            char aux8[100]="";
        if (lineas!=NULL){
            int indice1=0;
            while(lineas!=NULL){
                //se guarda cada linea obtenida en un string diferente
                switch(indice1){
                    case 0:
                        strcpy(aux1,lineas);
                        break;
                    case 1:
                        strcpy(aux2,lineas);
                        break;
                    case 2:
                        strcpy(aux3,lineas);
                        break;
                    case 3:
                        strcpy(aux4,lineas);
                        break;
                    case 4:
                        strcpy(aux5,lineas);
                        break;
                    case 5:
                        strcpy(aux6,lineas);
                        break;
                    case 6:
                        strcpy(aux7,lineas);
                        break;
                     case 7:
                        strcpy(aux8,lineas);
                        break;
                }
               
                indice1=indice1+1;
                lineas=strtok(NULL,"\n\t");
            }
        }
            //se crea string auxiliar para utilizar con strtok
            char paso[100]="";
            for(int j=0; j<8 ; j++){
                //en cada iteracion paso toma el valor de cada una de las string guardadas anteriormente
                switch(j){
                     case 0:
                        strcpy(paso,aux1);
                        break;
                    case 1:
                        strcpy(paso,aux2);
                        break;
                    case 2:
                        strcpy(paso,aux3);
                        break;
                    case 3:
                        strcpy(paso,aux4);
                        break;
                    case 4:
                        strcpy(paso,aux5);
                        break;
                    case 5:
                        strcpy(paso,aux6);
                        break;
                    case 6:
                        strcpy(paso,aux7);
                        break;
                     case 7:
                        strcpy(paso,aux8);
                        break;
                }
                //en cada iteracion se divide la accion y sus ingredientes por el delimitador "|"
                 char *pasos=strtok(paso,"|");
                 if(pasos!=NULL){
                    int indice2=0;
                    while(pasos!=NULL){
                        if(indice2==0){
                            //primero guardo la accion
                            strcpy(pthread_data->pasos_param[j].accion,pasos);
                        }
                        else{
                            //en la siguientes iteraciones guardo los ingredientes
                            strcpy(pthread_data->pasos_param[j].ingredientes[indice2-1],pasos);
                        }
                        indice2=indice2+1;
                        pasos=strtok(NULL,"|");
                }
                }
            }
            
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_cortar1),0,1);
    sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_fritar),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_hornear),0,1);
    sem_init(&(pthread_data->semaforos_param.sem_cortar2),0,1);
    sem_init(&(pthread_data->semaforos_param.sem_panListo),0,0);
    sem_init(&(pthread_data->semaforos_param.sem_milaLista),0,0);


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

	rc= pthread_create(&p2,NULL,mezclar,pthread_data);
	rc= pthread_create(&p3,NULL,salar,pthread_data);
    rc= pthread_create(&p4,NULL,empanar,pthread_data);
    rc= pthread_create(&p5,NULL,fritar,pthread_data);
    rc= pthread_create(&p6,NULL,hornear,pthread_data);
    rc= pthread_create(&p7,NULL,cortar2,pthread_data);
    rc= pthread_create(&p8,NULL,armar,pthread_data);

	//join de todos los hilos
	pthread_join (p1,NULL);
    pthread_join (p2,NULL);
    pthread_join (p3,NULL);
    pthread_join (p4,NULL);
    pthread_join (p5,NULL);
    pthread_join (p6,NULL);
    pthread_join (p7,NULL);
    pthread_join (p8,NULL);


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }


	//destruccion de los semaforos 
	sem_destroy(&sem_cortar1);
    sem_destroy(&sem_mezclar);
    sem_destroy(&sem_salar);
    sem_destroy(&sem_empanar);
    sem_destroy(&sem_fritar);
    sem_destroy(&sem_hornear);
    sem_destroy(&sem_cortar2);

	//salida del hilo
	 pthread_exit(NULL);
}

int main ()
{
    salida=fopen("salida.txt","w");
    if(salida==NULL){
        printf("Error escribiendo archivo");
    }
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
    int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
    *equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
    pthread_t equipo3; 
	pthread_t equipo4;

    sem_init(&sem_horno,0,2);
    sem_init(&sem_salero,0,1);
    sem_init(&sem_sarten,0,1);
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3); 

    rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
    pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);

    sem_destroy(&sem_horno);
    sem_destroy(&sem_salero);
    sem_destroy(&sem_sarten);

    pthread_exit(NULL);

    fclose(salida);
}
