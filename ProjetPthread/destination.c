#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>


void* generation_destinations(void* arg)
{
    srand(time(0));


    unsigned max_sleep = 5;
    unsigned min_rand = 1;
    unsigned taille_dest_proposees = (sizeof(DEST_PROPROSEES)/sizeof(DEST_PROPROSEES[0]));
    unsigned nb_dest_actuelles = 0;


    while(1)
    {


        sem_getvalue(&s_dest, &nb_dest_actuelles);

        if(nb_dest_actuelles < NB_DEST)
        {


            int index = 0;


            boolean sortie = 0;

            while (index < NB_DEST && !sortie)
            {
                if (destinations_list[index] == "NO")
                {
                    unsigned index_dest = (rand() % (taille_dest_proposees));

                    destinations_list[index] = DEST_PROPROSEES[index_dest];

                    set_color(0, 15);
                    printf("Ajout destination %s\n", DEST_PROPROSEES[index_dest]);
                    sem_post(&s_dest);

                    sortie = 1;
                }

                index ++;
            }
        }
        unsigned sleep_time = (rand() % (max_sleep - min_rand + 1)) + min_rand;
        sleep(sleep_time);
    }

    pthread_exit ((void*)0);

}


void init_destinations()
{

    //Tableau des villes à null

    for(unsigned i = 0; i<NB_DEST; ++i)
        destinations_list[i] = "NO";


    //semaphore des villes en bloqué
    sem_init(&s_dest, 0, 0);
    sem_init(&m_dest, 0, 1);


    pthread_create(&t_dest, NULL, generation_destinations, NULL);
}



void take_dest(unsigned num_cam, unsigned couleur_ale)
{

    boolean sortie = 0;
    int nb_dest_dispo = 0;
    unsigned index = 0;

    sem_getvalue(&s_dest, &nb_dest_dispo);


    if(nb_dest_dispo <= 0)
    {
        set_color(couleur_ale, 0);
        printf("\tN-%i : Pas de destination\n",num_cam);
    }

    sem_wait(&m_dest);

    while (!sortie)
    {
        if(index == NB_DEST)
            index = 0;

        if(destinations_list[index] != "NO")
        {



            char* destination = destinations_list[index];

            destinations_list[index] = "NO";
            sem_wait(&s_dest);


            set_color(couleur_ale, 0);
            printf("\tN-%i : Destination : %s\n",num_cam, destination);

            sem_post(&m_dest);

            sortie = 1;
        }

        index++;

    }


}






