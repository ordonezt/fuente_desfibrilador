/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COMUNICACION_H
#define COMUNICACION_H

/*######## PARAMETROS DE LA UART ########*/
//Velocidad de la comunicacion
#define BAUDRATE  9600
//Velocidad de transmicion a PC
#define MILLIS_TRANSMITIR 300
#define TIMEOUT_UART      6000

#define LONGITUD_COMANDO  5

//Enumeracion de estados de la recepcion de la UART
typedef enum{
  ESPERANDO_INICIO,
  ESPERANDO_COMANDO,
  ESPERANDO_FINAL
}uart_estado_t;
//Enum de los comandos de recepcion por UART
typedef enum{
  NADA,
  CARGAR,
  DESCARGAR,
  REINICIAR
}uart_comandos_t;

void maquinaUart(char caracter);
void comunicacion_PC(char id[]);
void comunicacion_inicializar(void);

#endif /* COMUNICACION_H */
