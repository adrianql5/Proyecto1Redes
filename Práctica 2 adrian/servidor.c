#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <inttypes.h>


int main(int argc, char **argv) {
    int socketCliente; // Socket para la conexión con el cliente
    socklen_t tamañoDireccionCliente; // Tamaño de la estructura sockaddr_in del cliente
    ssize_t totalBytesEnviados; // Contador de los bytes enviados
    struct sockaddr_in direccionServidor; // Dirección del socket del servidor
    struct sockaddr_in direccionCliente;  // Dirección del cliente

    char mensajeInicial[1000];  // Primer mensaje que se enviará al cliente
    char mensaje2[] = "Segundo mensaje máquina";  // Segundo mensaje que se enviará al cliente

    // Verificar que se pase un puerto en el argumento
    if (argc != 2) {
        printf("Indique un puerto de escucha como argumento\n");
        exit(EXIT_FAILURE);
    }

    // Convertir el puerto de cadena a entero y a formato de red
    uint16_t puertoServidor = atoi(argv[1]);
    puertoServidor = htons(puertoServidor); // Convertir el puerto de host a formato de red

    // Crear el socket del servidor (IPv4, orientado a conexión - TCP)
    int socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor < 0) {
        perror("No se pudo crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura de la dirección del servidor
    direccionServidor.sin_family = AF_INET;  // Usamos IPv4
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);  // Aceptar conexiones desde cualquier IP local
    direccionServidor.sin_port = puertoServidor;  // Puerto de escucha

    // Asignar la dirección al socket del servidor
    if (bind(socketServidor, (struct sockaddr *)&direccionServidor, sizeof(struct sockaddr_in)) < 0) {
        perror("No se pudo asignar la dirección");
        exit(EXIT_FAILURE);
    }

    // Poner el servidor en modo escucha (hasta 5 conexiones en cola)
    if (listen(socketServidor, 5) < 0) {
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }

    // Imprimir el puerto en el que está escuchando el servidor
    printf("Servidor escuchando en el puerto %d...\n", ntohs(direccionServidor.sin_port));

    while (1) {
        tamañoDireccionCliente = sizeof(direccionCliente);
        // Aceptar conexión de un cliente
        socketCliente = accept(socketServidor, (struct sockaddr *)&direccionCliente, &tamañoDireccionCliente);
        if (socketCliente < 0) {
            perror("Error al aceptar la conexión");
            exit(EXIT_FAILURE);
        }

        // Obtener y mostrar la dirección IP y puerto del cliente
        char ipCliente[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(direccionCliente.sin_addr), ipCliente, INET_ADDRSTRLEN);
        printf("La dirección IP de la conexión entrante es: %s:%d\n", ipCliente, ntohs(direccionCliente.sin_port));

        // Enviar el primer mensaje al cliente
        strcpy(mensajeInicial, "Hola cliente, conexión establecida.\n");
        ssize_t bytesEnviadosInicial = send(socketCliente, mensajeInicial, strlen(mensajeInicial), 0);
        totalBytesEnviados += bytesEnviadosInicial;

        if (bytesEnviadosInicial < 0) {
            perror("Error al enviar el primer mensaje");
            exit(EXIT_FAILURE);
        }

        // Enviar el segundo mensaje al cliente
        ssize_t bytesEnviadosSegundo = send(socketCliente, mensaje2, strlen(mensaje2), 0);
        totalBytesEnviados += bytesEnviadosSegundo;

        if (bytesEnviadosSegundo < 0) {
            perror("Error al enviar el segundo mensaje");
            exit(EXIT_FAILURE);
        }

        // Imprimir el número de bytes enviados en total
        printf("Número total de bytes enviados: %zd\n", totalBytesEnviados);

        // Cerrar la conexión con el cliente
        close(socketCliente);
    }

    // Cerrar el socket del servidor al finalizar
    close(socketServidor);

    return 0;
}
