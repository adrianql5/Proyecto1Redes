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

char string[1000];

int main() {
    puerto = 0x1388;//puerto 5000
    puertoared = htons(puerto);//lo paso de formato host a red

    socketServidor = socket(AF_INET, SOCK_STREAM, 0);//creo socket en formato IPv4,estilo orientado a conexion, protocolo por defecto
    if (socketServidor < 0) {
        //comprueo si hubo error al crear
        perror("No se pudo crear el socket de Servidor");
        exit(EXIT_FAILURE);
    }

    direccionSocket.sin_family = AF_INET;//formato IPv4
    direccionSocket.sin_addr.s_addr = htonl(INADDR_ANY);//direccion IP local del servidor usamos esa por default y la ponemso en formato de red
    direccionSocket.sin_port = puertoared;//numero de puerto

    //le asigno al socket su dirección 
    if (bind(socketServidor, (struct sockaddr *)&direccionSocket, sizeof(struct sockaddr_in)) < 0) {
        //compruebo errores
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }

    //pongo al servidor a hacer una escucha
    if (listen(socketServidor, 1) < 0) {
        //compruebo errores
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }
    //Hago que imprima desde que puerto está escuchando, paso el puerto de red a host.
    printf("Servidor escuchando en el puerto %d...\n", ntohs(direccionSocket.sin_port));


    tamaño = sizeof(direccionCliente);
    //acepto la conexion de la dirección del Cliente y el socket
    socketConexion = accept(socketServidor, (struct sockaddr *)&direccionCliente, &tamaño);

    if (socketConexion < 0) {
        //compruebo errores
        perror("Error al intentar aceptar");
        exit(EXIT_FAILURE);
    }


    //printeo que ha sido aceptada la dirección del cliente en formato texto y el puerto del cliente
    printf("Conexión aceptada desde %s:%d\n", inet_ntoa(direccionCliente.sin_addr), ntohs(direccionCliente.sin_port));

    
    //copio el mensaje en mi string
    strcpy(string, "Hola cliente, conexión establecida.\n");
    //envío el mensaje al socket de conexion
    ssize_t bytes_enviados = send(socketConexion, string, strlen(string), 0);

    if (bytes_enviados < 0) {
        //compruebo error
        perror("Error al enviar datos");
        exit(EXIT_FAILURE);
    }

    //imprimo el nº de bytes enviados
    printf("Número de bytes enviados: %zd\n", bytes_enviados);
    //cierro ambos sockets
    close(socketConexion);
    close(socketServidor);

    return 0;
}
