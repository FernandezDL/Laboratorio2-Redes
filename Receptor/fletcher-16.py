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

def binary_to_text(binary_str):
    # Divide el mensaje binario en bloques de 8 bits
    n = int(binary_str, 2)
    binary_list = [binary_str[i:i+8] for i in range(0, len(binary_str), 8)]
    # Convierte cada bloque de 8 bits a un carácter ASCII
    text = ''.join([chr(int(b, 2)) for b in binary_list])
    return text

def fletcherCode(binaryMessage):
    flag = False

    # print("\n\n--------------- Receptor en codigo Fletcher-16 ---------------\n")

    while not flag:
        # binaryMessage = input("Ingrese la cadena a utilizar: \n")

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
        print("\n\n--------------- Mensaje original --------------- \n", {binary_to_text(message)})

    else:
        print("\n\n**************** Se detecto un error **************** \n")
        print("*************** El mensaje se descarta *************** \n")

def main():
    import socket

    # Configuration
    host = 'localhost'
    port = 12345

    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((host, port))
    sock.listen(1)

    print(f"Listening on {host}:{port}...")

    while True:
        connection, client_address = sock.accept()
        try:
            print(f"Connection from {client_address}")

            # Receive the data in small chunks
            data = b""
            while True:
                chunk = connection.recv(4096)
                if not chunk:
                    break
                data += chunk

            if data:
                binaryMessage = data.decode()
                fletcherCode(binaryMessage)
        finally:
            connection.close()
if __name__ == "__main__":
    main()
