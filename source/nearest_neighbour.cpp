/*
   nearest_neighbour.cpp

    Data di creazione:      14 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */


#include "nearest_neighbour.h"

#include <iostream>
#include <cstdlib>


using namespace std;

void ricerca_minimo(unsigned int fronteer_size, FronteerNode *frontiera, FronteerNode *min){
    min->id       = frontiera[0].id;
    min->distance = frontiera[0].distance;

    if (fronteer_size == 1)
        return;

    for (unsigned int i=1; i<fronteer_size; i++){
        if (frontiera[i].distance < min->distance){
            min->id = frontiera[i].id;
            min->distance = frontiera[i].distance;
        }
    }
}

double costruisci_tour(unsigned int size, unsigned int *tour, int start){
    int current = start;            // scegliere una città in maniera arbitraria come città corrente.
    double dist = 0;

    tour[0] = city_id[current];     // la prima città scelta viene salvata nel tour
    city_visited[current] = true;   // la prima cottà viene impostata come visitata

    /* creazion dell'array della frontiera*/
    FronteerNode *frontiera = (FronteerNode *)malloc(sizeof(FronteerNode) * size);

    for (unsigned int j=0; j<size-1; j++) {

        // trovare il percorso più corto tra la città attuale e una vittà non ancora visitata.
        // la frontiera si riduce di 1 città ad ogni iterazione
        unsigned int fronteer_size = size - j -1;
        FronteerNode temp;

        // Costruzione della frontiera
        unsigned int p = 0;                   // posizione nell'array della frontiera
        for (unsigned int i=0; i<size; i++){
            if (!city_visited[i]){
                frontiera[p].id = i;
                frontiera[p].distance = distances[current][i];
                p++;
            }
        }

        ricerca_minimo(fronteer_size, frontiera, &temp);  // ricerca della distanza minima all'interno della frontiera

        current = temp.id;      // imposto la città trovata come città attuale.
        dist += temp.distance;  // aumento la distanza percorsa

        city_visited[current] = true;   // imposto come visitata la città
        tour[j+1]= city_id[current];    // aggiungo la città al tour
    }

    return dist;
}
