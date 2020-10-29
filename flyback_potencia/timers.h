/* USER CODE BEGIN Header */
/**
* @file   timers.h
*
* @version 0.0.1
*
* @date   Septiembre 2020
*
* @author Tomás Bautista Ordóñez
*
* @brief  SC1902
*
* Este archivo contiene las cabeceras de la maquinaria de timers
*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TIMERS_H
#define TIMERS_H

//#include "stm32l4xx.h"
//#include "stdbool.h"
//#include "string.h"

#define TIMER_LEN     5
#define TIMER_LEN_ID  6

typedef struct{
  uint32_t cuenta;
  bool pausado;
  char id[TIMER_LEN_ID];
  void (*accion) (char[]);
}timer_t;

/**
 * Retorna el valor del tick actual
 * @return tick actual.
 */
uint32_t timer_get_tick(void);

/**
 * Agrega un timer a la lista con la configuracion espeificadada.
 * @param ms Cantidad de ms de timeout.
 * @param pausado true o false.
 * @param id Identificador del timer.
 * @param accion Accion a ejecutar cuando expire el timer.
 * @return 1 error, 0 ok.
 */
uint8_t timer_agregar(uint32_t ms, bool pausado, char id[], void (*accion) (char[]));

/**
 * Cambia el estado del timer (pausdo o no).
 * @param pausado true o false.
 * @param id Identificador del timer.
 * @return 1 error, 0 ok.
 */
uint8_t timer_cambiar_estado(char id[], bool pausado);

/**
 * Borra el timer.
 * @param id Identificador del timer.
 * @return 1 error, 0 ok.
 */
uint8_t timer_borrar(char id[]);

/**
 * Borra todos los timers.
 */
void timer_borrar_todos(void);

/**
 * Configura y transmite la configuracion del canal y los latiguillos del mismo.
 */
void timer_tarea(void);

/**
 * Configura o agrega un timer.
 * @param ms Cantidad de ms de timeout.
 * @param pausado true o false.
 * @param id Identificador del timer.
 * @param accion Accion a ejecutar cuando expire el timer.
 * @return 1 error, 0 ok.
 */
uint8_t timer_configurar(uint32_t ms, bool pausado, char id[], void (*accion) (char[]));

/**
 * Checkea si el timer esta o no corriendo.
 * @param id Identificador del timer.
 * @return true esta corriendo, false esta pausado.
 */
bool timer_esta_corriendo(char id[]);

#endif /* TIMERS_H */
