#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <inttypes.h>

int socketCliente;
struct sockaddr_in direccionCliente;
socklen_t tamaño;
uint16_t puerto;
uint16_t puertoared;
char string[1000];

int main(int argc, char const *argv[]) {

    // Verificar que se pasen suficientes argumentos
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <IP> <Puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Convertir el argumento de puerto a número entero
    puerto = atoi(argv[2]);  // puerto pasado como argumento
    puertoared = htons(puerto);  // lo paso de formato host a red

    // Crear socket en formato IPv4, orientado a conexión
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente < 0) {
        perror("No se pudo crear el socket de Cliente");
        exit(EXIT_FAILURE);
    }

    tamaño = sizeof(direccionCliente);

    direccionCliente.sin_family = AF_INET;  // formato IPv4
    direccionCliente.sin_port = puertoared;  // número de puerto

    // Convertir la dirección IP del servidor a formato de red y asignarla
    if (inet_pton(AF_INET, argv[1], &direccionCliente.sin_addr) <= 0) {
        perror("Dirección IP inválida o no soportada");
        exit(EXIT_FAILURE);
    }

    // Intentar conectar al servidor
    if (connect(socketCliente, (struct sockaddr *)&direccionCliente, tamaño) < 0) {
        perror("No se pudo conectar");
        exit(EXIT_FAILURE);
    }

    // Recibir mensaje del servidor
    ssize_t bytesRecibidos = recv(socketCliente, string, sizeof(string), 0);
    if (bytesRecibidos < 0) {
        perror("Error al recibir");
        exit(EXIT_FAILURE);
    }

    // Imprimir mensaje recibido y número de bytes
    printf("Mensaje: %s. Número de bytes: %zd\n", string, bytesRecibidos);

    // Cerrar el socket
    close(socketCliente);

    return 0;
}

