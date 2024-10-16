#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>

//Código de Xabier Nóvoa Gómez y Adrián Quiroga Linares

int main(int argc, char const *argv[])
{
    // Comprobar que se pasaron 2 argumentos (IP y puerto)
    if (argc != 3){
        printf("Introducir IP y puerto como argumentos\n");
        exit(EXIT_FAILURE);
    }



    // Copiamos la IP del argumento a una variable local
    char ipServidorCadena[INET_ADDRSTRLEN];
    strncpy(ipServidorCadena, argv[1], INET_ADDRSTRLEN);

    // Convertimos la IP a formato binario para usarla en la estructura sockaddr_in
    struct in_addr ipServidorBinario;
    inet_pton(AF_INET, ipServidorCadena, (void *)&ipServidorBinario.s_addr);

    char mensajeRecibido[10000];  // Buffer para almacenar el mensaje recibido

    // Convertimos el puerto a entero y lo guardamos como uint16_t
    uint16_t puertoServidor = (uint16_t)atoi(argv[2]);

    // Creamos el socket del cliente
    int socketCliente;
    struct sockaddr_in direccionServidor;

    socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente < 0){
        perror("No se pudo crear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    // Configuramos la estructura de la dirección del servidor
    direccionServidor.sin_family = AF_INET;                // Usamos IPv4
    direccionServidor.sin_addr.s_addr = ipServidorBinario.s_addr; // Dirección IPv4 del servidor
    direccionServidor.sin_port = htons(puertoServidor); // Convertimos el puerto al formato de red

    // Verificamos si la IP del servidor es válida
    if (inet_pton(AF_INET, argv[1], &direccionServidor.sin_addr) <= 0){
        perror("Dirección IP inválida o no soportada");
        exit(EXIT_FAILURE);
    }

    // Intentamos conectarnos al servidor
    if (connect(socketCliente, (struct sockaddr *)&direccionServidor, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo conectar al servidor");
        exit(EXIT_FAILURE);
    }

    // Bucle para recibir datos
    ssize_t n;
    size_t tamano_mensaje = 5;
    printf("Recibiendo datos...\n");

    while ((n = recv(socketCliente, mensajeRecibido, tamano_mensaje, 0)) > 0) {
        // Añadimos \0  para manejarlo como cadena
        mensajeRecibido[n] = '\0';
        printf("Mensaje recibido: %s. Número de bytes: %zd\n", mensajeRecibido, n);
    }

    // Comprobamos si hubo un error en la recepción
    if (n < 0) {
        perror("Error al recibir el mensaje");
    }

    // Cerramos el socket
    close(socketCliente);
    printf("------------------------------------------------\n");
    printf("Conexión cerrada.\n");

    return 0;
}
