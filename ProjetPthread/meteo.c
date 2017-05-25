#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>


void* generation_meteo(void* arg)
{
    srand(time(0));


    unsigned max_sleep = 40;
    unsigned min_rand = 1;
    unsigned taille_meteo_proposees = (sizeof(METEO_PROPROSEES)/sizeof(METEO_PROPROSEES[0]));

    while(1)
    {


        unsigned index_meteo = (rand() % (taille_meteo_proposees));

        meteo = METEO_PROPROSEES[index_meteo];

        set_color(0, 14);
        printf("Changement Meteo : %s\n", meteo);


        unsigned sleep_time = (rand() % (max_sleep - min_rand + 1)) + min_rand;
        sleep(sleep_time);
    }

    pthread_exit ((void*)0);

}


void init_meteo()
{

    sem_init(&m_meteo, 0, 1);


    pthread_create(&t_dest, NULL, generation_meteo, NULL);
}



void take_meteo(unsigned num_cam, unsigned couleur_ale)
{


    int meteo_dispo = 0;

    sem_getvalue(&m_meteo, &meteo_dispo);


    if(meteo_dispo <= 0)
    {
        set_color(couleur_ale, 0);
        printf("\tN-%i : Meteo occpuée\n",num_cam);
    }

    sem_wait(&m_meteo);


    set_color(couleur_ale, 0);
    printf("\tN-%i : Meteo : %s\n",num_cam, meteo);

    sem_post(&m_meteo);



}






