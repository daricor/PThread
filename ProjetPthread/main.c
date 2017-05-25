#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <Windows.h>

#define NOMBRE_QUAIS 4
#define INIT_CAMIONS 6
#define NB_COUL 15
#define NB_DEST 6


pthread_t camions;
pthread_t t_dest;

sem_t m_pesee;
sem_t s_quai;
sem_t s_dest;
sem_t m_dest;

char* destinations_list[NB_DEST];
const char* DEST_PROPROSEES[] = {"MONTPELLIER", "MARSEILLE", "LYON", "PARIS", "BORDEAUX", "ARRAS"};


#include "camion.c"
#include "destination.c"



void set_color(int t, int f) // couleur affichage en console (t =text f= fond couleur de 0=noir a 15=blanc)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f * 16 + t);
}


int main(int argc, char **argv)
{


    unsigned max_new_camion = 15;
    unsigned min_rand = 0;

    init_destinations();
    init_cycle_camions();


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
