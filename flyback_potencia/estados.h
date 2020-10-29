//Enumeracion de estados de la maquina
typedef enum{
  RESET,
  REPOSO,
  CARGANDO,
  CARGADO,
  DESCARGANDO,
}estado_t;

#define TIMEOUT_CARGADO   5000
#define TIMEOUT_CARGA     10000

void maquina_de_estados(void);

void transicion_cargar(estado_t *estado);
void transicion_cargado(estado_t *estado);
void transicion_descargar(estado_t *estado);
void transicion_reposo(estado_t *estado);

bool condicion_cargar(void);
bool condicion_cargado(void);
bool condicion_descargar(void);
bool condicion_reposo(void);

void accion_cargar(void);
void accion_cargado(void);
void accion_descargar(void);
void accion_reposo(void);
