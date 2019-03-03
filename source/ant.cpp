/*
   ant.cpp

    Data di creazione:      31 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#include "ant.h"

#include "opt.h"

#include <cmath>
#include <iostream>


using namespace std;

Ant::Ant()
{
}

void Ant::initialize(unsigned int size, unsigned int start_city){
    // pulizia della soluzione precedente
    map_size = size;
    map = (char *)calloc(map_size, sizeof(char));
    solution = (unsigned int *)malloc(sizeof(unsigned int) * size);
    tour_length = 0;
    current = start_city;
    p_current = 0;

    // aggiunta della prima città
    solution[0] = city_id[current];
    map[current] = 1;
    p_current++;
}

void Ant::visit_city(double **pheromone, double q0){

    double q = (double)my_rand();
    unsigned int r = current;
    unsigned int chosen_city = -2;

    // Calcolo del valore massimo
    unsigned int city_max = -1;
    double max = 0;
    unsigned int available_cities = 0;

    for (unsigned int i=0; i<map_size; i++){
        if (map[i] == 0){
            available_cities++;
        }
        if ((map[i] == 0) && (i != r)){
            double val = pheromone[r][i] / (distances[r][i] * distances[r][i]); /* pow((1/distances[r][i]), beta);*/
            if (val > max){
                city_max = i;
                max = val;
            }
        }
    }

    if (available_cities == 1){
        chosen_city = city_max;
    } else {

        // Meccanismo di Explotation/Exploration
        if (q <= q0){
            // Exploitation

            chosen_city = city_max;

        } else {
            // Exploration

            double probability[map_size];

            // calcolo della sommatoria dei nodi disponibili
            double sum = 0;

            // calcolo della probabilità di ogni nodo
            for (unsigned int i=0; i<map_size; i++){
                if ((map[i] == 0) && (i != r) /*&& (i != city_max)*/){
                    probability[i] = pheromone[r][i] / (distances[r][i] * distances[r][i]); /* pow((1/distances[r][i]), beta);*/
                    sum += probability[i];
                } else {
                    probability[i] = 0;
                }
            }

            // scelta del nodo più probabile
            double prob = (double)(my_rand() * sum);
            double prob_sum = 0;

            int city_to_explore = map_size-1;
            for (unsigned int i=0; i<map_size; i++){
                prob_sum+=probability[i];

                if (prob < prob_sum){
                    city_to_explore = i;
                    break;
                }
            }

            chosen_city = city_to_explore;
        }
    }

    map[chosen_city] = 1;
    next = chosen_city;
    solution[p_current] = city_id[next];
    p_current++;
}

void Ant::update_trail(double **pheromone, double t0, double ro){
    
}

void Ant::generate_solution(double **pheromone, double q0, double t0, double ro){
    visit_city(pheromone,q0);
    update_trail(pheromone,t0,ro);
}

void Ant::complete_tour(unsigned int size){
    opt2(size,solution);
    tour_length = calcola_lunghezza_tour(size, solution);
}

void Ant::stampa_percorso(){
    cout << "Percorso attuale: ";
    for (unsigned int i=0; i<map_size; i++){
        cout << solution[i] << ";";
    }
    cout << endl;
}

double Ant::take_tour_lenght(){
    return tour_length;
}

int Ant::get_current_city() {
    return current;
}
