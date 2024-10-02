## Sockets

Interfaz entre una **app** y la **capa de transporte**. La app solicita una transmision de datos y la capa de transporte la realiza.
Para la app, el **envío de un dato equivale a la escritura en un socket**, y la **recepción una lectura**.

Para  2 procesos situados en diferentes ordenadores necesitamos saber:
* La dirección **IP**  del *ordenador en el cual se ejecuta el proceso*
* EL **puerto que tiene asignado el proceso dentro del ordenador**

Los sockets se contruyen con **IP y puerto**. Las direcciones IP serían como la dirección de un edificio, y los puertos indican el piso y letra. Los sockets son los buzones, y se identifican por el número de socket.

## Servidores y Clientes

Un servidor es aquel que porporciona un servicio, mientras que el cliente es el que lo solicita.
Un servido web sirve las paginas web y un cliente web las solicita
Se denomina serviso al porgrama que efectúa el servicio como al ordenador en que se ejecuta.

## Sockets orientados a conexión
En estos sockets antes de transmitir datos se solicita una conexion. El porgrama qu solicita la conexion es el cliente, y el que la aceepta el servidor.
* Crear el socket
* Solicitar conexión o aceptarla
* Enviar y recibir datos
* Cerrar los sockets

## Sockets sin conexión
Este tipo de sockets permite enviar datos en cualquier omento sin necesidad de establecer una conexiónb previa. Los sockets de ambos extremos funciona de igual forma
* Crar el socket, asginar IP y Puerto origen
* Envio de daots y recepcion escribiendo y leyendo en el socket
* Cerrar el socket