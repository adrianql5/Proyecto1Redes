#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>

//Código de Xabier Nóboa Gómez y Adrián Quiroga Linares

int main(int argc, char const *argv[])
{
    if(argc != 2){
        printf("Indique un puerto de escucha como argumento");
        exit(EXIT_FAILURE);
    }

    int sockserv, socketCliente, valorMensaje; // El socket de conexión es el de cliente
    uint16_t puerto = (uint16_t)atoi(argv[1]);            // Número de puerto con formato de host
    uint16_t puertoRed = htons(puerto);        //host to network,
    struct sockaddr_in direccionServidor;      // Estructura de la dirección de socket que los campos dominio, tipo y protocolo
    struct sockaddr_in direccionCliente;
    socklen_t tamano = sizeof(struct sockaddr_in);
    char mensaje[500] = "Hola, que tal estás?";
    char mensaje2[] = "Segundo mensaje máquina";
    sockserv = socket(AF_INET, SOCK_STREAM, 0);
    if (sockserv < 0)
    {
        perror("No se pudo crear el servidor");
        exit(EXIT_FAILURE);
    }
    direccionServidor.sin_family = AF_INET;                // IPv4
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY); // La dirección IPv4 entra cualquiera
    direccionServidor.sin_port = puertoRed;                // El número de puerto

    if (bind(sockserv, (struct sockaddr *)&direccionServidor, sizeof(struct sockaddr_in)) < 0)
    {
        perror("No se pudo asignar direccion ");
        exit(EXIT_FAILURE);
    }

    if (listen(sockserv, 5) < 0)
    {
        perror("Se ha producido un error al al intentar poner en escucha el servidor");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        socketCliente = accept(sockserv, (struct sockaddr *)&direccionCliente, &tamano);
        if (socketCliente < 0)
        {
            perror("No se pudo aceptar l a conexion");
            exit(EXIT_FAILURE);
        }
        char ipCliente[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(direccionCliente.sin_addr), ipCliente, INET_ADDRSTRLEN);
        printf("La dirección IP de la conexión entrante es: %s:%d\n",ipCliente,ntohs(direccionCliente.sin_port));



        valorMensaje = send(socketCliente, mensaje, strlen(mensaje), 0);
        if (valorMensaje < 0)
        {
            perror("Ocurrió un erro al enviar el mensaje");
            exit(EXIT_FAILURE);
        }

        sleep(1);  // Esperamos 1 segundo para simular que el servidor tarda entre envío y envío

        //Segundo mensaje, mismo código que el anterior pero con el +1
        valorMensaje = send(socketCliente, mensaje2, strlen(mensaje2) + 1, 0); //Aquí si que hay que acordarse del fin
        if (valorMensaje < 0)
        {
            perror("Ocurrió un erro al enviar el mensaje");
            exit(EXIT_FAILURE);
        }
        close(socketCliente);
    }
    close(sockserv);
    return 0;
}
