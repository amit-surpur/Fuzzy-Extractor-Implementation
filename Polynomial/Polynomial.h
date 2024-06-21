#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <Arduino.h>

class Polynomial {
private:
    int* coefficients;
    int size;

public:
    Polynomial(const int* coeffs, int len) {
        size = len;
        coefficients = new int[size];
        for (int i = 0; i < size; ++i) {
            coefficients[i] = coeffs[i];
        }
    }

    Polynomial(const Polynomial& other) {
        size = other.size;
        coefficients = new int[size];
        for (int i = 0; i < size; ++i) {
            coefficients[i] = other.coefficients[i];
        }
    }

    ~Polynomial() {
        delete[] coefficients;
    }

    String toString() const {
        String result = "";
        bool firstTerm = true;
        for (int i = 0; i <size; i++) {
            if (coefficients[size-1-i] != 0) {
                if (!firstTerm) {
                    result += " + ";
                }
                if (i == 0) {
                    result += "1";
                } else if (i == 1) {
                    result += "x";
                } else {
                    result += "x^" + String(i);
                }
                firstTerm = false;
            }
        }
        return firstTerm ? "0" : result;
    }

    Polynomial operator+(const Polynomial& other) const {
        int maxSize = max(size, other.size);
        int* resultCoeffs = new int[maxSize];
        for (int i = 0; i < maxSize; ++i) {
            int a = i < size ? coefficients[size - 1 - i] : 0;
            int b = i < other.size ? other.coefficients[other.size - 1 - i] : 0;
            resultCoeffs[maxSize - 1 - i] = a ^ b;
        }
        Polynomial result(resultCoeffs, maxSize);
        delete[] resultCoeffs;
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        int resultSize = size + other.size - 1;
        int* resultCoeffs = new int[resultSize]();
        for(int i = 0; i<resultSize; i++){
          resultCoeffs[i] = 0;
        }
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < other.size; ++j) {
                resultCoeffs[i + j] ^= coefficients[i] & other.coefficients[j];
                //Serial.println(resultCoeffs[i+j]);
            }
        }
        Polynomial result(resultCoeffs, resultSize);
        delete[] resultCoeffs;
        return result;
    }

    Polynomial operator%(const Polynomial& modulus) const {
        int resultSize = size;
        int* resultCoeffs = new int[resultSize];
        for (int i = 0; i < resultSize; ++i) {
            resultCoeffs[i] = coefficients[i];
        }

        int modLen = modulus.size;
        while (resultSize >= modLen) {
            if (resultCoeffs[0] == 1) {
                for (int i = 0; i < modLen; i++) {
                    resultCoeffs[i] ^= modulus.coefficients[i];
                }
            }
            for (int i = 0; i < resultSize - 1; ++i) {
                resultCoeffs[i] = resultCoeffs[i + 1];
            }
            resultSize--;
        }

        Polynomial result(resultCoeffs, resultSize);
        delete[] resultCoeffs;
        return result;
    }

    int operator()(int x) const {
        int result = 0;
        for (int i = 0; i < size; ++i) {
            result = (result << 1) | coefficients[i];
        }
        return result;
    }

    bool operator==(const Polynomial& other) const {
        if (size != other.size) {
            return false;
        }
        for (int i = 0; i < size; ++i) {
            if (coefficients[i] != other.coefficients[i]) {
                return false;
            }
        }
        return true;
    }

    int degree() const {
        return size - 1;
    }

    int weight() const {
        int weight = 0;
        for (int i = 0; i < size; ++i) {
            weight += coefficients[i];
        }
        return weight;
    }

    int* getCoefficients() const {
        return coefficients;
    }
};

#endif
