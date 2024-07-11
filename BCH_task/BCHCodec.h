#ifndef BCH_CODEC_H
#define BCH_CODEC_H
#include <Polynomial.h>
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
        // Serial.print(codeword[i]);
        // Serial.print(" ");
    }
    // Serial.println();
}

String err_poly(int* codeword){
    Polynomial w_x(codeword, 15);
    int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1};
    Polynomial g_x(g_x_coeff, 9);
    Polynomial s_x = w_x % g_x;
    String poly;
    int j = 0;
    for (int i = 0; i < 100; i++) {
        int* temp_coeff = (int*) calloc(i + 1, sizeof(int));
        temp_coeff[0] = 1;
        Polynomial temp(temp_coeff, i + 1);
        free(temp_coeff);

        Polynomial s_i_x = (s_x * temp) % g_x;
        //Serial.println("s_i_x: " + s_i_x.toString());
        // Serial.print("weight: ");
        // Serial.println(s_i_x.weight());

        if (s_i_x.weight() <= 2) {
            j = i;
            //Serial.println("s_j_x: " + s_i_x.toString());
            poly = s_i_x.toString();
            break;
        }
    }
    //Serial.println("s_j_x out of loop: "+poly);
    // Construct s_j_x polynomial from string representation
    Polynomial s_j_x = Polynomial::fromString(poly);
    Serial.println("s_j_x: "+ s_j_x.toString());
    int n = 15;
    int* temp_coeff2 = (int*) calloc(n - j + 1, sizeof(int));
    temp_coeff2[0] = 1;
    Polynomial temp(temp_coeff2, n - j + 1);
    free(temp_coeff2);

    int mod_poly_coeff[16] = {0};
    mod_poly_coeff[0] = 1;
    mod_poly_coeff[15] = 1;
    Polynomial mod_poly(mod_poly_coeff, 16);

    Polynomial e_x = (temp * s_j_x)%mod_poly;
    //Serial.println("s_j_x: "+ s_j_x.toString());
    String err_p = e_x.toString();
    Serial.println("e_x_poly: " + err_p);
    return err_p;
}
// Decoder function
void decode_15_7(int* codeword, int* msg) {
    String e_x_poly = err_poly(codeword);
    Serial.println("e_x1: "+e_x_poly);
    Polynomial w_x(codeword, 15);
    Polynomial e_x = Polynomial::fromString(e_x_poly);
    Serial.println("e_x: "+e_x.toString());
    Polynomial corrected_code = w_x + e_x;
    const int* corrected_coeff = corrected_code.getCoefficients();

    // Assuming the original message length is 7
    for (int i = 0; i < 7; i++) {
        msg[i] = corrected_coeff[i]; // Adjust index to get the message part
    }
}
#endif
// #ifndef BCH_CODEC_H
// #define BCH_CODEC_H

// #include "Polynomial.h"
// #include <Arduino.h>

// // Function prototypes
// void encoder_15_7(int* msg, int* codeword);
// void decode_15_7(int* codeword, int* msg);
// void err_poly(int* codeword, int* err_coeff);

// // Encoder function
// void encoder_15_7(int* msg, int* codeword) {
//     int temp_coeff[9] = {0};
//     temp_coeff[0] = 1;
//     Polynomial temp(temp_coeff, 9);
//     Polynomial poly_msg(msg, 7);
//     int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1}; // g(x) coefficients
//     Polynomial g_x(g_x_coeff, 9);
    
//     Polynomial r_x = (temp * poly_msg) % g_x;
//     Polynomial c_x = (temp * poly_msg) + r_x; // Codeword polynomial
    
//     int* c_x_coeff = c_x.getCoefficients();
//     for (int i = 0; i < 15; i++) {
//         codeword[i] = c_x_coeff[i];
//         // Serial.print(codeword[i]);
//         // Serial.print(" ");
//     }
//     // Serial.println();
// }

