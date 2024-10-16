/**
 * Código de Xabier Nóvoa Gómez y Adrian Quiroga Linares
 * Cliente
 */



#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h> 

/**
 * Convertir una cadena de caracteres minúsculos a mayúsculos
 * @param cadena cadena a convertir
 */
void toMayusculas(char cadena[]){
    int i = 0;
    while (cadena[i] != '\0')
    {
        cadena[i] = toupper(cadena[i]); // Convierte cada carácter a mayúsculas
        i++;
    }
}


int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        printf("Introducir nombre del archivo,IP y puerto como argumentos en ese orden");
        exit(EXIT_FAILURE);
    }
    //Abrir el archivo de entrada en modo lectura
    FILE *archivoEntrada = fopen(argv[1], "r");
    if (archivoEntrada == 0)
    {
        printf("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Buffer para almacenar cada línea
    char linea[1000];
    // IPv4
    char ip[INET_ADDRSTRLEN];
    strncpy(ip, argv[2], INET_ADDRSTRLEN);
    struct in_addr ipServidor; 

    //Convertir formato de presentation a network
    inet_pton(AF_INET, ip, (void *)&ipServidor.s_addr);
    int valorMensajeEnviado,valorMensajeRecibido;

    //Varibale para almacenar el mensaje
    char mensajeRecibido[1000];
    //Guardar el puerto de los argumentos
    uint16_t puerto = (uint16_t)atoi(argv[3]);
    int socketDatos;
    struct sockaddr_in direccionServidor;
    // Contador de bytes para comprobaciones y mensaje final

    int bytesEnviados = 0;
    int bytesRecibidos = 0;

    //Crear el socket de conexión
    socketDatos = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDatos < 0)
    {
        perror("No se pudo crear el servidor");
        exit(EXIT_FAILURE);
    }

    // Poner los datos de direccionServidor
    direccionServidor.sin_family = AF_INET;                // IPv4
    direccionServidor.sin_addr.s_addr = ipServidor.s_addr; // La dirección IPv4 entra cualquiera
    direccionServidor.sin_port = htons(puerto);

    if (connect(socketDatos, (struct sockaddr *)&direccionServidor, sizeof(struct sockaddr_in)) < 0)
    {
        perror("No se pudo asignar direccion ");
        exit(EXIT_FAILURE);
    }

    //El servidor está conectado
    printf("Servidor conectado");
    char nombreArchivo[100];
    strcpy(nombreArchivo, argv[1]);

    //Crear un archivo que tenga como nombre el nombre del archiovo recibido en mayúsculas
    toMayusculas(nombreArchivo);
    FILE* archivoSalida = fopen(nombreArchivo,"w");

    //Ir leyendo línea por línea, enviando línea por línea
    while (fgets(linea, sizeof(linea), archivoEntrada))
    {
        //Pillar una cadena hasta fin de línea
        char *lineaLeida = strtok(linea, "\n"); 

        valorMensajeEnviado = send(socketDatos, lineaLeida, strlen(lineaLeida) + 1, 0);
        if (valorMensajeEnviado < 0)
        {
            perror("Ocurrió un erro al enviar el mensaje:");
            exit(EXIT_FAILURE);
        }
        bytesEnviados += valorMensajeEnviado;
        valorMensajeRecibido = recv(socketDatos, mensajeRecibido, 1000, 0);
        //sleep(1) Para comprobar si el servidor responde de manera secuancial a las distintas peticiones
        
        //Comprobar si se recibió el mensaje
        if (valorMensajeRecibido < 0)
        {
            perror("Ocurrió un erro al recibir el mensaje:");
            exit(EXIT_FAILURE);
        }

        bytesRecibidos += valorMensajeRecibido;
        //Escribir línea a línea
        fprintf(archivoSalida,"%s\n",mensajeRecibido);
    }

    close(socketDatos);
    
    //Cerramos archivos y resumimos la información de envío

    fclose(archivoEntrada);
    fclose(archivoSalida);
    printf("------------------------------------------------\n");
    printf("Conexión cortada. Datos recibidos: %d bytes\n", bytesRecibidos);
    printf("Conexión cortada. Datos enviados: %d bytes\n", bytesEnviados);
    return 0;
}
