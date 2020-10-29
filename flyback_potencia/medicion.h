//Entradas analogicas
#define ADC_CAP   3

/*######## PARAMETROS DE TEMPORIZACION ########*/
//Velocidad de muestreo 50ms, 20Hz
#define MILLIS_ADC  50
////Velocidad de transmicion a PC
//#define MILLIS_TRANSMITIR 300

/*######## PARAMETROS DEL ADC ########*/
#define ADC_GANANCIA    1
#define ADC_REFERENCIA  5//
#define ADC_OFFSET      0//Porcentual
#define ADC_RESOLUCION  1024//Cuentas

/*######## TENSION DE SEGURIDAD ########*/
#define TENSION_PELIGROSA_MINIMA  20//V

void medicion_inicializar(void);
