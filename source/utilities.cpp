/*
   utilities.cpp

    Data di creazione:      14 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#include "utilities.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <sys/time.h>


using namespace std;

double calcola_distanza(double inizio_x, double inizio_y, double fine_x, double fine_y)
{
    double x = (fine_x-inizio_x);
    double y = (fine_y-inizio_y);
    return rint(sqrt(x*x + y*y));
}

double calcola_lunghezza_tour(unsigned int size, unsigned int *tour){
    double totale_distanza = 0;
    unsigned int i = 0;

    for(; i<(size-1); i++)
        totale_distanza += distances[tour[i]][tour[i+1]];

    totale_distanza += distances[tour[i]][tour[0]];

    return totale_distanza;
}

bool controlla_tour(unsigned int size, unsigned int *tour) {

    int sum=0;
    for (unsigned int i=0; i<size; i++){
        sum += tour[i] +1;
    }
    /*  somma di una progressione aritmetica: se ogni città compare una volta sola
        allora la loro somma equivale alla somma di tutte gli indici delle città in ordine crescente */
    int expected = ((size * (size+1))/2);

    if (sum == expected ){
        return true;
    } else {
        return false;
    }
}

void salva_tour_su_file(string directory, string nome, string dimensione, string algoritmo, int size, int seed, unsigned int *tour, double tour_lenght){
    stringstream ss;
    ss << directory << nome << ".tour";
    string path = ss.str();

    ofstream file;
    file.open(path.c_str());

    if (file.is_open()) {
        file << "NAME: " << nome << ".tour" << endl;
        file << "LENGTH: " << tour_lenght << endl;
        file << "TYPE: TOUR" << endl;
        file << "DIMENSION: " << dimensione << endl;
        file << "SEED: " << seed << endl;
        file << "ALGORITHM: " << algoritmo << endl;
        file << "TOUR_SECTION" << endl;

        for (int i=0; i<size; i++) {
            file << (tour[i] + 1) << endl;
        }
        file << "-1" << endl;
        file << "EOF" << endl;
        file.flush();
        file.close();
    }
}

void mostra_tour(unsigned int size, unsigned int *tour){
    for (unsigned int i=0; i<size; i++){
        printf("[%3d] %3d ",i , tour[i]);
        if (i % 10 == 0)
            cout << endl;
    }
    cout << endl;
}

void sort2(unsigned int *p0, unsigned int *p1){
    unsigned int temp = 0;
    if (*p0 > *p1){
        temp = *p1;
        *p1 = *p0;
        *p0 = temp;
    }
}

void sort3(unsigned int *p0, unsigned int *p1, unsigned int *p2){
    sort2(p0,p1);
    sort2(p1,p2);
    sort2(p0,p1);
}

void sort4(unsigned int *p0, unsigned int *p1, unsigned int *p2, unsigned int *p3){
    sort2(p0,p1);
    sort2(p2,p3);
    sort2(p0,p2);
    sort2(p1,p3);
    sort2(p1,p2);
}
