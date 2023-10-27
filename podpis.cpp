#include <iostream>
#include <cmath>

using namespace std;

int modExp(int base, int exponent, int modulus) {
    if (modulus == 1) return 0;
    int result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent % 2 == 1) { 
            result = (result * base) % modulus;
        }
        exponent = exponent / 2; 
        base = (base * base) % modulus;
    }
    return result;
}

int gcd(int a, int b) {
     while (b != 0) {
        int remainder = a % b;
        a = b;
        b = remainder;
    }
    return abs(a);
}
int modInverse(int number, int modulus) {
    int originalModulus = modulus;
    int inverse = 0, current = 1;

    if (modulus == 1)
        return 0;

    while (number > 1) {
        int quotient = number / modulus;
        int temp = modulus;

        modulus = number % modulus;
        number = temp;
        temp = inverse;

        inverse = current - quotient * inverse;
        current = temp;
    }

    if (current < 0)
        current += originalModulus;

    return current;
}

int selectPublicKey(int phi_n) {
    for (int i = 2; i < phi_n; i++) {
        if (gcd(i, phi_n) == 1) {
            return i;
        }
    }
}

void generateKeys(int &e, int &d, int &n) {
    int p = 61, q = 53;
    n = p * q;
    int phi_n = (p - 1) * (q - 1);
    cout << "Eulerove phi_n je: " << phi_n << "\n";

    e = selectPublicKey(phi_n);
    d = modInverse(e, phi_n);
}

unsigned int simpleHash(const string& data, int n) {
    unsigned int hash = 0;
    for (char c : data) {
        hash = (hash * 1313 + c) % n;
    }
    return hash;
}


int createSignature(const string &document, int d, int n) {
    int hash = simpleHash(document, n);
    return modExp(hash, d, n);
}

bool verifySignature(const string &document, int signature, int e, int n) {
    int hash = simpleHash(document, n);
    return hash == modExp(signature, e, n);
}

int main() {
    int e, d, n;
    generateKeys(e, d, n);

    cout << "Verejný kľúč: (" << e << ", " << n << ")\n";
    cout << "Súkromný kľúč: (" << d << ", " << n << ")\n";

    string document = "Hello, World!";
    int signature = createSignature(document, d, n);
    cout << "Podpis: " << signature << "\n";

    bool isValid = verifySignature(document, signature, e, n);
    cout << "Podpis je " << (isValid ? "platný" : "neplatný") << ".\n";


    return 0;
}
