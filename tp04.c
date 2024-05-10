/*
 * Auteur: Walid HASNAOUI
 * Drn modification : 10/05/2024
 * 
 * Ce programme simule une course de voitures avec un arbitre et des voitures. Chaque voiture
 * doit effectuer 10 tours de piste. L'arbitre donne le signal de départ de la course, puis
 * chaque voiture commence à effectuer ses tours. Les tours complétés par chaque voiture sont
 * affichés à l'écran
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NB_VOITURES 5

typedef struct {
    int id;
    int tours_completés;
} Voiture;

Voiture voitures[NB_VOITURES];
sem_t depart_course;
pthread_mutex_t mutex_tours = PTHREAD_MUTEX_INITIALIZER;

// Fonction exécutée par chaque thread voiture
void* fonction_voiture(void* arg) {
    Voiture* voiture = (Voiture*)arg;

    // Attendre le signal de départ de la course
    sem_wait(&depart_course); 

    for(int i = 0; i < 10; i++) {
        usleep(rand() % 1000000);
        pthread_mutex_lock(&mutex_tours);
        voiture->tours_completés++;
        printf("Voiture %d a complété le tour %d\n", voiture->id, voiture->tours_completés);
        pthread_mutex_unlock(&mutex_tours);
    }

    return NULL;
}

// Fonction exécutée par le thread arbitre
void* fonction_arbitre(void* arg) {
    for(int i = 0; i < NB_VOITURES; i++) {
        sem_post(&depart_course); // Signal pour que toutes les voitures commencent la course
    }

    return NULL;
}

int main() {
    pthread_t threads_voitures[NB_VOITURES];
    pthread_t thread_arbitre;

    sem_init(&depart_course, 0, 0);

    for(int i = 0; i < NB_VOITURES; i++) {
        voitures[i].id = i + 1;
        voitures[i].tours_completés = 0;
        pthread_create(&threads_voitures[i], NULL, fonction_voiture, &voitures[i]);
    }

    pthread_create(&thread_arbitre, NULL, fonction_arbitre, NULL);

    for(int i = 0; i < NB_VOITURES; i++) {
        pthread_join(threads_voitures[i], NULL);
    }
    pthread_join(thread_arbitre, NULL);

    sem_destroy(&depart_course);

    return 0;
}
