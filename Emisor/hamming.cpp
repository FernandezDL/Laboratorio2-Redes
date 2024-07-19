#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

//Función que calcula si un número es potencia de dos
bool isPowerOfTwo(int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

int main() {
    int n, m, r = 0;
    string binaryMessage;
    bool flag = false;

    cout << "--------------- Emisor en codigo de Hamming ---------------" << endl << endl;

    while (!flag) {
        cout << "Ingrese la cadena a utilizar: " << endl;
        cin >> binaryMessage;

        m = binaryMessage.length();

        if (m > 0) { //Termina si la cadena tiene longitud mayor a 0
            while (pow(2, r) < (m + r + 1)) { //Calcula el número de bits de paridad
                r++;
            }

            flag = true;
        } else {
            cout << "La longitud de la cadena binaria debe ser mayor a 0, intente nuevamente." << endl;
        }
    }

    n = m + r;

    vector<char> hammingCode(n, '0'); //Arreglo para el mensaje codificado

    int j = 0;
    for (int i = 1; i <= n; i++) {
        if (isPowerOfTwo(i)) { //Si es potencia de dos
            hammingCode[i - 1] = 'p'; //Se inserta un caracter 'p' para marcar el espacio
        } else {
            hammingCode[i - 1] = binaryMessage[j];
            j++;
        }
    }

    //Calculo de los bits de paridad
    for (int i = 1; i <= n; i++) {
        if (isPowerOfTwo(i)) {
            int conteo = 0;

            for (int j = i; j <= n; j++) {
                if ((j & i) != 0) {
                    if (hammingCode[j - 1] == '1') {
                        conteo++;
                    }
                }
            }

            if (conteo % 2 == 0) {
                hammingCode[i - 1] = '0';
            } else {
                hammingCode[i - 1] = '1';
            }
        }
    }

    cout << endl << "--------------- Codigo de Hamming ---------------" << endl;
    for (char c : hammingCode) {
        cout << c;
    }

    return 0;
}
