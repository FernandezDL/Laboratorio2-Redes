def binaryToChecksum(binario):
    decimal = 0
    longitud = len(binario)

    for i in range(longitud):
        bit = int(binario[i])
        decimal += bit * (2 ** (longitud - 1 - i))

    return decimal

def fletcher16(data):
    sum1 = 0
    sum2 = 0

    for byte in data:
        sum1 = (sum1 + int(byte)) % 255
        sum2 = (sum2 + sum1) % 255

    return (sum2 << 8) | sum1

def main():
    flag = False

    print("\n\n--------------- Receptor en codigo Fletcher-16 ---------------\n")

    while not flag:
        binaryMessage = input("Ingrese la cadena a utilizar: \n")

        if(len(binaryMessage)>0):
            if(len(binaryMessage) % 8 != 0): #Si la longitud de la cadena no es múltiplo de 8
                print("\nLa longitud del mensaje es de ", len(binaryMessage), " y debe ser un multiplo de 8, intente nuevamente.")
                print("---------------------------------------------------------------------------\n\n")

            else: #Si la longitud de la cadena es múltiplo de 8
                flag = True 
        else:
            print("\nLa longitud del mensaje debe ser mayor a 0, intente nuevamente.")
            print("---------------------------------------------------------------------------\n\n")

    binaryChecksum = binaryMessage[-16:]
    message = binaryMessage[:-16]

    checksumFromBinary = binaryToChecksum(binaryChecksum)
    checksumFromMessage = fletcher16(message)

    if(checksumFromBinary == checksumFromMessage): #Si los checksum's coinciden
        print("\n\n--------------- Mensaje original --------------- \n", message)

    else:
        print("\n\n**************** Se detecto un error **************** \n")
        print("*************** El mensaje se descarta *************** \n")
    
if __name__ == "__main__":
    main()
