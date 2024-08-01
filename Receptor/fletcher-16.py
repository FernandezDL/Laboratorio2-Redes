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
    binary_list = [binary_str[i:i+8] for i in range(0, len(binary_str), 8)]
    text = ''.join([chr(int(b, 2)) for b in binary_list])
    return text

def fletcherCode(binaryMessage):
    flag = False

    while not flag:
        if len(binaryMessage) > 0:
            if len(binaryMessage) % 8 != 0:  # Si la longitud de la cadena no es múltiplo de 8
                print("\nLa longitud del mensaje es de ", len(binaryMessage), " y debe ser un multiplo de 8, intente nuevamente.")
                print("---------------------------------------------------------------------------\n\n")
            else:  # Si la longitud de la cadena es múltiplo de 8
                flag = True
        else:
            print("\nLa longitud del mensaje debe ser mayor a 0, intente nuevamente.")
            print("---------------------------------------------------------------------------\n\n")

    binaryChecksum = binaryMessage[-16:]
    message = binaryMessage[:-16]

    checksumFromBinary = binaryToChecksum(binaryChecksum)
    checksumFromMessage = fletcher16(message)

    if checksumFromBinary == checksumFromMessage:  # Si los checksum's coinciden
        print("\n\n--------------- Mensaje original --------------- \n", {binary_to_text(message)})
        return False
    else:
        print("\n\n**************** Se detecto un error **************** \n")
        print("*************** El mensaje se descarta *************** \n")
        return True

def main():
    import socket
    import time

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

            durations = []
            noisy = []
            numTest = 100

            for _ in range(numTest):
                startTime = time.time()

                # Receive the data
                data = b""
                while True:
                    chunk = connection.recv(4096)
                    if b"END" in chunk:
                        data += chunk[:chunk.find(b"END")]
                        break
                    data += chunk

                if data:
                    binaryMessage = data.decode()
                    response = fletcherCode(binaryMessage)

                endTime = time.time()
                durations.append((endTime - startTime) * 1e6)  # Convert to microseconds
                noisy.append(response)

                # Print results for debugging
                print(f"Processed message {_ + 1}/{numTest}, Duration: {(endTime - startTime) * 1e6} µs, Noisy: {response}")

            print("\n\n------------------------------------------------------------ \n")

            # Output the durations to a file for further analysis
            with open("Statistics/Fletcher/Receptor/receive_durations.txt", "w") as outfile:
                for duration in durations:
                    outfile.write(f"{duration}\n")

            with open("Statistics/Fletcher/Receptor/noisy_output.txt", "w") as outfile2:
                for k in noisy:
                    outfile2.write(f"{k}\n")

        finally:
            connection.close()

if __name__ == "__main__":
    main()
