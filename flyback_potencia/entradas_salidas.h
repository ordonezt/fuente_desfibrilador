/*######## DEFINICION DE PINES ########*/
//Salidas digitales
#define CORTE     8
#define DESCARGA  11
#define LED       13
//Salidas analogicas
#define PWM       5

#define MILLIS_HEARTBEAT    300

#define CORRIENTE_LIMITE_DEFAULT  3000//mA
#define CORRIENTE_MAXIMA  3000//mA

#define CORRIENTE_OFFSET      3.6 //V
#define CORRIENTE_GANANCIA    0.001// V/A
#define CORRIENTE_RESOLUCION  255 //8 bits
#define CORRIENTE_VREF        5 //V

void abrir_rele(void);
void cerrar_rele(void);
void apagar_fuente(void);
void encender_fuente(void);

void heartbeat(char id[]);
void corriente_limite(uint16_t corriente);

void entradas_salidas_inicializar(void);
