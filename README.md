# Laboratorio 2 - Detección y corrección de errores

## Contenido
- [Descripción](https://github.com/FernandezDL/Laboratorio2-Redes?tab=readme-ov-file#descripci%C3%B3n)
- [Temas vistos](https://github.com/FernandezDL/Laboratorio2-Redes?tab=readme-ov-file#temas-vistos)
- [Organización](https://github.com/FernandezDL/Laboratorio2-Redes?tab=readme-ov-file#organizaci%C3%B3n)
- [Compilación]()
- [Informe]()
- [Integrantes](https://github.com/FernandezDL/Laboratorio2-Redes?tab=readme-ov-file#integrantes)

## Descripción
Los errores de transmisión suceden en toda comunicación y es parte de los retos al momento de
implementar este tipo de sistemas el manejar adecuadamente las fallas que puedan ocurrir. Por lo
tanto, a lo largo de la evolución del Internet se han desarrollado distintos mecanismos que sirven tanto para la detección como para la corrección de errores.

## Temas vistos
- Analizar algoritmos
- Corrección de errores
- Detección de errores
- Código de Hamming
- Fletcher checksum
     - Fletcher-16
- Emisor de mensajes
- Receptor de mensajes
- Sockets
- Analisis de resultados
   
## Organización
├── Emmisor

├─────── fletcher-16.cpp

└─────── hamming.cpp

├── Receptor

├─────── fletcher-16.py

└─────── hamming.py

└── Statistics

├─────── Fletcher

├────────────── Emisor

├───────────────────── durations.txt

└───────────────────── noisy_results.txt

└────────────── Receptor

├───────────────────── noisy_output.txt

└───────────────────── receive_durations.txt

└─────── Hamming

├────────────── Emisor

├───────────────────── durations.txt

├───────────────────── noise_counts.txt

└───────────────────── noisy_results.txt

└────────────── Receptor

├───────────────────── error_counts.txt

├───────────────────── noise_output.txt

└───────────────────── receive_durations.txt

## Compilación
Para correr el código se necesita tener el código del emisor y receptor corriendo al mismo tiempo.

1. Primero se debe correr el código del receptor, al ser un código en python solo se debe darle al botón de ejecutar
2. Cuando se muestre ```Listening on {host}:{port}...``` en la consola, donde los valores de `host` y `port` pueden variar dependiendo de las configuraciones hechas, ya se puede proceder a compilar el código del emisor.
3. Al ser un código de C++ se debe tener otra terminal separada a la que está corriendo el código del receptor, en la cual se debe correr el comando ``` g++ -o fletcher-16 fletcher-16.cpp -lws2_32 ``` o ``` g++ -o hamming hamming.cpp -lws2_32 ``` respectivamente. Estos códigos deben correrse dentro de la carpeta donde se tenga el códgio funente, si no va a dar error.
4. Una vez el código ya haya compilado se corre el ejecutable con el comando ``` ./fletcher-16 ``` o ``` ./hamming ```, respectivamente
5. Con esto el programa empezará a ejecutarse, y enviará al receptor los mensajes que se hayan definido

## Informe
[Informe hecho para la práctica](https://docs.google.com/document/d/1Eo4X0Vu4rPYmADFMp9DyjS3EiYBp3Cb_1RQccAKgXIo/edit?usp=sharing)

## Integrantes
-Diana Lucía Fernández Villatoro - 21747

-Jennifer Michelle Toxcón Ordoñez - 21276
