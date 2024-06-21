#ifndef BCH_CODEC_H
#define BCH_CODEC_H

#include "Polynomial.h"
#include <Arduino.h>

// Function prototypes
void encoder_15_7(int* msg, int* codeword);
void decode_15_7(int* codeword, int* msg);
void err_poly(int* codeword, int* err_coeff);

// Encoder function
void encoder_15_7(int* msg, int* codeword) {
    int temp_coeff[9] = {0};
    temp_coeff[0] = 1;
    Polynomial temp(temp_coeff, 9);
    Polynomial poly_msg(msg, 7);
    int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1}; // g(x) coefficients
    Polynomial g_x(g_x_coeff, 9);
    
    Polynomial r_x = (temp * poly_msg) % g_x;
    Polynomial c_x = (temp * poly_msg) + r_x; // Codeword polynomial
    
    int* c_x_coeff = c_x.getCoefficients();
    for (int i = 0; i < 15; i++) {
        codeword[i] = c_x_coeff[i];
        Serial.print(codeword[i]);
        Serial.print(" ");
    }
    Serial.println();
}

// Error polynomial function
void err_poly(int* codeword, int* err_coeff) {
    for(int i = 0; i<15; i++){
    Serial.print(codeword[i]);
  }
  Serial.println();
  Serial.println();
    Polynomial w_x(codeword, 15);
    Serial.println("codeword with errors : " + w_x.toString());
    int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1};
    Polynomial g_x(g_x_coeff, 9);
    Polynomial s_x = w_x % g_x;

    Serial.println("s_x: " + s_x.toString());
    Serial.print("weight: ");
    Serial.println(s_x.weight());

    int* s_j_x_coeff = nullptr;
    int s_j_x_degree;
    int j = 0;
    for (int i = 0; i < 50; i++) {
        int* temp_coeff = (int*) calloc(i + 1, sizeof(int));
        temp_coeff[0] = 1;
        Polynomial temp(temp_coeff, i + 1);
        free(temp_coeff);

        Polynomial s_i_x = (s_x * temp) % g_x;
        Serial.println("s_i_x: " + s_i_x.toString());
        Serial.print("weight: ");
        Serial.println(s_i_x.weight());

        if (s_i_x.weight() <= 2) {
            j = i;
            s_j_x_coeff = s_i_x.getCoefficients();
            s_j_x_degree = s_i_x.degree();
            Serial.println("s_j_x: " + s_i_x.toString());
            break;
        }
    }

    if (s_j_x_coeff == nullptr) {
        Serial.println("Failed to find a valid syndrome with weight <= 2");
        return;
    }

    int n = 15;
    int* temp_coeff2 = (int*) calloc(n - j + 1, sizeof(int));
    temp_coeff2[0] = 1;
    Polynomial temp(temp_coeff2, n - j + 1);
    free(temp_coeff2);

    int mod_poly_coeff[16] = {0};
    mod_poly_coeff[0] = 1;
    mod_poly_coeff[15] = 1;
    Polynomial mod_poly(mod_poly_coeff, 16);

    Polynomial s_j_x(s_j_x_coeff, s_j_x_degree);
    Polynomial e_x = (temp * s_j_x)%mod_poly;

    int* e_x_coeff = e_x.getCoefficients();
    // for (int i = 0; i < 15; i++) {
    //     err_coeff[i] = e_x_coeff[i];
    //     Serial.print(err_coeff[i]);
    //     Serial.print(" ");
    // }
    Serial.println();
    Serial.println("e_x: " + e_x.toString());
}

// Decoder function
void decode_15_7(int* codeword, int* msg) {
    int err_coeff[15] = {0};
    err_poly(codeword, err_coeff);

    Polynomial w_x(codeword, 15);
    Polynomial e_x(err_coeff, 15);

    Polynomial corrected_code = w_x + e_x;
    int* corrected_coeff = corrected_code.getCoefficients();

    // Assuming the original message length is 7
    for (int i = 0; i < 7; i++) {
        msg[i] = corrected_coeff[i + 8]; // Adjust index to get the message part
    }

    Serial.println("Corrected message:");
    for (int i = 0; i < 7; i++) {
        Serial.print(msg[i]);
        Serial.print(" ");
    }
    Serial.println();
}

#endif // BCH_CODEC_H