// // Error polynomial function
// void err_poly(int* codeword, int* err_coeff) {
// //     for(int i = 0; i<15; i++){
// //     Serial.print(codeword[i]);
// //   }
// //   Serial.println();
// //   Serial.println();
//     Polynomial w_x(codeword, 15);
//     // Serial.println("codeword with errors : " + w_x.toString());
//     int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1};
//     Polynomial g_x(g_x_coeff, 9);
//     Polynomial s_x = w_x % g_x;

//     Serial.println("s_x: " + s_x.toString());
//     // Serial.print("weight: ");
//     // Serial.println(s_x.weight());

//     int* s_j_x_coeff;
//     int s_j_x_degree;
//     int j = 0;
//     for (int i = 0; i < 100; i++) {
//         int* temp_coeff = (int*) calloc(i + 1, sizeof(int));
//         temp_coeff[0] = 1;
//         Polynomial temp(temp_coeff, i + 1);
//         free(temp_coeff);

//         Polynomial s_i_x = (s_x * temp) % g_x;
//         Serial.println("s_i_x: " + s_i_x.toString());
//         // Serial.print("weight: ");
//         // Serial.println(s_i_x.weight());

//         if (s_i_x.weight() <= 2) {
//             j = i;
//             s_j_x_coeff = s_i_x.getCoefficients();
//             s_j_x_degree = s_i_x.degree();
//             Serial.println("s_j_x: " + s_i_x.toString());
//             break;
//         }
//     }

//     if (s_j_x_coeff == nullptr) {
//         Serial.println("Failed to find a valid syndrome with weight <= 2");
//         return;
//     }
//     //Serial.println("s_i_x loop: "+ s_i_x.toString());
//     int n = 15;
//     int* temp_coeff2 = (int*) calloc(n - j + 1, sizeof(int));
//     temp_coeff2[0] = 1;
//     Polynomial temp(temp_coeff2, n - j + 1);
//     free(temp_coeff2);

//     int mod_poly_coeff[16] = {0};
//     mod_poly_coeff[0] = 1;
//     mod_poly_coeff[15] = 1;
//     Polynomial mod_poly(mod_poly_coeff, 16);

//     Polynomial s_j_x(s_j_x_coeff, s_j_x_degree);
//     Polynomial e_x = (temp * s_j_x)%mod_poly;
//     Serial.println("s_j_x: "+ s_j_x.toString());
//     int* e_x_coeff = e_x.getCoefficients();
//     // for (int i = 0; i < 15; i++) {
//     //     err_coeff[i] = e_x_coeff[i];
//     //     Serial.print(err_coeff[i]);
//     //     Serial.print(" ");
//     // }
//     // Serial.println();
//     Serial.println("e_x: " + e_x.toString());
// }

// // Decoder function
// void decode_15_7(int* codeword, int* msg) {
//     int err_coeff[15] = {0};
//     err_poly(codeword, err_coeff);

//     Polynomial w_x(codeword, 15);
//     Polynomial e_x(err_coeff, 15);

//     Polynomial corrected_code = w_x + e_x;
//     int* corrected_coeff = corrected_code.getCoefficients();

//     // Assuming the original message length is 7
//     for (int i = 0; i < 7; i++) {
//         msg[i] = corrected_coeff[i]; // Adjust index to get the message part
//         Serial.print(msg[i]);
//     }
//     Serial.println();
    
//     Serial.println("Corrected message:");
//     for (int i = 0; i < 7; i++) {
//         Serial.print(msg[i]);
//         Serial.print(" ");
//     }
//     Serial.println();
// }

// #endif // BCH_CODEC_H
// // #ifndef BCH_CODEC_H
// // #define BCH_CODEC_H

// // #include "Polynomial.h"
// // #include <Arduino.h>

// // // Function prototypes
// // void encoder_15_7(int* msg, int* codeword);
// // void decode_15_7(int* codeword, int* msg);
// // void err_poly(int* codeword, int* err_coeff);

// // // Encoder function
// // void encoder_15_7(int* msg, int* codeword) {
// //     int temp_coeff[9] = {0};
// //     temp_coeff[0] = 1;
// //     Polynomial temp(temp_coeff, 9);
// //     Polynomial poly_msg(msg, 7);
// //     int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1}; // g(x) coefficients
// //     Polynomial g_x(g_x_coeff, 9);
    
