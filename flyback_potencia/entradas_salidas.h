/*######## DEFINICION DE PINES ########*/
//Salidas digitales
#define CORTE     8
#define DESCARGA  11
#define LED       13
//Salidas analogicas
#define PWM       5

#define MILLIS_HEARTBEAT    300


void abrir_rele(void);
void cerrar_rele(void);
void apagar_fuente(void);
void encender_fuente(void);

void heartbeat(char id[]);

void entradas_salidas_inicializar(void);
