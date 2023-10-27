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
        exponent = exponent >> 1; 
        base = (base * base) % modulus;
    }
    return result;
}


int gcd(int a, int b) {
     while (b != 0) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return abs(a);
}
int modInverse(int a, int m) {
    int m0 = m;
    int y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        // q is quotient
        int q = a / m;
        int t = m;

        // m is remainder now, process same as Euclid's algo
        m = a % m, a = t;
        t = y;

        // Update y and x
        y = x - q * y;
        x = t;
    }

    // Make x positive
    if (x < 0)
        x += m0;

    return x;
}

void generateKeys(int &e, int &d, int &n) {
    // Krok 1: Vyber dve veľké prvocísla p a q (tu pre zjednodušenie používame malé čísla)
    int p = 61, q = 53;

    // Krok 2: Vypočítať n = p * q
    n = p * q;

    // Krok 3: Vypočítať Eulerovu funkciu phi(n)
    int phi_n = (p - 1) * (q - 1);

    // Krok 4: Vybrať verejný kľúč e
    for (int i = 2; i < phi_n; i++) {
        if (gcd(i, phi_n) == 1) {
            e = i;
            break;
        }
    }

    // Krok 5: Vypočítať súkromný kľúč d
    d = modInverse(e, phi_n);
}

unsigned int simpleHash(const string& data, int n) {
    unsigned int hash = 0;
    for (char c : data) {
        hash = (hash * 1313 + c) % n; // tu sme pridali mod n
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
