/*
   utilities.h

    Data di creazione:      14 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <cstdlib>


using namespace std;

extern double **distances;  // matrice delle distanze
extern int    *city_id;     // array dell'id delle città


/**
  Funzione per il calcolo della distanza tra due città.
  \param inizio_x   coordinata x della città iniziale
  \param inizio_y   coordinata y della città iniziale
  \param fine_x     coordinata x della città finale
  \param fine_y     coordinata x della città finale
  \return           la distanza tra le due città arrotondata all'intero
  */
double calcola_distanza(double inizio_x, double inizio_y, double fine_x, double fine_y);

/**
  Funzione per il calcolo della lunghezza totale di un tour. Il tour non deve comprendere la ripetizione del nodo iniziale alla
  fine in quanto questa funzione si preoccupa già di contare anche il collegamento tra l'ultimo e il primo nodo.
  \param tour       il tour da percorrere
  \param size       dimensione del tour
  \return           la lunghezza totale del tour
  */
double calcola_lunghezza_tour(unsigned int size, unsigned int *tour);

/**
  Funzione per la verifica della correttezza di un tour. Un tour è corretto quando passa da tutte le città una volta sola.
  \param size       numero di città presenti nella mappa
  \param tour       il tour da controllare.
  \return           true se il controllo è stato superato, altrimenti false.
  */
bool controlla_tour(unsigned int size, unsigned int *tour);

/**
  Funzione per la creazione di un file contenente il tour.
  \param directory  directory in cui andrà salvato il file
  \param nome       nome del tour
  \param dimensione numero di città che compone il tour
  \param algoritmo  nome dell'algoritmo utilizzato per la creazione del tour
  \param size       numero di città presenti nella mappa
  \param seed       seed utilizzato per il random
  \param tour       il tour da salvare
  \param tour_lenght lunghezza del tour
  */
void salva_tour_su_file(string directory, string nome, string dimensione, string algoritmo, int size, int seed, unsigned int *tour, double tour_lenght);

/**
  Funzione per la stampa a schermo del tour.
  \param toru il tour da mostrare
  */
void mostra_tour(unsigned int size, unsigned int *tour);

/**
  Sorting newtork a 2 vie: ordina due elementi per grandezza.
  Codice adattato da http://stackoverflow.com/a/2786959
  \param i primo elemento
  \param j secondo elemento
  */
void sort2(unsigned int *p0, unsigned int *p1);

/**
  Sorting newtork a 3 vie: ordina tre elementi per grandezza.
  Codice adattato da http://stackoverflow.com/a/2786959
  \param i primo elemento
  \param j secondo elemento
  \param k terzo elemento
  */
void sort3(unsigned int *p0, unsigned int *p1, unsigned int *p2);

/**
  Sorting newtork a 4 vie: ordina quattro elementi per grandezza.
  Codice adattato da http://stackoverflow.com/a/2786959
  \param i primo elemento
  \param j secondo elemento
  \param k secondo elemento
  \param l secondo elemento
  */
void sort4(unsigned int *i, unsigned int *j, unsigned int *k, unsigned int *l);

/**
  Ridefinizione della funzione rand()
  */
#define my_rand() ((double)rand()/(double)RAND_MAX)

#endif // UTILITIES_H
