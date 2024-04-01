#include "client.h"

// Declaro logger global para poder acceder desde utils
t_log* logger;
t_config* config;

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	// INICIO LOGGER
	logger = iniciar_logger();

	// LEO CONFIGURACION
	config = iniciar_config();
	valor = config_get_string_value(config,"CLAVE");
	if(valor == NULL){
		log_error(logger, "No se encontro 'CLAVE' en tp0.config");
		exit(1);
	}
	ip = config_get_string_value(config,"IP");
	if(ip == NULL){
		log_error(logger, "No se encontro 'IP' en tp0.config");
		exit(1);
	}
	puerto = config_get_string_value(config,"PUERTO");
	if(puerto == NULL){
		log_error(logger, "No se encontro 'PUERTO' en tp0.config");
		exit(1);
	}

	char msg_log[200];
	snprintf(msg_log,200,
		"Config leida\n\tClave: %s\n\tIp: %s\n\tPuerto: %s\n",valor,ip,puerto);
	log_info(logger,msg_log);

	// ME CONECTO AL SERVER
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// LEO DE LA CONSOLA +
	// Armamos y enviamos el paquete
	paquete(conexion);
	// Cierro conexion, destruyo config y logger y termino el programa
	terminar_programa(conexion);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(){
	t_log* logger = log_create("tp0.log","client",true,LOG_LEVEL_INFO);
	if(logger == NULL){
		fprintf(stderr,"No se pudo escribir el archivo de log tp0.log\n");
		exit(1);
	}
	return logger;
}

t_config* iniciar_config()
{
	t_config* config = config_create("tp0.config");
	if(config  == NULL){
		log_error(logger,"No se pudo leer el archivo de configuración tp0.config");
		exit(1);
	}
	return config;
}

void leer_consola(t_paquete* paquete)
{
	while(true){
		char* leido = readline("> ");
		if(strcmp(leido,"") == 0){ // Si no se ingreso nada salir
			free(leido);
			return;
		}
		log_info(logger,leido);
		agregar_a_paquete(paquete, leido, strlen(leido)+1); // El +1 es para el null (/0)
		free(leido);
	}
}

void paquete(int conexion)
{
	t_paquete* paquete = crear_paquete();
	leer_consola(paquete); // Grabo en el paquete lo leido en consola
	enviar_paquete(paquete,conexion); // Envio el paquete al server
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
	exit(0);
}
