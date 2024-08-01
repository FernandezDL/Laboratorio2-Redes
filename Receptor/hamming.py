def is_power_of_two(n):
    return (n != 0) and ((n & (n - 1)) == 0)

def binary_to_text(binary_str):
    # Divide el mensaje binario en bloques de 8 bits
    binary_list = [binary_str[i:i+8] for i in range(0, len(binary_str), 8)]
    # Convierte cada bloque de 8 bits a un carácter ASCII
    text = ''.join([chr(int(b, 2)) for b in binary_list])
    return text

def hammingCode(binary_message):
    r = 0
    flag = False

    print("--------------- Receptor en codigo de Hamming ---------------\n")

    while not flag:
        m = len(binary_message)

        if m > 0:
            while (2 ** r) < (m + r + 1):
                r += 1
            flag = True
        else:
            print("La longitud del mensaje debe ser mayor a 0, intente nuevamente.")

    n = m
    hamming_code = list(binary_message)

    # Verificación de los bits de paridad y corrección de errores
    error_position = 0
    error_count = 0

    for i in range(1, n + 1):
        if is_power_of_two(i):
            conteo = 0
            for j in range(i, n + 1):
                if (j & i) != 0:
                    if hamming_code[j - 1] == '1':
                        conteo += 1
            if conteo % 2 != 0:
                error_position += i

    if error_position > n:
        print("La cadena tiene más de dos errores y no pueden ser identificados")
        return True, "2+"

    elif error_position != 0:
        print("\n\n---------------------------------------------------------------------------")
        print(f"Error en la posición: {error_position}")
        hamming_code[error_position - 1] = '1' if hamming_code[error_position - 1] == '0' else '0'

        print("Mensaje corregido: ", end="")
        print("".join(hamming_code))

        # Extracción del mensaje original
        original_message = ""
        for i in range(1, n + 1):
            if not is_power_of_two(i):
                original_message += hamming_code[i - 1]

        print(f"\n\n--------------- Mensaje original --------------- \n{binary_to_text(original_message)}")
        return True, "1"

    elif error_position == 0:
        # Extracción del mensaje original
        original_message = ""
        for i in range(1, n + 1):
            if not is_power_of_two(i):
                original_message += hamming_code[i - 1]

        print(f"\n\n--------------- Mensaje original --------------- \n{binary_to_text(original_message)}")
        return False, "0"

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
            error_counts = []
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
                    response, error_count = hammingCode(binaryMessage)

                endTime = time.time()
                durations.append((endTime - startTime) * 1e6)  # Convert to microseconds
                noisy.append(response)
                error_counts.append(error_count)

                # Print results for debugging
                print(f"Processed message {_ + 1}/{numTest}, Duration: {(endTime - startTime) * 1e6} µs, Noisy: {response}, Errors: {error_count}")

            print("\n\n------------------------------------------------------------ \n")

            # Output the durations to a file for further analysis
            with open("Statistics/Hamming/Receptor/receive_durations.txt", "w") as outfile:
                for duration in durations:
                    outfile.write(f"{duration}\n")

            with open("Statistics/Hamming/Receptor/noisy_output.txt", "w") as outfile2:
                for k in noisy:
                    outfile2.write(f"{k}\n")

            with open("Statistics/Hamming/Receptor/error_counts.txt", "w") as outfile3:
                for count in error_counts:
                    outfile3.write(f"{count}\n")

        finally:
            connection.close()

if __name__ == "__main__":
    main()
