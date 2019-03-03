/*
   opt.cpp

    Data di creazione:      21 marzo 2013
               Autore:      Claudio "Dna" Bonesana
 */

#include "opt.h"

#include "utilities.h"

#include <iostream>
#include <cstdlib>
#include <cstring>


using namespace std;

void opt2_exchange(unsigned int size, unsigned int i, unsigned int j, unsigned int *tour){

    register unsigned int b;
    register unsigned int c;
    register unsigned int temp;

    if(i<j){
        b = (i+1) % size;
        c = j;
    } else{
        b = (j+1) % size;
        c = i;
    }

    for (unsigned int n=0; n<(c-b+1)/2; n++){
        temp = tour[b+n];
        tour[b+n] = tour[c-n];
        tour[c-n] = temp;
    }
}

void opt2(unsigned int size, unsigned int *tour){
    double best_gain = 0;
    double gain = 0;

    do{
        best_gain = 0;
        unsigned int best_i=0;
        unsigned int best_j=0;

        for (unsigned int i=0; i<size; i++){
            for (unsigned int j=i+2; j<size; j++){
                gain = opt2_compute_gain(size,i,j,tour);
                if (gain < best_gain){
                    best_gain = gain;
                    best_i=i;
                    best_j=j;
                }
            }
        }
        if (best_gain < 0.0){
            opt2_exchange(size, best_i, best_j, tour);
        }
    }
    while (best_gain < 0.0);
}
