#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <bitset>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Función que calcula si un número es potencia de dos
bool isPowerOfTwo(int n) {
    return (n != 0) && ((n & (n - 1)) == 0);
}

// Función para convertir una cadena a binario
string stringToBinary(const string& input) {
    string binaryString;

    for (size_t i = 0; i < input.size() + 1; ++i) {
        char c = input[i];
        bitset<8> binary(c);
        binaryString += binary.to_string();
    }
    
    return binaryString;
}

pair<string, pair<bool, int>> hamming(string message) {
    int n, m, r = 0;
    string binaryMessage, finalMessage;
    bool flag = false, noisy = false;
    int noiseCount = 0;

    // Inicializa la semilla del generador de números aleatorios
    srand(static_cast<unsigned int>(time(0)));

    cout << "--------------- Codigo de Hamming ---------------" << endl << endl;

    while (!flag) {
        if (message.length() > 0) { // Si no es una cadena vacia
            flag = true;
        } else {
            cout << "La longitud de la cadena debe ser mayor a 0, intente nuevamente." << endl;
        }
    }

    binaryMessage = stringToBinary(message); // Convierte la cadena a binario
    // Eliminar los espacios en blanco al final de binaryMessage
    binaryMessage.erase(find_if(binaryMessage.rbegin(), binaryMessage.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), binaryMessage.end());

    m = binaryMessage.length();

    while (pow(2, r) < (m + r + 1)) { // Calcula el número de bits de paridad
        r++;
    }
    
    n = m + r;

    vector<char> hammingCode(n, '0'); // Arreglo para el mensaje codificado

    int j = 0;
    for (int i = 1; i <= n; i++) {
        if (isPowerOfTwo(i)) { // Si es potencia de dos
            hammingCode[i - 1] = 'p'; // Se inserta un caracter 'p' para marcar el espacio
        } else {
            hammingCode[i - 1] = binaryMessage[j];
            j++;
        }
    }

    // Calculo de los bits de paridad
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

    cout << endl << "--------------- Cadena con ruido ---------------" << endl;

    // Introduciendo ruido aleatoriamente en la cadena
    for (char& c : hammingCode) {
        int randomNumber = rand() % 100;
        
        if (randomNumber == 0) { // Si hay probabilidad de 1/100
            c = (c == '0') ? '1' : '0'; // Cambia el valor

            cout << "\033[31m" << c << "\033[0m"; // Color rojo cambio de bit
            noisy = true;
            noiseCount++;
        } else {
            cout << c; // Color negro bit normal
        }

        finalMessage += c;
    }

    finalMessage += "END";

    return make_pair(finalMessage, make_pair(noisy, noiseCount));
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cerr << "Could not create socket: " << WSAGetLastError() << endl;
        return 1;
    }

    // Setup the sockaddr_in structure
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(12345);

    // Connect to the server
    if (connect(s, (sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Connection failed: " << WSAGetLastError() << endl;
        return 1;
    }

    vector<double> durations;
    vector<string> noisyResults;
    vector<int> noiseCounts;

    // Generar el mensaje codificado con Hamming
    for (int i = 0; i < 100; i++) {
        string message = "Mensaje Prueba " + to_string(i);

        auto result = hamming(message);
        string hammingCode = result.first;
        bool noisy = result.second.first;
        int noiseCount = result.second.second;

        auto start = high_resolution_clock::now();

        // Send data
        if (send(s, hammingCode.c_str(), hammingCode.size(), 0) < 0) {
            cerr << "Send failed: " << WSAGetLastError() << endl;
            return 1;
        }

        auto end = high_resolution_clock::now();
        double duration = duration_cast<microseconds>(end - start).count();
        durations.push_back(duration);

        noisyResults.push_back(noisy ? "True" : "False");
        noiseCounts.push_back(noiseCount);
    }

    closesocket(s);
    WSACleanup();

    // Output the durations to a file for further analysis
    ofstream durationFile("../Statistics/Hamming/Emisor/durations.txt");
    if (!durationFile) {
        cerr << "Error opening durations file for writing" << endl;
        return 1;
    }
    for (double duration : durations) {
        durationFile << duration << endl;
    }
    durationFile.close();

    // Output the noisy results to a file
    ofstream noisyFile("../Statistics/Hamming/Emisor/noisy_results.txt");
    if (!noisyFile) {
        cerr << "Error opening noisy results file for writing" << endl;
        return 1;
    }
    for (const string& result : noisyResults) {
        noisyFile << result << endl;
    }
    noisyFile.close();

    // Output the noise counts to a file
    ofstream noiseCountFile("../Statistics/Hamming/Emisor/noise_counts.txt");
    if (!noiseCountFile) {
        cerr << "Error opening noise counts file for writing" << endl;
        return 1;
    }
    for (int count : noiseCounts) {
        noiseCountFile << count << endl;
    }
    noiseCountFile.close();

    return 0;
}
