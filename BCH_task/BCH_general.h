#ifndef BCH_GENERAL_H
#define BCH_GENERAL_H

#include <BCHCodec.h>

//function prototypes
void fill(int* codeword_i, int* codeword, int i, int n);//n will define length of codeword: 1 for 15, 2 for 30, ...19 for 285
void extract(int* codeword_i, int* codeword, int i, int n);//...
int* encode(int* msg, int n);//...
int* decode(int* codeword, int n);//...

//finction definitions
void fill(int* codeword_i, int* codeword, int i, int n){
    for(int j = 0; j<=6; j++){
        codeword[7*i+j] = codeword_i[j];
    }
    for(int j = 0; j<=7; j++){
        codeword[8*i+j+7*n] = codeword_i[7+j];
    }
}

void extract(int* codeword_i, int* codeword, int i, int n){
    for(int j = 0; j<=6; j++){
        codeword_i[j] = codeword[7*i+j];
    }
    for(int j = 0; j<=7; j++){
        codeword_i[j+7] = codeword[8*i+j+7*n];
    }
}

int* encode(int* msg, int n){
    int* codeword = new int[n*15];
    for(int i = 0; i<n; i++){
        int* codeword_i = new int[15];
        int* msg_i = new int[7];
        for(int j = 0; j<=6; j++){
            msg_i[j] = msg[7*i+j];
        }
        encoder_15_7(msg_i, codeword_i);
        fill(codeword_i, codeword, i, n);
    }
    return codeword;
}

int* decode(int* codeword, int n){
    int* msg = new int[7*n];
    for(int i = 0; i<n; i++){
        int* codeword_i = new int[15];
        int* msg_i = new int[7];
        extract(codeword_i, codeword, i, n);
        decode_15_7(codeword_i, msg_i);
        for(int j = 0; j<=6; j++){
            msg[7*i+j] = msg_i[j];
        }
    }
    return msg;
}

#endif //BCH_GENERAL_H