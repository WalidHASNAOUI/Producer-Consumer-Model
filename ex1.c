#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* Variables globales */
int x = 0; // Déclaration de la variable globale x
sem_t sync_x; // Déclaration du sémaphore sync_x

/* Fonctions des threads */

// Fonction exécutée par le premier thread
void a(){
    sleep(10);  // Simule un travail en cours
    printf("X = %d\n", x); // Affiche la valeur actuelle de x
    x = 55; // Modifie la valeur de x
}

// Fonction exécutée par le deuxième thread
void b(){
    printf("X = %d\n", x); // Affiche la valeur actuelle de x
}

// Fonction exécutée par le premier thread
void *p1(void *arg)
{
    a(); // Appelle la fonction a
    // Indique au deuxième thread que 'a' a terminé
    sem_post(&sync_x);
    return NULL;
}

// Fonction exécutée par le deuxième thread
void *p2(void *arg)
{
    // Attend que le premier thread termine 'a'
    sem_wait(&sync_x);
    b(); // Appelle la fonction b
    return NULL;
}

int main () {
    pthread_t thread1, thread2;

    // Initialise le sémaphore sync_x à 0
    sem_init(&sync_x, 0, 0);

    // Crée les threads
    if (pthread_create(&thread1, NULL, p1, NULL) < 0) {
        perror("Erreur : le thread ne peut pas être créé");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, p2, NULL) < 0) {
        perror("Erreur : le thread ne peut pas être créé");
        exit(1);
    }

    // Attend que les threads créés se terminent
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Détruit le sémaphore
    sem_destroy(&sync_x);
    exit(0);
}
