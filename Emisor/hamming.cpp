#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <bitset>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

//Función que calcula si un número es potencia de dos
bool isPowerOfTwo(int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

//Función para convertir una cadena a binario
string stringToBinary(const string& input) {
    string binaryString;
    for (char c : input) {
        bitset<8> binary(c);
        binaryString += binary.to_string();
    }
    return binaryString;
}

int main() {
    int n, m, r = 0;
    string message, binaryMessage;
    bool flag = false;

    // Inicializa la semilla del generador de números aleatorios
    srand(static_cast<unsigned int>(time(0)));

    cout << "--------------- Codigo de Hamming ---------------" << endl << endl;

    while (!flag) {
        cout << "Ingrese la cadena a utilizar: " << endl;
        cin >> message;

        if(message.length() > 0) { //Si no es una cadena vacia
            flag = true;
        } else{
            cout << "La longitud de la cadena debe ser mayor a 0, intente nuevamente." << endl;
        }
    }

    binaryMessage = stringToBinary(message); //Convierte la cadena a binario
    // Eliminar los espacios en blanco al final de binaryMessage
    binaryMessage.erase(find_if(binaryMessage.rbegin(), binaryMessage.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), binaryMessage.end());

    m = binaryMessage.length();

    while (pow(2, r) < (m + r + 1)) { //Calcula el número de bits de paridad
        r++;
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

    cout<< endl <<"--------------- Cadena con ruido ---------------"<<endl ;
    
    //Introduciendo ruido aleatoriamente en la cadena
    for (char c : hammingCode) {
        int randomNumber = rand() % 100;
        
        if(randomNumber == 0){ //Si hay probabilidad de 1/100
            if(c == '0'){ //Cambia el valor
                c = '1';
            } else{
                c = '0';
            }

            cout<<"\033[31m"<<c<<"\033[0m"; //Color rojo cambio de bit
        } else{
            cout<<c; //Color negro bit normal
        }
    }

    return 0;
}
