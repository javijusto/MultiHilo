#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#define MAX 10 //números a producir
pthread_mutex_t the_mutex;
pthread_cond_t condc,condp;
#define N 10 //TAMAÑO DEL BUFFER 
int buffer[N]; //buffer usado entre el productor y el consumidor

void *producer(){ //Hilo productor
	int i,tiempo;
	for(i=0;i<MAX;i++){ //Bucle productor
        	tiempo = rand()%4; //Generamos un numero aleatorio entre 0 y 4		
        	sleep(tiempo); //Esperamos el tiempo
		pthread_mutex_lock(&the_mutex); //Conseguir acceso exclusivo al buffer

        while(buffer[i]!=0) 
		pthread_cond_wait(&condp,&the_mutex); //Bloqueamos condp según the_mutex
	buffer[i]=i+1; //Poner objeto en el buffer
	printf("Producido en %d, el producto %d\n",i,buffer[i]);
	pthread_cond_signal(&condc); //Despierta al consumidor
	pthread_mutex_unlock(&the_mutex); //Libera el acceso al buffer
 	}
	pthread_exit(0); //Finalizamos el hilo
}

	void *consumer(){ //Hilo consumidor
		int i,tiempo;

		for(i=0;i<MAX;i++){  //Bucle consumidor
		tiempo = rand()%4; //Generamos un numero aleatorio entre 0 y 4		
		sleep(tiempo); //Esperamos el tiempo
		pthread_mutex_lock(&the_mutex);//Acceso exclusivo al buffer
		while (buffer[i] == 0) 				pthread_cond_wait(&condc,&the_mutex); //Bloqueamos condp según the_mutex
		printf("-----------------------------------> Consumido en: %d, el producto: %d\n",i,buffer[i]);
		buffer[i]= 0; //Sacamos el objeto fuera del buffer
		pthread_cond_signal(&condp); //Despertamos al productor
		pthread_mutex_unlock(&the_mutex); //Liberamos el acceso al buffer
		}
		pthread_exit(0); //Finalizamos el hilo
	}

int main(void){
	printf("Productor - Consumidor \n");
	srand(time(NULL)); //Ponemos la semilla de los aleatorios
	pthread_t pro,con; //Declaramos los hilos
	pthread_mutex_init(&the_mutex,0); //Inicializamos la variable the_mutex
	pthread_cond_init(&condc,0); //Inicializamos la variable condc
	pthread_cond_init(&condp,0); //Inicializamos la variable condp
	pthread_create(&con,0,consumer,0); //Creamos el hilo consumidor y les asignamos su rutina a realizar
	pthread_create(&pro,0,producer,0); //Creamos el hilo productor y les asignamos su rutina a realizar
	pthread_join(pro,0); //Esperamos a que termine el hilo productor
	pthread_join(con,0); //Esperamos a que termine el hilo consumidor
	pthread_cond_destroy(&condc); //Borramos la variable condc
	pthread_cond_destroy(&condp); //Borramos la variable condp
	pthread_mutex_destroy(&the_mutex); //Borramos la variable que hacía de mutex 		
}

