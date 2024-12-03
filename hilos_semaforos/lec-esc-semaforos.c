#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_L 3
#define MAX_E 2

int dato = 0;
int n_lectores = 0;
sem_t mutex, m_escritores;

void *lector(void *arg) {
	int id = (int)arg;
	while (1) {
		// Leer datos
		sem_wait(&mutex);
		n_lectores++;
		if(n_lectores == 1){
			sem_wait(&m_escritores);
		}
		sem_post(&mutex);

		printf("Lector %d leyendo: %d\n", id, dato);
		// Retraso aleatorio de hasta 1 milisegundo
		usleep(rand() % 1000000);

		sem_wait(&mutex);
		n_lectores--;
		if(n_lectores == 0){
			sem_post(&m_escritores);
		}
		sem_post(&mutex);
	}
}

void *escritor(void *arg) {
	int id = (int)arg;
	int aux;
	while (1) {
		// Escribir datos
		sem_wait(&m_escritores);
		aux = dato;
		aux++;
		dato = aux;
		printf("Escritor %d escribiendo: %d\n", id, dato);
		sem_post(&m_escritores);
		// Retraso aleatorio de hasta 1 milisegundo
		usleep(rand() % 2000000);
	}
}
int main(void){ 
	// MAX_L lectores y MAX_E escritores
	pthread_t lectores[MAX_L], escritores[MAX_E];
	int i;
	pthread_attr_t atrib;
	sem_init(&mutex, 0, 1);
	sem_init(&m_escritores, 0, 2);
	
	// Inicializar la semilla del generador de números aleatorios
	srand(time(NULL));
	pthread_attr_init(&atrib);
	// Crear lectores
	for(int i = 0; i < MAX_L; i++){
		pthread_create(&lectores[i], &atrib, lector, (void*)i);
	}
	// Crear escritores
	for(int i = 0; i < MAX_E; i++){
		pthread_create(&escritores[i], &atrib, escritor, (void*)i);
	}
	// Esperar a que los hilos terminen
	for(int i = 0; i < MAX_L; i++){
		pthread_join(lectores[i], NULL);
	}

	for(int i = 0; i < MAX_E; i++){
		pthread_join(escritores[i], NULL);
	}

	printf("Acaba el main\n");
	return 0;
}
