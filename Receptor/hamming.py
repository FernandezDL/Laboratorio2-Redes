def is_power_of_two(n):
    return (n != 0) and ((n & (n - 1)) == 0)

def main():
    r = 0
    flag = False

    print("--------------- Receptor en codigo de Hamming ---------------\n")

    while not flag:
        binary_message = input("Ingrese la cadena a utilizar: \n")
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

    for i in range(1, n + 1):
        if is_power_of_two(i):
            conteo = 0
            for j in range(i, n + 1):
                if (j & i) != 0:
                    if hamming_code[j - 1] == '1':
                        conteo += 1
            if conteo % 2 != 0:
                error_position += i

    if error_position>n:
        print("La cadena tiene más de dos errores y no pueden ser identificados")
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

        print(f"\n\n--------------- Mensaje original --------------- \n{original_message}")
    elif error_position == 0:
        # Extracción del mensaje original
        original_message = ""
        for i in range(1, n + 1):
            if not is_power_of_two(i):
                original_message += hamming_code[i - 1]

        print(f"\n\n--------------- Mensaje original --------------- \n{original_message}")

if __name__ == "__main__":
    main()
