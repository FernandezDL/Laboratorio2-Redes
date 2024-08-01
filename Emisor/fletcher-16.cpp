#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <winsock2.h>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

uint16_t fletcher16(const vector<uint8_t>& data) {
    int sum1 = 0;
    int sum2 = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }

    return (sum2 << 8) | sum1;
}

string checksumToBinary(uint16_t checksum) {
    string binary = "";
    for (int i = 15; i >= 0; --i) {
        binary += (checksum & (1 << i)) ? '1' : '0';
    }
    return binary;
}

// Función para convertir una cadena a binario
string stringToBinary(const string& input) {
    string binaryString;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        bitset<8> binary(c);
        binaryString += binary.to_string();
    }
    
    return binaryString;
}

pair<string, bool> fletcherCode(string originalMessage) {
    string binaryMessage, codedMessage, noisyMessage;
    bool flag = false, noisy = false;

    // Inicializa la semilla del generador de números aleatorios
    srand(static_cast<unsigned int>(time(0)));

    cout << "--------------- Codigo Fletcher 16 ---------------" << endl << endl;

    while (!flag) {
        if (originalMessage.length() > 0) { // Si la cadena es mayor a 0
            binaryMessage = stringToBinary(originalMessage); // Convierte la cadena a binario

            // Eliminar los espacios en blanco al final de binaryMessage
            binaryMessage.erase(find_if(binaryMessage.rbegin(), binaryMessage.rend(), [](unsigned char ch) {
                return !isspace(ch);
            }).base(), binaryMessage.end());

            if (binaryMessage.length() % 8 != 0) { // Si la cadena no es múltiplo de 8
                while (binaryMessage.length() % 8 != 0) {
                    binaryMessage = '0' + binaryMessage;
                }
            }

            flag = true;
        } else {
            cout << "La longitud de la cadena binaria debe ser mayor a 0, intente nuevamente." << endl;
        }
    }

    vector<uint8_t> message;

    for (char c : binaryMessage) {
        message.push_back(c == '0' ? 0 : 1);
    }

    uint16_t checksum = fletcher16(message);
    string checksumBinary = checksumToBinary(checksum);

    codedMessage = binaryMessage + checksumBinary;
    
    cout << endl << "--------------- Codigo Fletcher-16 ---------------" << endl;

    for (char c : codedMessage) {
        int randomNumber = rand() % 100;

        if (randomNumber == 0) {
            if (c == '0') { // Cambia el valor
                c = '1';
            } else {
                c = '0';
            }

            cout << "\033[31m" << c << "\033[0m"; // Color rojo cambio de bit
            noisy = true; // Si un caracter se voltea, la cadena es ruidosa
        } else {
            cout << c; // Color negro bit normal
        }

        noisyMessage += c;
    }

    // Añadir la bandera "END" al final del mensaje
    noisyMessage += "END";

    return make_pair(noisyMessage, noisy);
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

    // Multiples pruebas
    int numTest = 100;
    vector<double> durations;
    vector<string> noisyResults;

    for (int i = 0; i < numTest; i++) {
        string message = "Mensaje Prueba " + to_string(i);
        auto result = fletcherCode(message);
        string noisyMessage = result.first;
        bool noisy = result.second;

        auto start = high_resolution_clock::now();

        // Send data
        if (send(s, noisyMessage.c_str(), noisyMessage.size(), 0) < 0) {
            cerr << "Send failed: " << WSAGetLastError() << endl;
            return 1;
        }

        auto end = high_resolution_clock::now();
        double duration = duration_cast<microseconds>(end - start).count();
        durations.push_back(duration);

        noisyResults.push_back(noisy ? "True" : "False");
    }

    closesocket(s);
    WSACleanup();

    // Output the durations to a file for further analysis
    ofstream durationFile("../Statistics/Fletcher/Emisor/durations.txt");
    if (!durationFile) {
        cerr << "Error opening durations file for writing" << endl;
        return 1;
    }
    for (double duration : durations) {
        durationFile << duration << endl;
    }
    durationFile.close();

    // Output the noisy results to a file
    ofstream noisyFile("../Statistics/Fletcher/Emisor/noisy_results.txt");
    if (!noisyFile) {
        cerr << "Error opening noisy results file for writing" << endl;
        return 1;
    }
    for (const string& result : noisyResults) {
        noisyFile << result << endl;
    }
    noisyFile.close();

    return 0;
}
