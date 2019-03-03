/*
   nearest_neighbour.h

    Data di creazione:      14 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#ifndef NEAREST_NEIGHBOUR_H
#define NEAREST_NEIGHBOUR_H

#include "utilities.h"


using namespace std;

extern double **distances;
extern int    *city_id;
extern bool   *city_visited;

/**
  Struttura utilizzata per la creazione di frontiere.
  */
typedef struct {
    int id;
    double distance;
} FronteerNode;


/**
  Funzione utilizzata per la ricerca del valore minimo all'interno di una frontiera.
  \param fronteer_size  dimensione della frontiera attuale
  \param frontiera      la frontiera in cui cercare il minimo
  \param min            il nodo minore verrà salvato in questo parametro
  */
void ricerca_minimo(unsigned int fronteer_size, FronteerNode *frontiera, FronteerNode *min);

/**
  Algoritmo di costruzione: Nearest Neighbour.
  \param size       numro di città nella mappa
  \param tour       vettore in cui verrà salvato il tour
  \param start      città iniziale. Solamente i valori positivi vengono accettati come città iniziali valide.
  \return           la lunghezza del tour trovato
  */
double costruisci_tour(unsigned int size, unsigned int *tour, int start=0);

#endif // NEAREST_NEIGHBOUR_H
