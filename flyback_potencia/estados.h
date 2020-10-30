#include "comunicacion.h"

//Enumeracion de estados de la maquina
typedef enum{
  RESET,
  REPOSO,
  CARGANDO,
  CARGADO,
  DESCARGANDO,
}estado_t;

#define TIMEOUT_CARGADO   20000
#define TIMEOUT_CARGA     20000

void maquina_de_estados(void);

void transicion_cargar(estado_t *estado);
void transicion_cargado(estado_t *estado);
void transicion_descargar(estado_t *estado);
void transicion_reposo(estado_t *estado);

bool condicion_cargar(uart_comandos_t comando);
bool condicion_cargado(uart_comandos_t comando);
bool condicion_descargar(uart_comandos_t comando);
bool condicion_reposo(uart_comandos_t comando);

void accion_cargar(void);
void accion_cargado(void);
void accion_descargar(void);
void accion_reposo(void);
