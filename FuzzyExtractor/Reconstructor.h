#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include <BCH_general.h>

//function prototypes
int* correct_data(int* PUF_, int* helper_data, int n);
int* reconstructor(int* PUF_corrected);//key reconstruction using hashing

//function definition
int* correct_data(int* PUF_, int* helper_data, int n){
    int* xor_puf_hd = new int[15*n];
    for(int i = 0; i<15*n; i++){
        xor_puf_hd[i] = PUF_[i]^helper_data[i];
    }
    //correct errors using BCH decoder
    int* dec_out = decode(xor_puf_hd, n);
    //now encode data to get initial rng
    int* enc_out = encode(dec_out, n);
    //get corrected puf response
    int* PUF_corrected = new int[15*n];
    for(int i = 0; i<15*n; i++){
        PUF_corrected[i] = enc_out[i]^helper_data[i];
    }
    return PUF_corrected;
}

#endif