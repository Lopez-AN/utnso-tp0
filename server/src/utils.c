#include"utils.h"

t_log* logger;

int iniciar_servidor(t_log* logger)
{
	// Creacion del socket
	struct addrinfo hints; // Parametros de conexion
	struct addrinfo *server_info; // Almacena la info del servidor luego de llamar a getaddrinfo

	// Bind + Listen
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // Protocolo ipv4
	hints.ai_socktype = SOCK_STREAM; // Protocolo TCP
	// Este parametro + null en getaddrinfo permite aceptar cualquier IP que tenga asignada la máquina que está corriendo el proceso
	hints.ai_flags = AI_PASSIVE;

	int error_code = getaddrinfo(NULL, "4444", &hints, &server_info);
	if(error_code) {
		fprintf(stderr, "Error al crear socket (func getaddrinfo): %s [%d]\n", gai_strerror(error_code), error_code);
		exit(1);
	}

	// Creo el socket
	int socket_servidor = socket(
		server_info -> ai_family,
		server_info -> ai_socktype,
		server_info -> ai_protocol
	);

	// Lo bindeo
	bind(socket_servidor, server_info -> ai_addr, server_info -> ai_addrlen);
	// Pongo a escuchar, SOMAXCONN es el maximo numero de conexiones que acepta el OS
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(server_info); // Libero la memoria del struct generado por getaddrinfo

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log* logger)
{
	// Quedo a la escucha de un cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	printf(buffer);
	//log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
