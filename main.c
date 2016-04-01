//
//  main.c
//  EXR2.4
//
//  Created by Abraham Esses on 3/31/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//

//Una cadena de cines quiere implementar un sistema de reservación de asientos de tal manera que los clientes no tengan que formarse a la entrada de la sala para entrar a una función de un película. Los clientes de la cadena pueden comprar los boletos y reservar sus asientos de diferentes maneras: mediante el sitio Web de la cadena, mediante una aplicación móvil o directamente en una de las taquillas de uno de los complejos que forman parte de la cadena. La cadena está conformada por un número C (10) de complejos distribuidos en diferentes ciudades y cada complejo tiene un número S (3) de salas donde se exponen las películas y un número T(3) de taquillas o cajas donde se pueden adquirir los boletos. Suponiendo que el complejo quiere garantizar que el mayor número de clientes posibles puedan comprar boletos de manera simultánea (utilizando cualquiera de los tres medios posibles), programe una solución utilizando las técnicas de concurrencia y sincronización estudiadas, donde se evite la ocurrencia de bloqueos y la inanición,  garantizando el mayor nivel de concurrencia posible.
//Usuarios por sala 50

#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define nhilos 200

typedef struct{
    sem_t asientos;
}Sala;
typedef struct{
    Sala a;
    sem_t taquilla;
}Cine;

Cine * cines;
void * usuarios (void *);
Sala * salas;


void * usuarios(void * p){
    int rand_cine = rand()%10;
    int id = (int)p;
    //printf("Soy el hilo %d\n",id);
    Cine c = cines[rand_cine];
    printf("Entrando al cine %d\n",rand_cine);
    printf("Llegando a la taquilla\n");
    sem_wait(&c.taquilla);
    printf("Llegando a la sala\n");
    sem_wait(&c.a.asientos);
    printf("Viendo película...\n");
    sem_post(&c.taquilla);
    sem_post(&c.a.asientos);
    pthread_exit(NULL);
}
void init_cine(){
    for (int i = 0; i < 10; i++) {
        sem_init(&cines[i].taquilla,0,3);
        sem_init(&cines[i].a, 0, 3);
        sem_init(&cines[i].a.asientos, 0, 50);
    }
}
int main(int argc, const char * argv[]) {
    cines  = (Cine *)malloc(sizeof(Cine)*10);
    salas = (Sala *)malloc(sizeof(Sala)*3*10);
    
    pthread_t * threads_clientes = (pthread_t *) malloc(nhilos * sizeof(pthread_t));
        for (int i = 0; i < nhilos ; ++i) {
        pthread_create(&threads_clientes[i],NULL,usuarios,i);
    }
    
    /* Esperar a que todos los hilos terminen */
    
    for (int i = 0; i < nhilos; ++i) {
        
        pthread_join(*(threads_clientes+i), NULL);
    
    }
    
    free(threads_clientes);
    return 0;
}
