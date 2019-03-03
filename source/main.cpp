/*
   main.cpp

    Data di creazione:      2 marzo 2013
               Autore:      Claudio "Dna" Bonesana
               Titolo:      Traveling Salesman Problem
                            13a Coppa di Algoritmi
            Versione:      10
          Descrizione:
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <limits>
#include <ctime>

#include <unistd.h>
#include <signal.h>

#include <vector>

#include "utilities.h"
#include "antcolony.h"

#define NAME 0
#define DIMENSION 3
#define BEST_KNOW 5

#define P_NAME 0
#define P_DIME 1
#define P_BEST 2
#define P_ALG0 3


using namespace std;

bool          debug = false;    // flag per la modalità debug
int           start_city = -1;  // città di partenza per algoritmi costruttivi. -1 equivale a random
unsigned int  seed = time(0);   // seed usato per il random

int    *city_id;                // array degli indici delle città
double *city_x;                 // array delle coordinate x delle città
double *city_y;                 // array delle coordinate y delle città
bool   *city_visited;           // array dello stato visitato oppure no delle città

unsigned int cities_size = 0;   // numero di città

double **distances;             // matrice delel distanze

unsigned int *tour;             // soluzione
double best_length = 0;         // lunghezza soluzione

/* parametri per il timer e il tempo di esecuzione */
struct sigaction act;
sigset_t         set;
time_t inizio;
time_t fine;
time_t termine;

vector<string> parameters;         // vettore dei parametri letti dall'header
string output_dir     = "";        // path per l'output dei dati (file .tour)
string algoritmo_nome = "Ant Colony System";

/**
    Funzione per il timer. Allo scoccare del tempo prestabilito, questa funzione viene attivata.
    Salva il tour su file, stampa a schermo gli ultimi dati e chiude il programma.
 */
void timer_interrupt(int ignored);

/**
    Programma principale
 */
