#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

int main(){
    string binaryMessage;
    bool flag = false;

    cout << "--------------- Emisor en codigo Fletcher 16 ---------------" << endl << endl;

    while(!flag){
        cout << "Ingrese la cadena a utilizar: " << endl;
        cin >> binaryMessage;

        if(binaryMessage.length() > 0){ //Si la cadena es mayor a 0
            if(binaryMessage.length() % 8 != 0){ //Si la cadena no es múltiplo de 8
                while(binaryMessage.length() % 8 != 0){
                    binaryMessage = '0' + binaryMessage;
                }
            }

            flag = true;
        } else{
            cout << "La longitud de la cadena binaria debe ser mayor a 0, intente nuevamente." << endl;
        }
    }

    cout << "Cadena a usar: " << binaryMessage << endl;

    vector<uint8_t> message;

    for(char c : binaryMessage) {
        message.push_back(c == '0' ? 0 : 1);
    }

    uint16_t checksum = fletcher16(message);
    string checksumBinary = checksumToBinary(checksum);

    cout << endl << "--------------- Codigo Fletcher-16 ---------------" << endl;
    cout << binaryMessage<<checksumBinary << endl;

    return 0;
}
