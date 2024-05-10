#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Variables globales */
int x = 0; // Déclaration de la variable globale x
sem_t m; // Sémaphore pour contrôler l'accès à 'x'

/* Fonction du thread */
void *thread_function(void *arg)
{
    int i = (int)arg;
    sem_wait(&m); // Entrée en section critique
    x = x + 1;
    sem_post(&m); // Sortie de la section critique
    printf("x: %d, i: %d\n", x, i);
    return NULL;
}

void main ()
{
    pthread_t tid[10]; // Tableau de threads
    int i;

    // Initialisation du sémaphore avec la valeur 1
    sem_init(&m, 0, 1);

    // Création de dix threads
    for (i = 0; i < 10; i++) {
        if (pthread_create(&tid[i], NULL, thread_function, (void *)i) < 0) {
            perror("Erreur : le thread ne peut pas être créé");
            exit(1);
        }
    }

    // Attente de la terminaison de tous les threads
    for (i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Valeur finale de x : %d\n", x);

    // Destruction du sémaphore
    sem_destroy(&m);
    exit(0);
}
