#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <inttypes.h>

int socketServidor;
int socketConexion;

struct sockaddr_in direccionSocket;
struct sockaddr_in direccionCliente;

socklen_t tamaño;

uint16_t puerto;
uint16_t puertoared;

ssize_t contador_bytes;

char string[1000];

int main(int argc, char **argv) {
    // Verificar que se pase un puerto en hexadecimal
    if (argc < 2) {
        fprintf(stderr, "Debes escribir el puerto en hexadecimal\n");
        exit(EXIT_FAILURE);
    }

    // Convertir el puerto de cadena a entero (en hexadecimal)
    puerto = atoi(argv[1]);
    puertoared = htons(puerto); // Convertir el puerto de host a formato de red

    // Crear el socket del servidor
    socketServidor = socket(AF_INET, SOCK_STREAM, 0); // IPv4, orientado a conexión (TCP)
    if (socketServidor < 0) {
        perror("No se pudo crear el socket de Servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura de la dirección del socket
    direccionSocket.sin_family = AF_INET; // Formato IPv4
    direccionSocket.sin_addr.s_addr = htonl(INADDR_ANY); // Aceptar conexiones desde cualquier IP local
    direccionSocket.sin_port = puertoared; // Número de puerto

    // Asignar la dirección al socket
    if (bind(socketServidor, (struct sockaddr *)&direccionSocket, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }

    // Poner el servidor en escucha
    if (listen(socketServidor, 1) < 0) {
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }

    // Imprimir el puerto en el que está escuchando (convirtiéndolo de red a host)
    printf("Servidor escuchando en el puerto %d...\n", ntohs(direccionSocket.sin_port));

    while (1) {
        tamaño = sizeof(direccionCliente);
        // Aceptar conexión de un cliente
        socketConexion = accept(socketServidor, (struct sockaddr *)&direccionCliente, &tamaño);

        if (socketConexion < 0) {
            perror("Error al intentar aceptar");
            exit(EXIT_FAILURE);
        }

        // Imprimir la dirección IP y puerto del cliente
        printf("Conexión aceptada desde %s:%d\n", inet_ntoa(direccionCliente.sin_addr), ntohs(direccionCliente.sin_port));

        // Copiar un mensaje al string y enviarlo al cliente
        strcpy(string, "Hola cliente, conexión establecida.\n");
        ssize_t bytes_enviados = send(socketConexion, string, strlen(string), 0);

        contador_bytes += bytes_enviados;

        if (bytes_enviados < 0) {
            perror("Error al enviar datos");
            exit(EXIT_FAILURE);
        }

        // Imprimir el número de bytes enviados
        printf("Número de bytes enviados: %zd\n", contador_bytes);

        // Cerrar la conexión con el cliente
        close(socketConexion);
    }

    // Cerrar el socket del servidor al terminar
    close(socketServidor);

    return 0;
}