int main(int argc, char *argv[]){
    int   arg;                  // argomento
    char *in_name = 0;          // nome del file in input
    int   timing = 180;         // durata massima in caso di tempo non assegnato

    /* Parametri di Ant Colony System */
    int ants       = 4;
    double alpha   = 0.1;
    double ro      = 0.1;
    double q0      = 0.95;
    double q0_min  = 0.4;
    double q0_step = 0.005;

    unsigned int max_best_counter  = 30;
    unsigned int max_local_counter = 100;


    /* Recupero dei parametri da linea di comando. */
    while ( (arg = getopt(argc, argv, "o:c:s:d:Dht:F:A:R:Q:q:S:B:L:")) != -1){
        switch(arg){
        case 'o':           // Path per output
            output_dir = optarg;
            break;
        case 'c':           // Città di partenza
            start_city = atoi(optarg) -1;
            break;
        case 's':           // Random seed
            seed = atol(optarg);
            break;
        case 'd':           // File di dati con la mappa
            in_name = optarg;
            break;
        case 'D':           // DEBUG: stampa a schermo di dati
            debug = true;
            break;
        case 'F':           // Numero formiche
            ants = atoi(optarg);
            break;
        case 'A':           // Parametro alpha
            alpha = atof(optarg);
            break;
        case 'R':           // Parametro rho
            ro = atof(optarg);
            break;
        case 'Q':           // Parametro q0
            q0 = atof(optarg);
            break;
        case 'q':           // Parametro q0 min
            q0_min = atof(optarg);
            break;
        case 'S':           // Parametro q0 step
            q0_step = atof(optarg);
            break;
        case 'B':           // Parametro massimo di best_counter
            max_best_counter = atoi(optarg);
            break;
        case 'L':           // Parametro massimo di local counter
            max_local_counter = atoi(optarg);
            break;
        case 'h':           // help
            cout << "Parametri" << endl <<
                    "    -d       Lista delle città su cui l'algoritmo andrà a lavorare." << endl <<
                    "    -c     * Città di partenza utilizzato dall'algoritmo Nearest Neighbour con 2-opt." << endl <<
                    "    -s     * Seed utilizzato dagli algoritmi con parti meta euristiche."<< endl <<
                    "    -D     * Abilita la modalità debug con la stampa a schermo di dati e informazioni aggiuntive." << endl <<
                    "    -h     * Mostra questo help." << endl <<
                    "    -o     * Imposta l'output del file .tour." << endl <<
                    "    -t     * Imposta la durata massima dell'algoritmo" << endl << endl <<

                    "    -F     * Numero intero di formiche" << endl <<
                    "    -A     * Parametro double Alpha"    << endl <<
                    "    -R     * Parametro double Ro"       << endl <<
                    "    -Q     * Parametro double Q0"       << endl <<
                    "    -q     * Parametro double Q0_min"   << endl <<
                    "    -S     * Parametro double Q0_Step"  << endl <<
                    "    -B     * Parametro intero Max_best_counter " << endl <<
                    "    -L     * Parametro intero Max_local_counter" <<  endl <<
                    "I parametri contrassegnati da * sono opzionali." << endl << endl;
            exit(0);
        case 't':           // durata massima
            timing = atoi(optarg);
            break;
        case '?':
            cout << "Parametro non valido." << endl;
            return 4;
        default:
            break;
        }
    }

    /* Se non è stato scelto nessun file */
    if (in_name == 0){
        cout << "Nessuna lista di città è stata data in pasto al programma." << endl;
        exit(8);
    }

    /* Grande e potente Random, fa che esca qualcosa di buono... */
    srandom(seed);

    /* Preparazione del timer */
    act.sa_handler = timer_interrupt;
    if (sigaction(SIGALRM,&act,0) != 0){
        cout << "Errore con SIGARLM." << endl;
        exit(48);
    }

    sigemptyset(&set);
    alarm(timing);

    /* Inizio del conteggio del tempo trascorso. */
    inizio = time(0);
    termine = inizio + timing + 1; // margine di errore

    /* Preparazione della matrice iniziale. */
    ifstream myfile;            // file dei dati
    string line;                // stringa temporanea

    /* Lettura dei dati da file */
    myfile.open (in_name,ios::in);
    if (myfile.is_open()){
        for (unsigned int i=0; i<7; i++){
            getline(myfile,line);
            if (i == NAME || i == DIMENSION || i == BEST_KNOW){
                parameters.push_back(strchr((char *)line.c_str(),':')+2);
            }
        }

        cities_size  = atoi(parameters[P_DIME].c_str());
        city_id      = (int *)malloc(sizeof(int)    * cities_size);
        city_x       = (double *)malloc(sizeof(double) * cities_size);
        city_y       = (double *)malloc(sizeof(double) * cities_size);
        city_visited = (bool *)malloc(sizeof(bool)   * cities_size);
        tour         = (unsigned int *)malloc(sizeof(unsigned int) * cities_size);

        for (unsigned int i=0; i<cities_size; i++){
            myfile >> city_id[i] >> city_x[i] >> city_y[i]; // lettura di una città
            --city_id[i];                                       // aggiustamento dell'indice perché gli array iniziano da zero ma le città da 1
            city_visited[i] = false;                            // la citta' non è mai stata visitata
        }
        myfile.close();
    } else {
        cout << "Il file " << argv[1] << " non è stato aperto correttamente!" << endl;
        exit(16);
    }

    parameters.push_back(algoritmo_nome);

    if (start_city > (int)cities_size && start_city != -1){
        cout << "La città scelta ha un indice superiore al numero di città presenti nel tour." << endl;
        exit(32);
    }

    /* Creazione della matrice delle distanze */
    distances = (double **)malloc(sizeof(double *) * cities_size);    // allocazione dei puntatori per i vettori di valori

    /* La matrice viene riempita completamente */
    for (int i=cities_size-1; i>=0; i--){

        distances[i] = (double *)malloc(sizeof(double) * cities_size);         // allocazione del vettore di valori
        for (unsigned int j=0; j<cities_size; j++){
            // calcolo della distanza tra le varie città
            distances[i][j] = calcola_distanza(city_x[i], city_y[i], city_x[j], city_y[j]);
        }
    }

    cout << "Computazione in corso... ";
    cout.flush();

    /* Utilizzo di Ant Colony System */
    ant_colony(cities_size, tour, start_city, ants, alpha, ro, q0, q0_min, q0_step, 10.0, max_best_counter, max_local_counter,termine);

    return 0;
}

void timer_interrupt(int ignored){

    time_t fine = time(0);

    cout << "fatto!" << endl << endl;
    cout << "Tour:                    " << parameters[P_NAME] << endl;
    cout << "Algoritmo utilizzato:    " << algoritmo_nome << endl;
    cout << "Città iniziale:          " << (start_city + 1) << endl;
    cout << "Distanza minima trovata: " << best_length << endl <<
            "SEED utilizzato:         " << seed << endl <<
            "Tempo trascorso:         " << (fine-inizio) << " secondi" << endl;

    /* controllo sulla correttezza */
    if(controlla_tour(cities_size, tour)){
        cout << "Soluzione valida" << endl;
    } else {
        cout << "Soluzione NON valida" << endl;
    }

    salva_tour_su_file(output_dir,parameters[P_NAME],parameters[P_DIME],parameters[P_ALG0],cities_size,seed,tour,best_length);

    exit(0);
}
