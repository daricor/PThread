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
#define NB_DEST 6


pthread_t camions;
pthread_t t_dest;

char* destinations_list[NB_DEST];
const char* DEST_PROPROSEES[] = {"MONTPELLIER", "MARSEILLE", "LYON", "PARIS", "BORDEAUX", "ARRAS"};

boolean take_dest(unsigned num_cam, unsigned couleur_ale)
{

    boolean ok = 0;
    boolean recus = 0;
    while(!ok)
    {
        int index = 0;

        while (index < NB_DEST)
        {
            if (destinations_list[index] != "null")
            {
                char* tmp_dest = destinations_list[index];
                destinations_list[index] = "null";

                set_color(couleur_ale, 0);
                printf("\tN-%i : Destination %s\n", num_cam, tmp_dest);
                return 1;
            }

            index++;
        }
        if(!recus)
        {
            set_color(couleur_ale, 0);
            printf("\tN-%i : Pas de destinations : En attente\n", num_cam);

            recus = 1;
        }

    }
}


void* cycleCamion(void* arg)
{
    srand(time(0));


    unsigned max_sleep_chargement = 30;
    unsigned max_sleep_pesee = 10;
    unsigned max_couleur = 15;

    unsigned min_rand = 1;

    int nombre_quais_libres;
    int pesee_libre;

    pthread_t selfThread = pthread_self();
    int selfIdThread = (int) selfThread;

    unsigned couleur_ale = (rand() % (max_couleur - min_rand + 1)) + min_rand;

    unsigned sleep_chargement = (rand() % (max_sleep_chargement - min_rand + 1)) + min_rand;
    unsigned sleep_pesee = (rand() % (max_sleep_pesee - min_rand + 1)) + min_rand;


    set_color(couleur_ale, 0);
    printf("N-%i Arrivee camion\n", selfIdThread);


    sem_getvalue(&s_quai,&nombre_quais_libres);


    take_dest(selfIdThread, couleur_ale);


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

void* destinations(void* arg)
{
    srand(time(0));


    unsigned max_sleep = 30;
    unsigned min_rand = 1;
    unsigned taille_dest_proposees =  sizeof(DEST_PROPROSEES);


    while(1)
    {
        int index = 0;
        boolean sortie = 0;

        while (index < NB_DEST && !sortie)
        {
            if (destinations_list[index] == "null")
            {
                unsigned index_dest = (rand() % (taille_dest_proposees - 0 + 1)) + 0;

                destinations_list[index] = DEST_PROPROSEES[index_dest];

                sortie = 1;
            }

            index ++;
        }

        unsigned sleep_time = (rand() % (max_sleep - min_rand + 1)) + min_rand;
        sleep(sleep_time);
    }

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

    unsigned max_new_camion = 15;
    unsigned min_rand = 0;

    for(unsigned i = 0; i<NB_DEST; ++i)
        destinations_list[i] = "null";


    pthread_create(&t_dest, NULL, destinations, NULL);
    init_cycle(INIT_CAMIONS);

    while (1)
    {
        unsigned time_new_camion = (rand() % (max_new_camion - min_rand + 1)) + min_rand;
        sleep(time_new_camion);
        nouveauCamion();
    }


    int status;
    pthread_join(camions, (void**) &status);
    pthread_join(t_dest, (void**) &status);

    return 0;

}