// //     Polynomial r_x = (temp * poly_msg) % g_x;
// //     Polynomial c_x = (temp * poly_msg) + r_x; // Codeword polynomial
    
// //     const int* c_x_coeff = c_x.getCoefficients();
// //     for (int i = 0; i < 15; i++) {
// //         codeword[i] = (i < c_x.degree() + 1) ? c_x_coeff[i] : 0;
// //     }
// // }

// // // Error polynomial function
// // void err_poly(int* codeword, int* err_coeff) {
// //     Polynomial w_x(codeword, 15);
// //     int g_x_coeff[9] = {1, 1, 1, 0, 1, 0, 0, 0, 1};
// //     Polynomial g_x(g_x_coeff, 9);
// //     Polynomial s_x = w_x % g_x;
// //     Serial.println("s_x: "+ s_x.toString());
// //     const int* s_j_x_coeff;
// //     const int s_j_x_degree = 0;
// //     int j = 0;

// //     for (int i = 0; i < 100; i++) {
// //         int* temp_coeff = (int*) calloc(i + 1, sizeof(int));
// //         temp_coeff[0] = 1;
// //         Polynomial temp(temp_coeff, i + 1);
// //         free(temp_coeff);

// //         Polynomial s_i_x = (s_x * temp) % g_x;
// //         Serial.println("s_i_x: "+s_i_x.toString()+" weight: "+s_i_x.weight());
// //         if (s_i_x.weight() <= 2) {
// //             //Serial.println("Hi");
// //             j = i;
// //             //int* s_i_x_coeff = new int[s_i_x.degree() + 1];
// //             //int s_i_x_degree = s_i_x.degree();
// //             s_j_x_coeff = s_i_x.getCoefficients();
// //             // for(int l = 0; l<15; l++){
// //             //     s_i_x.getCoefficients()[l];
// //             // }
// //             //Serial.println("degree: "+s_i_x_degree);
// //             //Serial.println("Hi");
// //             break;
// //         }
// //     }

// //     if (s_j_x_coeff == nullptr) {
// //         Serial.println("Failed to find a valid syndrome with weight <= 2");
// //         return;
// //     }

// //     int n = 15;
// //     int* temp_coeff2 = (int*) calloc(n - j + 1, sizeof(int));
// //     temp_coeff2[0] = 1;
// //     Polynomial temp(temp_coeff2, n - j + 1);
// //     free(temp_coeff2);

// //     int mod_poly_coeff[16] = {0};
// //     mod_poly_coeff[0] = 1;
// //     mod_poly_coeff[15] = 1;
// //     Polynomial mod_poly(mod_poly_coeff, 16);

// //     Polynomial s_j_x(s_j_x_coeff, s_j_x_degree + 1);
// //     Serial.println("s_j_x: "+ s_j_x.toString());
// //     Polynomial e_x = (temp * s_j_x) % mod_poly;
// //     Serial.println("err_poly: "+e_x.toString());
// //     delete[] s_j_x_coeff;

// //     const int* e_x_coeff = e_x.getCoefficients();
// //     for (int i = 0; i < 15; i++) {
// //         err_coeff[i] = (i < e_x.degree() + 1) ? e_x_coeff[i] : 0;
// //     }
    
// // }

// // // Decoder function
// // void decode_15_7(int* codeword, int* msg) {
// //     int err_coeff[15] = {0};
// //     err_poly(codeword, err_coeff);

// //     Polynomial w_x(codeword, 15);
// //     Polynomial e_x(err_coeff, 15);

// //     Polynomial corrected_code = w_x + e_x;
// //     const int* corrected_coeff = corrected_code.getCoefficients();

// //     // Assuming the original message length is 7
// //     for (int i = 0; i < 7; i++) {
// //         msg[i] = corrected_coeff[i]; // Adjust index to get the message part
// //     }
// // }

// // #endif // BCH_CODEC_H
