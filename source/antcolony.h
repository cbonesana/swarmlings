/*
   antcolony.h

    Data di creazione:      31 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#ifndef ANTCOLONY_H
#define ANTCOLONY_H

#include "ant.h"

#include "utilities.h"
#include "nearest_neighbour.h"
#include "opt.h"


using namespace std;

extern double **distances;  // matrice delle distanze
extern int    *city_id;     // array dell'indice delle città
extern bool    debug;       // parametro di debug

/**
  Algoritmo Ant Colony System.
  \param size               numro di città nella mappa
  \param best_solution      vettore con la soluzione finale dell'algoritmo
  \param start_city         città iniziale per l'algoritmo costruttivo (se minore di -1 esegue una scelta random)
  \param ant_number         numero totale di formiche
  \param alpha              determina il quantitativo di feromone lasciato dalle migliori formiche
  \param ro                 determina la variazione, l'evaporazione nel tempo del feromone
  \param q0                 determina la probabilità di scelta tra Exploitation o Exploration (0 < q0 < 1)
  \param q0_min             valore minimo di q0 (0 < q0_min < 1)
  \param q0_step            valore di decremento di q0 (0 < q0_min < 1)
  \param threshold          frazione minima del feromone
  \param max_best_counter   contatore per la disattivazione del deposito del feromone migliore
  \param max_local_counter  contatore per la disattivazione del deposito del feromone locale
  \param stop_time          durata dell'algoritmo in secondi
  \return                   la lunghezza della soluzione migliore trovata
 */
double ant_colony(
        unsigned int size,
        unsigned int *best_solution,
        int start_city,
        unsigned int ant_number,
        double alpha,
        double ro,
        double q0,
        double q0_min,
        double q0_step,
        double threshold,
        unsigned int max_best_counter,
        unsigned int max_local_counter,
        time_t stop_time = 180
);

#endif // ANTCOLONY_H
