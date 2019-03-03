/*
   opt.h

    Data di creazione:      21 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#ifndef OPT_H
#define OPT_H

#include "utilities.h"


using namespace std;


extern double **distances;      // matrice delle distanze
extern int    *city_id;         // array dell'id delle città
extern bool   *city_visited;    // array dello stato di visita delle città

/**
  Funzione per il calcolo del guadagno dello scambio in base agli indici forniti.
  \param size       numro di città nella mappa
  \param i          primo indice
  \param j          secondo indice
  \param tour       tour in cui scambiare gli indici
  \return           il guadagno basato sugli scambio degli indici forniti
  */
inline double opt2_compute_gain(unsigned int size, unsigned int i, unsigned int j, unsigned int *tour){
    return distances[tour[i]][tour[j]]
            +distances[tour[(i+1) % size]][tour[(j+1) % size]]
            -distances[tour[i]][tour[(i+1) % size]]
            -distances[tour[j]][tour[(j+1) % size]];
}

/**
  Funzione per lo scambio dei nodi e la creazione del nuovo vettore secondo il miglioramento di 2opt.
  \param size       numro di città nella mappa
  \param i          primo indice
  \param j          secondo indice
  \param tour       tour in cui scambiare gli indici
  \param new_tour   nuovo tour con gli indici scambiati
  */
void opt2_exchange(unsigned int size, unsigned int i, unsigned int j, unsigned int *tour);

/**
  Funzione per l'esecuzione completa del 2-opt.
  \param size       numro di città nella mappa
  \param tour       tour da migliorare
  */

void opt2(unsigned int size, unsigned int *tour);

#endif // OPT_H
