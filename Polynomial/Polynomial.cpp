// #include "Polynomial.h"

// Polynomial::Polynomial(const uint8_t *coeffs) {
//     coefficients = reinterpret_cast<uint8_t *>(malloc(sizeof(uint8_t) * (strlen(coeffs) + 1)));
//     for (int i = strlen(coeffs) - 1; i >= 0; i--) {
//         coefficients[i] = coeffs[strlen(coeffs) - i - 1] - '0';
//     }
// }

// String Polynomial::toString() {
//     String result = "";
//     for (int i = degree(); i >= 0; i--) {
//         if (coefficients[i] != 0) {
//             if (i > 0) {
//                 result += "x^" + String(i) + " + ";
//             } else {
//                 result += "1 + ";
//             }
//         }
//     }

//     if (result.endsWith("+ ")) {
//         result.substring(0, result.length() - 3);
//     }

//     return result;
// }

// uint8_t Polynomial::degree() const {
//     return degree();
// }

// uint8_t Polynomial::weight() const {
//     return weight();
// }

// uint8_t *Polynomial::getCoefficients() const {
//     return coefficients;
// }

// Polynomial Polynomial::operator+(const Polynomial &other) const {
//     // Pad the shorter list of coefficients with zeros
//     if (degree() > other.degree()) {
//         uint8_t *padded = new uint8_t[degree() + 1];
//         memcpy(padded, coefficients, degree() + 1);
//         for (int i = degree(); i >= other.degree(); i--) {
//             padded[i] = 0;
//         }
//         return Polynomial(padded);
//     } else {
//         uint8_t *padded = new uint8_t[other.degree() + 1];
//         memcpy(padded, other.coefficients, other.degree() + 1);
//         for (int i = degree(); i >= other.degree(); i--) {
//             padded[i] = 0;
//         }
//         return Polynomial(padded);
//     }
// }

// Polynomial Polynomial::operator*(const Polynomial &other) const {
//     // Initialize result coefficients
//     uint8_t *result = new uint8_t[degree() + other.degree() + 1];
//     for (int i = 0; i <= degree() + other.degree(); i++) {
//         result[i] = 0;
//     }
//     for (int i = 0; i <= degree(); i++) {
//         for (int j = 0; j <= other.degree(); j++) {
//             result[i + j] ^= coefficients[i] & other.coefficients[j];
//         }
//     }
//     return Polynomial(result);
// }

// Polynomial Polynomial::operator%(const Polynomial &modulus) const {
//     // Initialize result coefficients
//     uint8_t *result = new uint8_t[modulus.degree() + 1];
//     for (int i = 0; i <= modulus.degree(); i++) {
//         result[i] = 0;
//     }

//     int mod_len = modulus.degree();

//     while (degree() >= mod_len) {
//         if (coefficients[0] == 1) {
//             for (int i = 0; i < mod_len; i++) {
//                 result[i] ^= modulus.coefficients[i];
//             }
//         }
//         coefficients.remove(0);
//     }

//     return Polynomial(result);
// }

// bool Polynomial::operator==(const Polynomial &other) const {
//     return memcmp(coefficients, other.coefficients, sizeof(coefficients)) == 0;
// }