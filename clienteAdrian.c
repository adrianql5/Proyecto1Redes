#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Introducir IP y puerto como argumentos");
        exit(EXIT_FAILURE);
    }
    char ip[INET_ADDRSTRLEN];
    strncpy(ip, argv[1], INET_ADDRSTRLEN);
    struct in_addr ipServidor;
    inet_pton(AF_INET, ip, (void *)&ipServidor.s_addr);
    int valorMensaje;
    char mensaje[10000];
    uint16_t puerto = (uint16_t)atoi(argv[2]); 
    int sockeCliente;
    struct sockaddr_in direccionServidor;
    //char MensajeRecibido[500];
    sockeCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (sockeCliente < 0)
    {
        perror("No se pudo crear el servidor");
        exit(EXIT_FAILURE);
    }
    direccionServidor.sin_family = AF_INET;                // IPv4
    direccionServidor.sin_addr.s_addr = ipServidor.s_addr; // La dirección IPv4 entra cualquiera
    direccionServidor.sin_port = htons(puerto);
    if (connect(sockeCliente, (struct sockaddr *)&direccionServidor, sizeof(struct sockaddr_in)) < 0)
    {
        perror("No se pudo asignar direccion ");
        exit(EXIT_FAILURE);
    }

    sleep(2); //Tiempo para que le de tiempo a enviar el segundo mensaje
    //Por lo menos en localhost si quitamos el sleep le da tiempo a enviar los dos mensajes

    valorMensaje = recv(sockeCliente, mensaje,5000, 0); //Aumentar el tamaño máximo del mensaje que se puede enviar
    if (valorMensaje < 0)
    {
        perror("Ocurrió un erro al enviar el mensaje");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", mensaje);
    
    close(sockeCliente);
    printf("------------------------------------------------\n");
    printf("Conexión cortada. Datos recibidos: %d bytes\n",valorMensaje);
    return 0;
}
