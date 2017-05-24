#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>

sem_t m_pesee;
sem_t s_quai;

#define NOMBRE_QUAIS 4
#define INIT_CAMIONS 6
#define NB_COUL 15


pthread_t camions;

void* cycleCamion(void* arg)
{


    srand(time(0));

    unsigned max_sleep_chargement = 30;
    unsigned max_sleep_pesee = 10;
    unsigned max_couleur = 15;

    unsigned min_rand = 0;

    int nombre_quais_libres;
    int pesee_libre;

    pthread_t selfThread = pthread_self();
    int selfIdThread = (int) selfThread;

    int couleur_ale = (rand() % (max_couleur - min_rand + 1)) + min_rand;

    unsigned sleep_chargement = (rand() % (max_sleep_chargement - min_rand + 1)) + min_rand;
    unsigned sleep_pesee = (rand() % (max_sleep_pesee - min_rand + 1)) + min_rand;


    set_color(couleur_ale, 0);
    printf("N-%i Arrivee camion\n", selfIdThread);


    sem_getvalue(&s_quai,&nombre_quais_libres);

    if (nombre_quais_libres <= 0)
    {
        set_color(couleur_ale, 0);
        printf("\tN-%i : Pas de quais de libre : En attente\n", selfIdThread);
    }

    sem_wait(&s_quai);

    set_color(couleur_ale, 0);
    printf("\tN-%i : Quai libre, en deplacement\n", selfIdThread);

    sleep(sleep_chargement);

    sem_getvalue(&m_pesee,&pesee_libre);

    if (pesee_libre <= 0)
    {
        set_color(couleur_ale, 0);
        printf("\tN-%i : Pesee non libre : En attente\n", selfIdThread);
    }

    sem_wait(&m_pesee);

    set_color(couleur_ale, 0);
    printf("\tN-%i : Pesee libre : En deplacement\n", selfIdThread);

    sem_post(&s_quai);

    sleep(sleep_pesee);

    set_color(couleur_ale, 0);
    printf("N-%i : Fin du cycle\n", selfIdThread);

    sem_post(&m_pesee);

    pthread_exit ((void*)0);

}


void nouveauCamion()
{
    pthread_create(&camions, NULL, cycleCamion, NULL);
}


void init_cycle(unsigned n_)
{
    sem_init(&s_quai, 0, NOMBRE_QUAIS);
    sem_init(&m_pesee, 0, 1);


    for (unsigned i = 0; i<n_; ++i)
    {
        sleep(1);
        nouveauCamion();
    }
}

void set_color(int t, int f) // couleur affichage en console (t =text f= fond couleur de 0=noir a 15=blanc)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f * 16 + t);
}


int main(int argc, char **argv)
{



    init_cycle(INIT_CAMIONS);

    int status;
    pthread_join(camions, (void**) &status);

    return 0;

}
