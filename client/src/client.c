#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();
	valor = config_get_string_value(config,"CLAVE");
	if(valor == NULL){
		printf("No se encontro 'CLAVE' en tp0.config\n");
		exit(1);
	}
	ip = config_get_string_value(config,"IP");
	if(ip == NULL){
		printf("No se encontro 'IP' en tp0.config\n");
		exit(1);
	}
	puerto = config_get_string_value(config,"PUERTO");
	if(valor == NULL){
		printf("No se encontro 'PUERTO' en tp0.config\n");
		exit(1);
	}

	char msg_log[200];
	snprintf(msg_log,200,"Config leida\n\tClave: %s\n\tIp: %s\n\tPuerto: %s\n",valor,ip,puerto);
	log_info(logger,msg_log);

	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void){
	t_log* logger = log_create("tp0.log","client",true,LOG_LEVEL_INFO);
	if(logger == NULL){
		printf("No se pudo escribir el archivo de log tp0.log\n");
		exit(1);
	}
	return logger;
}

t_config* iniciar_config(void)
{
	t_config* config = config_create("tp0.config");
	if(config  == NULL){
		printf("No se pudo leer el archivo de configuración tp0.config\n");
		exit(1);
	}
	return config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline("> ");
	while(strcmp(leido,"") != 0){
		log_info(logger,leido);
		free(leido);
		leido = readline("> ");
	}
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	printf("Destruir config y logger\n");
	log_destroy(logger);
	config_destroy(config);
}
