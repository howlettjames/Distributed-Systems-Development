#include <iostream>
#include <thread>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>      

#include "SocketDatagrama.h"
#include "mongoose.h"

#define SERVIDOR_PUERTO 8200                                        // Puerto en el que buscaremos servicios    

using namespace std;
using namespace std::chrono;

static const char *s_http_port = "8000";                            // Puerto en el que correrá nuestro servidor web    
static struct mg_serve_http_opts s_http_server_opts;                // Estructura para almacenar opciones HTTP de nuestro servidor

static void handle_search_services(struct mg_connection *nc, struct http_message *hm) 
{
    SocketDatagrama socket_datagrama(0);        // Abrimos un socket para enviar un mensaje broadcast
    socket_datagrama.setBroadcast();            // Habilitamos el envió de mensajes broadcast
	char ip_broadcast[17];                      // Var donde almacenaremos la IP enviada por el usuario por medio de HTTP
    int numeros[2];                             // Numeros que enviaremos a los servidores del cap 9 de programación en Linux que estén disponibles 
    numeros[0] = 5;                                                 
    numeros[1] = 6;
    int resultado = 0;                          // Para almacenar el resultado de la suma de los números
    char ips[512] = {0};                        // Para almacenar en formato JSON las IP de los sevidores disponibles       
    char tiempos_respuesta[512] = {0};          // Para almacenar en formato JSON los tiempos de respuesta de los servers disponibles    
    bool firstTime = true;                      // Para checar la primera vez que recibimos una respuesta
    time_point<high_resolution_clock> start;    // Para almacenar el momento/tiempo en que se envió el mensaje de broadcast
    double elapsed_time_ms = 0;                 // Para almacenar los ms que tarda en responder cada servidor

    // CON ESTE MÉTODO OBTENEMOS LA IP QUE ENVIÓ EL CLIENTE Y LA COLOCAMOS DENTRO DE NUESTRA VAR ip_broadcast
	mg_get_http_var(&hm->body, "ip_broadcast", ip_broadcast, sizeof(ip_broadcast));
    printf("IP de Broadcast recibida: %s\n", ip_broadcast);
    
    // CREAMOS UN PAQUETE PARA SER ENVIADO BROADCAST A TODOS LOS SERVIDORES QUE ESTÉN DISPONIBLES EN EL PUERTO ESPECIFICADO
    PaqueteDatagrama paquete_broadcast((char *) numeros, sizeof(int) * 2, ip_broadcast, SERVIDOR_PUERTO);
    // CREAMOS OTRO PAQUETE VACÍO, PARA ALMACENAR LOS PAQUETES ENVIADOS POR LOS SERVIDORES DISPONIBLES
    PaqueteDatagrama paquete_recepcion(sizeof(int));

    sprintf(ips, "[");
    sprintf(tiempos_respuesta, "[");
    
    // ENVIAMOS EL MENSAJE DE BROADCAST
    socket_datagrama.envia(paquete_broadcast);
    
    printf("Esperando respuesta(s) al broadcast...\n");
    start = high_resolution_clock::now();
    while(socket_datagrama.recibeTimeout(paquete_recepcion, 2, 0) > 0)
    {
        // TOMAMOS EL TIEMPO/MOMENTO EN QUE EL SERVIDOR RESPONDIÓ
        auto stop = high_resolution_clock::now();
        // SE ALMACENAN EL NÚMERO DE MILISEGUNDOS QUE TARDÓ EN CONTESTAR
        elapsed_time_ms = duration<double, milli>(stop - start).count();

        printf("--------------------------------------------------\n");
        printf("IP: %s\n", paquete_recepcion.obtieneDireccion());
        printf("Puerto: %d\n", paquete_recepcion.obtienePuerto());
        resultado = *(int *) paquete_recepcion.obtieneDatos();
        printf("Resultado: %d\n", resultado);

        // PARA LA PRIMER RESPUESTA EL FORMATO ES LIGERAMENTE DIFERENTE, PUES NO LLEVA UNA COMA
        if(firstTime)
        {
            sprintf(ips + strlen(ips), "\"%s\"", paquete_recepcion.obtieneDireccion());
            sprintf(tiempos_respuesta + strlen(tiempos_respuesta), "%lf", elapsed_time_ms);
            firstTime = false;
        }
        else
        {
            sprintf(ips + strlen(ips), ", \"%s\"", paquete_recepcion.obtieneDireccion());
            sprintf(tiempos_respuesta + strlen(tiempos_respuesta), ", %lf", elapsed_time_ms);
        }
        // VOLVEMOS A TOMAR EL TIEMPO PARA CALCULAR LO QUE TARDA EL SIGUIENTE SERVIDOR EN RESPONDER
        start = high_resolution_clock::now();
    }
    printf("--------------------------------------------------\n");
    sprintf(ips + strlen(ips), "]");
    sprintf(tiempos_respuesta + strlen(tiempos_respuesta), "]");
    printf("%s\n", ips);
    printf("%s\n", tiempos_respuesta);
    printf("Ya no hay mas servidores\n\n");

	// Use chunked encoding in order to avoid calculating Content-Length
	mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
	// Output JSON object
	mg_printf_http_chunk(nc, "{ \"ips\": %s, \"tiempos\": %s }", ips, tiempos_respuesta);
	// Send empty chunk, the end of response
	mg_send_http_chunk(nc, "", 0);
}

