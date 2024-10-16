/**
 * Código de Xabier Nóvoa Gómez
 * Servidor
 */


#include <stdlib.h>
#include <stdio.h>
#include <errno.h> //para perror
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h> // para toupper()

void toMayusculas(char cadena[])
{
    int i = 0;
    while (cadena[i] != '\0')
    {
        cadena[i] = toupper(cadena[i]); // Convierte cada carácter a mayúsculas
        i++;
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Indique un puerto de escucha como argumento");
        exit(EXIT_FAILURE);
    }

    int sockEscucha, sockDatos, valorMensajeRecibido, valorMensajeEnviado;  // El socket de conexión es el de cliente
    uint16_t puerto = (uint16_t)atoi(argv[1]); // Número de puerto con formato de host
    uint16_t puertoRed = htons(puerto);        // host to network,
    struct sockaddr_in direcEscucha;           // Estructura de la dirección de socket que los campos dominio, tipo y protocolo
    struct sockaddr_in direcDatos;
    socklen_t tamano = sizeof(struct sockaddr_in);
    char mensajeRecibido[1000];

    sockEscucha = socket(AF_INET, SOCK_STREAM, 0);
    if (sockEscucha < 0)
    {
        perror("No se pudo crear el servidor");
        exit(EXIT_FAILURE);
    }

    direcEscucha.sin_family = AF_INET;                // IPv4
    direcEscucha.sin_addr.s_addr = htonl(INADDR_ANY); // La dirección IPv4 entra cualquiera
    direcEscucha.sin_port = puertoRed;                // El número de puerto

    if (bind(sockEscucha, (struct sockaddr *)&direcEscucha, sizeof(struct sockaddr_in)) < 0)
    {
        perror("No se pudo asignar direccion ");
        exit(EXIT_FAILURE);
    }

    if (listen(sockEscucha, 5) < 0)
    {
        perror("Se ha producido un error al al intentar poner en escucha el servidor");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando por el puerto %d...\n", puerto);
    //Tener el cliente continuamente en escucha
    while (1)
    {
        sockDatos = accept(sockEscucha, (struct sockaddr *)&direcDatos, &tamano);
        if (sockDatos < 0)
        {
            perror("No se pudo aceptar la conexion");
            exit(EXIT_FAILURE);
        }
        char ipCliente[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(direcDatos.sin_addr), ipCliente, INET_ADDRSTRLEN);
        printf("La dirección IP de la conexión entrante es: %s:%d\n", ipCliente, ntohs(direcDatos.sin_port));
        printf("Recibiendo archivo y convirtiendo los datos\n");
        while ((valorMensajeRecibido = recv(sockDatos, mensajeRecibido, sizeof(mensajeRecibido) + 1, 0)) > 0)
        {
            toMayusculas(mensajeRecibido);
            valorMensajeEnviado = send(sockDatos, mensajeRecibido, valorMensajeRecibido, 0);
            if (valorMensajeEnviado < 0)
            {
                perror("Ocurrió un error al enviar el mensaje");
                exit(EXIT_FAILURE);
            }
        }
        //COntrol de errores
        if (valorMensajeRecibido < 0)
        {
            perror("Ocurrió un error al recibir el mensaje");
            exit(EXIT_FAILURE);
        }
        //Cerrar conexión
        close(sockDatos);

        printf("Todos los datos enviados\n");
        // Ponerle un \n al final del print para que haga flush
        printf("--------------------------------------------------------\n");
        printf("Esperando nueva conexión. Escuchando por el puerto %d...\n",puerto); 
    }
    close(sockEscucha);
    return 0;
}
