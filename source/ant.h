/*
   ant.h

    Data di creazione:      31 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#ifndef ANT_H
#define ANT_H

#include "utilities.h"

#include <cstdlib>


using namespace std;

extern int    *city_id;
extern double **distances;

/**
    Classe per la definizione delle formiche e del loro operato.
 */
class Ant {
private:
    string name;

    char *map;                  // mappa locale delle città visitate
    unsigned int map_size;      // numero di città nella mappa

    unsigned int p_current;     // città sulla quale la formica è attualmente posizionata
    unsigned int current;       // nodo attuale
    unsigned int next;          // prossimo nodo

    double tour_length;         // lunghezza attuale della soluzione

    /**
        Seleziona probabilisticamente la prossima città in base al meccanismo di Exploration e Exploitation.
        \param pheromone    matrice feromonica
        \param q0           parametro per il funzionamento del meccanismo
     */
    void visit_city(double **pheromone, double q0);

    /**
        Applica l'aggiornamento locale del feromone.
        \param pheromone    matrice feromonica
        \param t0           parametro tau zero
        \param ro           parametro per il deposito del feromone
     */
    void update_trail(double **pheromone, double t0, double ro);

public:
    unsigned int *solution;     // vettore della soluzione

    /**
        Costruttore.
     */
    Ant();

    /**
        Inizializza la formica per la costruzione di una nuova soluzione
        \param size         numero di città nella mappa
        \param start_city   città iniziale su cui posizionarsi
     */
    void initialize(unsigned int size, unsigned int start_city);

    /**
        Sposta la formica sulla prossima città seguendo il meccanismo di Exploration e Explotation.
        \param pheromone    matrice feromonica
        \param q0           parametro per il funzionamento del meccanismo
        \param t0           parametro tau zero
        \param ro           parametro per il deposito del feromone
     */
    void generate_solution(double **pheromone, double q0, double t0, double ro);

    /**
        Ottimizza il tour con un 2-opt.
     */
    void complete_tour(unsigned int size);

    void stampa_percorso();

    /**
        \return             la lunghezza della soluzione trovata
     */
    double take_tour_lenght();

    /**
        \return             la città attuale
     */
    int get_current_city();

    //unsigned int *get_solution();

};

#endif // ANT_H