// ESTA FUNCIÓN SE ENCARGA DE MANEJAR CUALQUIER TIPO DE EVENTO QUE OCURRA EN NUESTRO SERVIDOR WEB,
// CADA EVENTO LEVANTA UNA BANDERA Y SE NOS PROPORCIONA LA INFORMACIÓN RELACIONADA A ESE EVENTO A TRAVÉS 
// DEL APUNTADOR "EV_DATA", EL CUAL DEBEMOS CASTEAR AL TIPO DE ESTRUCTURA DE DATOS DEPENDIENDO DEL EVENTO,
// EN ESTE CASO MANEJAREMOS EVENTOS HTTP, POR ESA RAZÓN OCUPAMOS LA ESTRUCTURA "HTTP_MESSAGE"
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) 
{
    struct http_message *hm = (struct http_message *) ev_data;

    // CHECANDO QUE TIPO DE EVENTO OCURRIÓ
    switch(ev) 
    {
        case MG_EV_HTTP_REQUEST:
            // SI SE TRATA DE UN EVENTO HTTP, DEBEMOS CHECAR SI ES UNA PETICIÓN REST HACIA ALGÚN ENDPOINT e.g. "/search"
            // O SI SE ESTÁ PIDIENDO DEVOLVER ALGÚN ARCHIVO DENTRO DEL DIRECTORIO DEL SERVIDOR (ELSE) 
            if(mg_vcmp(&hm->uri, "/search") == 0) 
                handle_search_services(nc, hm);  // El usuario desea buscar los servicios disponibles en la IP de Broadcast que nos proporcionó
            else
                mg_serve_http(nc, (struct http_message *) ev_data, s_http_server_opts);
        break;
        default:
        break;
    }
}

int main(void) 
{
    struct mg_mgr mgr;                                  // manager de los eventos
    struct mg_connection *nc;                           // nuestra conexión 
    cs_stat_t st;                                       
    
    // INICIALIZANDO OBJETO MANEJADOR DE EVENTOS    
    mg_mgr_init(&mgr, NULL);                    
    // CREANDO CONEXIÓN DE ESCUCHA Y AÑADIÉNDOLA A LA FUNCIÓN "EV_HANDLER", EN EL PUERTO ESPECIFICADO             
    nc = mg_bind(&mgr, s_http_port, ev_handler);
    if(nc == NULL) 
    {
        fprintf(stderr, "Cannot bind to %s\n", s_http_port);
        exit(1);
    }

    // INDICANDO QUE ESA CONEXIÓN ACEPTA PROTOCOLO HTTP/WEBSOCKET
    mg_set_protocol_http_websocket(nc);
    // Set up HTTP server parameters
    s_http_server_opts.document_root = "web_root";  // Set up web root directory

    // CHECANDO QUE EL DIRECTORIO ROOT HAYA SIDO ESTABLECIDO CORRECTAMENTE
    if(mg_stat(s_http_server_opts.document_root, &st) != 0) 
    {
        fprintf(stderr, "%s", "Cannot find web_root directory, exiting\n");
        exit(1);
    }

    // CHECANDO COMO MÁXIMO CADA 1000MS, LOS EVENTOS QUE OCURREN EN NUESTRA CONEXIÓN
    printf("Starting web server on port %s\n", s_http_port);
    for(;;) 
        mg_mgr_poll(&mgr, 1000);

    mg_mgr_free(&mgr);
	return 0;
}
