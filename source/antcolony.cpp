/*
   antcolony.cpp

    Data di creazione:      31 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#include "antcolony.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstring>


using namespace std;

extern double best_length;

double **pheromone;     // matrice feromonica: ad ogni arco corrisponde una quantità di feromone
double t0;              // valore di tau zero

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
        time_t stop_time
){
    double        val              = 0.0;
    unsigned long iterations       = 0;
    unsigned int  start            = 0;
    double        best_length      = INT_MAX;   // lunghezza della soluzione migliore
    int           local_best_ant   = 0;         // migliore formica di ogni iterazione
    double        local_min_length = 0.0;       // lunghezza migliore di ogni iterazione

    unsigned int  local_solution[size];         // soluzione migliore di ogni iterazione (locale all'iterazione)

    double q0_effective = q0;   // q0 utilizzato realmente dalle formiche. Viene decrementato se necessario
    bool best_found = false;    // flag per l'individuazione

    /*
        questi parametri sono utilizzati per evitare che le formiche si fossilizzino sul feromone
        quando questi contatori raggiungono i valori max_best_counter e max_local_counter, le formiche
        smettono di depositare nuovo feromone, accelerando l'evaporazione e permettendo di trovare
        nuove, strane strade permettendo loro di giungere laddove nessuna formica è mai giunta prima.
     */
    unsigned int steady_best_counter=0;
    unsigned int steady_local_counter=0;

    if (start_city > -1) {
        start = start_city;
    } else {
        start = (int)(my_rand() * size);
    }

    /* costruzione della soluzione iiniziale e ottimizzazione */
    costruisci_tour(size, best_solution, start);
    opt2(size, best_solution);
    best_length = calcola_lunghezza_tour(size,best_solution);

    /* calcolo di tau zero */
    t0 = 1 / (best_length * size);

    if (debug){
        cout << "best length: " << best_length << endl;
        cout << "T0: " << t0 << endl;
    }

    //allocazione matrice feromonica ( o_O )
    pheromone = (double **)malloc(sizeof(double *) * size);
    for (unsigned int i=0; i<size; i++){
        pheromone[i] = (double *)malloc(sizeof(double) * size);
        for (unsigned int j=0; j<size; j++){
            if (i == j){
                pheromone[i][j] = 0;
            } else {
                pheromone[i][j] = t0;
            }
        }
    }

    // schiusa delle n mila formiche
    Ant formica[ant_number];

    do {
        // Posizionamento casuale delle formiche sulle città
        for (unsigned int i=0; i<ant_number; i++){
            formica[i].initialize(size, (int)(my_rand() * size));
        }

        for (unsigned int city=0; city<(size-1); city++){
            for (unsigned int ant=0; ant<ant_number; ant++){
                // Ogni formica costruisce una soluzione aggiungendo una città dopo l'altra
                formica[ant].generate_solution(pheromone, q0_effective, t0, ro);
            }
        }

        // calcolo della lunghezza della soluzione locale generata da ogni formica
        local_min_length = INT_MAX;
        best_found = false;
        for (unsigned int i=0; i<ant_number; i++)
        {
            formica[i].complete_tour(size);         // ottimizzazione della singola soluzione
            val = formica[i].take_tour_lenght();    // prelevamento della soluzione ottimizzata

            // ricerca della miglior soluzione locale
            if (val < local_min_length) {
                local_min_length = val;
                local_best_ant = i;
                steady_local_counter=0;
            } else {
                steady_local_counter++;     // conteggio delle formiche peggiori o uguali alla soluzione locale
            }
            if (debug) cout << val << "\t";

        }

        // Aggiornamento della miglior soluzione locale
        memcpy(local_solution, formica[local_best_ant].solution, sizeof(unsigned int)*size);

        if (debug) cout << q0_effective << "\t" << local_min_length << "\t" << best_length << endl;

        // Aggiornamento della miglior soluzione
        if(local_min_length < best_length)  {
            best_found = true;
            best_length = local_min_length;
            memcpy(best_solution, formica[local_best_ant].solution, sizeof(unsigned int)*size);
            steady_best_counter = 0;
        } else {
            steady_best_counter++;      // conteggio di quante volte la miglior soluzione è stata trovata
        }

        unsigned int r;
        unsigned int s=0;

        // tutto il feromone viene ridotto:
        //     t(r,s) <- (1-alpha)*t(r,s) + alpha * 0
        for (unsigned int i=0; i<size; i++){
            for (unsigned int j=i+1; j<size; j++){
                if (pheromone[i][j] < t0/threshold){
                    pheromone[i][j] = t0/threshold;
                } else {
                    pheromone[i][j] = (1-alpha) * pheromone[i][j];
                }
                pheromone[j][i] = pheromone[i][j];
            }
        }

        for (unsigned int i=0; i<size; i++) {
            /*
                Il feromone della soluzione migliore in assoluto viene incrementato se tale soluzione non è stata trovata
                per troppe volte (per evitare il sovraccarico di feromone).
             */
            if(steady_best_counter < max_best_counter) {
                r = best_solution[i];
                s = best_solution[(i+1) % size];
                pheromone[r][s] += alpha / best_length;
                pheromone[s][r]  = pheromone[r][s];
            }

            /*
                Il feromone della miglior soluzione locale viene incrementato se è differente dalla migliore soluzione
                (per evitare il sovraccarico di feromone) e se tale soluzione è stata trovata entro il limite imposto
                da max_local_counter. Se le formiche non trovano soluzioni migliori entro max_local_counter allora non
                depositano ulteriore feromone.
                Il deposito di questo ulteriore feromone aiuta la ricerca locale.
             */
            if((local_min_length != best_length) && (steady_local_counter < max_local_counter)) {
                r = local_solution[i];
                s = local_solution[(i+1) % size];
                pheromone[r][s] += alpha / local_min_length;
                pheromone[s][r]  = pheromone[r][s];
            }
        }

        if(best_found) {
            q0_effective = q0;              // se è stata trovata una soluzione migliore q0 viene mantenuto tale...
        } else {
            if(q0_effective>q0_min) {
                q0_effective -= q0_step;    // ... altrimenti viene decrementato
            }
        }

        if (debug) iterations++;
    } while (time(0) < stop_time);

    if (debug) cout << "iterations:\t" << iterations << endl;

    return best_length;
}


