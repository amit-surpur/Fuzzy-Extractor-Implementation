#ifndef GENERATOR_H
#define GENERATOR_H

#include <BCH_general.h>

//function prototypes
int* helper_gen(int* PUF, int* key, int n);
int* generator(int* key);//use hashing method to generate key

//function definitions
int* helper_gen(int* PUF, int* key, int n){
    int* rng = new int[7*n];
    for(int i = 0; i<7*n; i++){
        rng[i] = rand()%2;
    }
    int* enc_rng = encode(rng, n);
    int* helper_data = new int[15*n];
    for(int i = 0; i<15*n; i++){
        helper_data[i] = enc_rng[i]^PUF[i];
    }
    return helper_data;
}

#endif