#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<sys/syscall.h>
#define MAX 10 //Cuantos números se producen
pthread_mutex_t the_mutex;
pthread_cond_t condc,condp;
#define N 10 //TAMAÑO DEL BUFFER 
int buffer[N]; //buffer usado entre el productor y el consumidor
int p,c; //variables para numero de productores/consumidores (iniciadas en main)
int producir,consumir,controlador; //variables de control para llenado, vaciado y buufer lleno

void *producer(){ //Hilo productor
int i,tiempo;
unsigned int tid;
tid=syscall(SYS_gettid); //Obtenemos el id del hilo

	for(i=0;i<MAX*c;i++){ //Bucle productor
        tiempo = rand()%4; //Creamos un numero aleatorio entre 0 y 4		
        sleep(tiempo);  //Esperamos el tiempo aleatorio
	pthread_mutex_lock(&the_mutex); //Conseguir acceso exclusivo al buffer

        while(controlador==N) pthread_cond_wait(&condp,&the_mutex); //Bloqueamos condp según the_mutex
	buffer[producir]=rand()%30; //Poner objeto en el buffer
	printf("Producido por %u en %d, el producto %d, controlador= %d\n",tid,producir,buffer[producir],controlador);
	controlador++; //Aumentamos el numero de productos actual
	producir=(producir+1)%N; //Aumentamos la posicion en la que metemos los productos
	pthread_cond_signal(&condc); //Despierta al consumidor
	pthread_mutex_unlock(&the_mutex); //Libera el acceso al buffer
 	}
	pthread_exit(0); //Finalizamos el hilo
}

void *consumer(){ //Hilo consumidor
int i,tiempo;
unsigned int tid;
tid=syscall(SYS_gettid); //Obtenemos el id del hilo

	for(i=0;i<MAX*p;i++){  //Bucle consumidor
	tiempo = rand()%4; //Creamos un numero aleatorio entre 0 y 4		
        sleep(tiempo); // Esperamos el tiempo aleatorio
	pthread_mutex_lock(&the_mutex);//Acceso exclusivo al buffer
	while (controlador == 0) pthread_cond_wait(&condc,&the_mutex); //Bloqueamos condp según the_mutex
        printf("-----------------------------------> Consumido por %u en: %d, el producto: %d, controlador=%d\n",tid,consumir,buffer[consumir], controlador);
	buffer[consumir]= 0; //Sacamos el objeto fuera del buffer
	controlador--; //Disminuimos el número de productos disponibles
	consumir=(consumir+1)%N; // Aumentamos la posición de donde estamos consumiendo
	pthread_cond_signal(&condp); //Despertamos al productor
	pthread_mutex_unlock(&the_mutex); //Liberamos el acceso al buffer
	}
	pthread_exit(0); //Finalizamos el hilo
}

int main(void){
p=2; //Número de productores
c=1; //Número de consumidores

//Iniciamos las variables de control
producir=0;
consumir=0;
controlador=0;

printf("Productor - Consumidor \n");
srand(time(NULL)); //Ponemos la semilla de los aleatorios
int i;

pthread_t pro[p],con[c]; //Declaramos los hilos
pthread_mutex_init(&the_mutex,0); //Inicializamos la variable the_mutex
pthread_cond_init(&condc,0); //Inicializamos la variable condc
pthread_cond_init(&condp,0); //Inicializamos la variable condp


for (i=0;i<p;i++){
pthread_create(&pro[i],0,producer,0); //Creamos el hilo productor y les asignamos su rutina a realizar
}

for (i=0;i<c;i++){
pthread_create(&con[i],0,consumer,0); //Creamos el hilo consumidor y les asignamos su rutina a realizar
}

for (i=0;i<c;i++){
pthread_join(con[i],0); //Esperamos a que termine el hilo consumidor
}


for (i=0;i<p;i++){
pthread_join(pro[i],0); //Esperamos a que termine el hilo productor
}


pthread_cond_destroy(&condc); //Borramos la variable condc
pthread_cond_destroy(&condp); //Borramos la variable condp
pthread_mutex_destroy(&the_mutex); //Borramos la variable que hacía de mutex

  		
}

