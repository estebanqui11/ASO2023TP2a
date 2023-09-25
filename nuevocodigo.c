#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
pthread_mutex_t lock; // Declaración del mutex

void *comer_hamburguesa(void *tid) {
    while(1) {
        pthread_mutex_lock(&lock); // Bloquear el acceso a la variable compartida

        if (cantidad_restante_hamburguesas > 0) {
            printf("Hola! Soy el hilo (comensal) %ld. Me voy a comer una hamburguesa! Queda(n) %d\n", (long)tid, cantidad_restante_hamburguesas);
            cantidad_restante_hamburguesas--;
        } else {
            printf("SE TERMINARON LAS HAMBURGUESAS :( \n"); 
            pthread_mutex_unlock(&lock); // Importante desbloquear antes de salir
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(&lock); // Desbloquear el acceso a la variable compartida
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    pthread_mutex_init(&lock, NULL); // Inicializar el mutex

    for(int i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d \n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)(long)i);
        if (status != 0) {
            printf("Algo salió mal al crear el hilo. Código de error: %d \n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval); // Espero por la terminación de los hilos que creé
    }

    pthread_mutex_destroy(&lock); // Liberar el mutex al final

    return 0;
}
 

